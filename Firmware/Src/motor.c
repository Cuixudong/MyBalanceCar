#include "motor.h"
#include "tim.h"
#include "mpu6050.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"

const char FirmwareVer[] = "3.0";
const char EEPROMVer[]  = "2.00";
const char MCUVer[] = "STM32F103C8T6";

uint16_t  g_RunTime = 0;//系统运行时间计数，，开机后开始计数，每秒增1
uint16_t  g_BatVolt=0;//电池电压，实际值*100

//小车运行模式:遥控模式、红外寻迹模式，超声波跟随模式
uint8_t g_CarRunningMode = CONTROL_MODE;

uint8_t g_u8MainEventCount;
uint8_t g_u8SpeedControlCount;
uint8_t g_u8SpeedControlPeriod;
uint8_t g_u8DirectionControlPeriod;
uint8_t g_u8DirectionControlCount;

uint8_t g_cMotorDisable = 0;//值等于0时电机正常转动，否则停止转动

/******电机控制参数******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
float g_fCarAngle;

/******速度控制参数******/
short g_s16LeftMotorPulse = 0;
short g_s16RightMotorPulse = 0;

int g_s32LeftMotorPulseOld = 0;
int g_s32RightMotorPulseOld = 0;
int g_s32LeftMotorPulseSigma = 0;
int g_s32RightMotorPulseSigma = 0;

float g_fCarSpeed = 0.0f;
float g_iCarSpeedSet = 0.0f;
float g_fCarSpeedOld = 0.0f;
float g_fCarPosition = 0.0f;

/*-----角度环和速度环PID控制参数-----*/
PID_t g_tCarAnglePID = {470.0f, 0.00f, 0.85f};
PID_t g_tCarSpeedPID = {3.25f,  0.1f, 0.0f};//2.9 0.14

/******蓝牙控制参数******/
float g_fBluetoothSpeed = 0.0f;
float g_fBluetoothDirection = 0.0f;
float g_fBluetoothDirectionOld = 0.0f;
float g_fBluetoothDirectionNew = 0.0f;
float g_fBluetoothDirectionOut = 0.0f;

//全局变量初始化函数
void CarUpstandInit(void)
{
    g_s16LeftMotorPulse = g_s16RightMotorPulse = 0;
    g_s32LeftMotorPulseOld = g_s32RightMotorPulseOld = 0;
    g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;

    g_fCarSpeed = g_fCarSpeedOld = 0;
    g_fCarPosition = 0;
    g_fCarAngle    = 0;
    
    g_fAngleControlOut = g_fSpeedControlOut = g_fBluetoothDirectionOut = 0;
    g_fLeftMotorOut    = g_fRightMotorOut   = 0;
    g_fBluetoothSpeed  = g_fBluetoothDirection = 0;
    g_fBluetoothDirectionNew = g_fBluetoothDirectionOld = 0;

    g_u8MainEventCount=0;
    g_u8SpeedControlCount=0;
    g_u8SpeedControlPeriod=0;
}

//电机转速异常检测
static int AbnormalSpinFlag = 0;
void AbnormalSpinDetect(short leftSpeed,short rightSpeed)
{
    #define SPD_M 300
    static uint16_t count = 0;

    //速度设置为0时检测，否则不检测
    if(g_iCarSpeedSet==0)
    {
        if(((leftSpeed>SPD_M)&&(rightSpeed>SPD_M))||((leftSpeed<-SPD_M)&&(rightSpeed<-SPD_M)))
        {
            // 左右电机转速大于SPD_M、方向相同、持续时间超过250ms，则判断为空转
            count++;
            if(count>50)
            {
                count = 0;
                AbnormalSpinFlag = 1;//失控停转，允许着地检测
            }
        }
        else
        {
            count = 0;
        }
    }
    else
    {
        count = 0;
    }
}

//小车着地检测
void LandingDetect(void)
{
    static float lastCarAngle = 0;
    static uint16_t count = 0,count1 = 0;

    if(AbnormalSpinFlag == 0)return;

    // 小车角度5°~-5°启动检测
    if((g_fCarAngle > -5) && (g_fCarAngle < 5))
    {
        count1++;
        if(count1 >= 50)
        {   //每隔250ms判断一次小车角度变化量，变化量小于0.8°或大于-0.8°判断为小车静止
            count1 = 0;
            if(((g_fCarAngle - lastCarAngle) < 0.8) && ((g_fCarAngle - lastCarAngle) > -0.8))
            {
                count++;
                if(count >= 4) {
                    count = 0;
                    count1 = 0;
                    g_fCarPosition = 0;//位移积分清零
                    AbnormalSpinFlag = 0;//已经着地，取消检测
                }
            }
            else {
                count = 0;
            }
            lastCarAngle = g_fCarAngle;
        }
    }
    else
    {
        count1 = 0;
        count = 0;
    }
}

//电机使能/失能控制
void MotorManage(void)
{
    AbnormalSpinDetect(g_s16LeftMotorPulse, g_s16RightMotorPulse);
    LandingDetect();
    if(AbnormalSpinFlag)
    {
        g_cMotorDisable |= (0x01<<1);
    }
    else
    {
        g_cMotorDisable &= ~(0x01<<1);
    }
    if(g_fCarAngle > 30 || g_fCarAngle < (-30))
    {
        g_cMotorDisable |= (0x01<<2);
    }
    else
    {
        g_cMotorDisable &= ~(0x01<<2);
    }
}

//电机转速及方向控制函数
void SetMotorVoltageAndDirection(int i16LeftVoltage,int i16RightVoltage)
{
    if(i16LeftVoltage < 0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,-i16LeftVoltage);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,i16LeftVoltage);
    }

    if(i16RightVoltage < 0)
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,-i16RightVoltage);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,i16RightVoltage);
    }

    if(g_cMotorDisable)
    {
        //HAL_GPIO_WritePin(SHDN_GPIO_Port,SHDN_Pin,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
        __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);
    }
    else
    {
        //HAL_GPIO_WritePin(SHDN_GPIO_Port,SHDN_Pin,GPIO_PIN_SET);
    }
}

//电机输出函数
void MotorOutput(void)
{
    g_fLeftMotorOut  = g_fAngleControlOut + g_fSpeedControlOut + g_fBluetoothDirection ;
    g_fRightMotorOut = g_fAngleControlOut + g_fSpeedControlOut - g_fBluetoothDirection ;

    /*增加死区常数*/
    if((int)g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
    else if((int)g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
    if((int)g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
    else if((int)g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

    /*输出饱和处理，防止超出PWM范围*/
    if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX) g_fLeftMotorOut  = MOTOR_OUT_MAX;
    if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN) g_fLeftMotorOut  = MOTOR_OUT_MIN;
    if((int)g_fRightMotorOut > MOTOR_OUT_MAX) g_fRightMotorOut = MOTOR_OUT_MAX;
    if((int)g_fRightMotorOut < MOTOR_OUT_MIN) g_fRightMotorOut = MOTOR_OUT_MIN;

    SetMotorVoltageAndDirection((int)g_fLeftMotorOut,(int)g_fRightMotorOut);
}

//采集电机速度脉冲
void GetMotorPulse(void)
{
    g_s16LeftMotorPulse = ( int32_t )__HAL_TIM_GET_COUNTER(&htim2);
    g_s16RightMotorPulse = -1 * ( int32_t )__HAL_TIM_GET_COUNTER(&htim4);
    TIM2->CNT = 0;
    TIM4->CNT = 0;   //清零
    g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
    g_s32RightMotorPulseSigma += g_s16RightMotorPulse;
}

//速度环控制函数
void SpeedControl(void)
{
    float fP,fI;// 速度比例，速度积分
    float fDelta;

    g_fCarSpeed = (g_s32LeftMotorPulseSigma  + g_s32RightMotorPulseSigma ) * 0.5 ;
    g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;   //全局变量 注意及时清零

    g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;
    g_fCarSpeedOld = g_fCarSpeed;

    fDelta = CAR_SPEED_SET - g_fCarSpeed;

    fP = fDelta * (g_tCarSpeedPID.P);
    fI = fDelta * (g_tCarSpeedPID.I);

    g_fCarPosition += fI;

    //积分上限设限
    if((int32_t)g_fCarPosition > CAR_POSITION_MAX)    g_fCarPosition = CAR_POSITION_MAX;
    if((int32_t)g_fCarPosition < CAR_POSITION_MIN)    g_fCarPosition = CAR_POSITION_MIN;

    g_fSpeedControlOutOld = g_fSpeedControlOutNew;
    g_fSpeedControlOutNew = fP + g_fCarPosition;
}

//角度环控制函数
void AngleControl(void)
{
    g_fCarAngle = Pitch - CAR_ZERO_ANGLE;
    g_fAngleControlOut =  g_fCarAngle * g_tCarAnglePID.P + gyro[1] * (g_tCarAnglePID.D);
}

void SpeedControlOutput(void)
{
    float fValue;// 两次速度环计算的速度差
    fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld ;
    g_fSpeedControlOut = fValue * (g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld;
}

float Scale(float input, float inputMin, float inputMax, float outputMin, float outputMax) {
    float output;
    if (inputMin < inputMax)
        output = (input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin));
    else
        output = (inputMin - input) / ((inputMin - inputMax) / (outputMax - outputMin));
    if (output > outputMax)
        output = outputMax;
    else if (output < outputMin)
        output = outputMin;
    return output;
}

void Steer(float direct, float speed)
{
    direct = -direct;
    speed = -speed;
    if(direct > 0)
        g_fBluetoothDirection = Scale(direct, 0, 10, 0, 600);
    else
        g_fBluetoothDirection = -Scale(direct, 0, -10, 0, 600);
    if(speed > 0)
        g_iCarSpeedSet = Scale(speed, 0, 10, 0, 580);
    else
        g_iCarSpeedSet = -Scale(speed, 0, -10, 0, 580);
}

//计算异或结果
uint8_t XOR_Get(char * str, uint8_t  len)
{
    uint8_t i=0;
    uint8_t  sum=0;
    for(i=0; i<len; i++)
    {
        sum ^= *str++;
    }
    return sum;
}

//检验异或结果是否正确
uint8_t XOR_Check(char * str, uint8_t  len,uint8_t checksum)
{
    uint8_t  sum=0;
    sum = XOR_Get(str, len);
    if(sum == checksum)
        return 1;
    else
        return 0;
}

//算累加和
uint8_t Sum_Get(char *dat,char len)
{
    char i,sum=0;
    for(i=0; i<len; i++)
    {
        sum += *dat++;
    }
    return sum;
}

//16位crc计算
uint16_t CRC16Calculate(uint8_t *buff, uint8_t len)
{
    uint16_t CRC_Temp;
    uint8_t i,j;
    CRC_Temp = 0xffff;

    for (i=0; i<len; i++)
    {
        CRC_Temp ^= buff[i];
        for (j=0; j<8; j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0800F000) /* Base @ of Page 60, 1 Kbytes */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0800F400) /* Base @ of Page 61, 1 Kbytes */
#define ADDR_FLASH_PAGE_62    ((uint32_t)0x0800F800) /* Base @ of Page 62, 1 Kbytes */
#define ADDR_FLASH_PAGE_63    ((uint32_t)0x0800FC00) /* Base @ of Page 63, 1 Kbytes */
#define ADDR_FLASH_PAGE_64    ((uint32_t)0x08010000) /* Base @ of Page 64, 1 Kbytes */

static uint8_t FlashWBuffer[128];
#define FLASH_START_ADDR ADDR_FLASH_PAGE_63//(0x08000000+1024*60)
#define FLASH_END_ADDR  FLASH_START_ADDR + FLASH_PAGE_SIZE
#define original_PID_Addr 0x0
#define Angle_PID_Addr  0x32
#define Speed_PID_Addr  0x48

/*
 操作顺序:
 - 每次写操作之前将整叶数据读取到缓冲区
 - 将要存储的数据按指定地址写到缓冲区
 - 叶擦除
 - 将整个缓冲区数据写到flash

 缓冲区大小为128字节，因此实际有效存储地址范围0x00-0x7F
 如果要增加存储容量，可通过增大缓冲区实现
*/
void ProgramFlash(uint32_t addr, uint8_t * data, uint16_t len)
{
    uint8_t i=0;
    uint32_t PAGEError = 0;
    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    for(i=0; i<128; i ++)
    {
        FlashWBuffer[i] = *((uint8_t*)(FLASH_START_ADDR+i));
    }
    for(i=0; i< len; i++)
    {
        FlashWBuffer[addr+i] = *data++;
    }

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_START_ADDR;
    EraseInitStruct.NbPages     = (FLASH_END_ADDR - FLASH_START_ADDR) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
    }
    else
    {
        for(i=0; i<128; i+=4)
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_START_ADDR+i, *((uint32_t*)&FlashWBuffer[i])) == HAL_OK)
            {
            }
            else
            {
                /* Error occurred while writing data in Flash memory.
                User can add here some code to deal with this error */
            }
        }
    }
    HAL_FLASH_Lock();
}

/*
 flash读操作，地址范围0x00--0x7F
*/
void ReadFlash(uint32_t addr, uint8_t * data, uint16_t len)
{
    uint8_t i;
    for(i=0; i<len; i++)
    {
        *data++ = *((uint8_t*)(FLASH_START_ADDR+addr+i));
    }
}

/*
 PID 参数初始化
 初次运些则将默认值写入flash
 非初次运行则从flash中读取pid参数
*/
void PIDInit()
{
#define PARA1 0x55
#define PARA2 0x52
    uint8_t flag[2];

    ReadFlash(original_PID_Addr, flag, 2);
    if((flag[0] == PARA1)&&(flag[1] == PARA2))
    {   // 非初次运行
        PIDRead();
    }
    else
    {
        // 初次运行
        flag[0] = PARA1;
        flag[1] = PARA2;
        ProgramFlash(original_PID_Addr, flag, 2);
        ProgramFlash(original_PID_Addr+2, (uint8_t*)&g_tCarAnglePID, sizeof(PID_t));// 保存角度环默认pid参数
        ProgramFlash(original_PID_Addr+2+16, (uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));// 保存速度环默认PID参数
    }
}

/*
 保存修改的PID参数
 flag：0--保持角度环pid，1--保存速度环pid
*/
void PIDWrite(uint8_t flag)
{
    uint8_t i=1;
    if(flag==0) {
        ProgramFlash(Angle_PID_Addr, &i, 1);
        ProgramFlash(Angle_PID_Addr+1,(uint8_t*)&g_tCarAnglePID, sizeof(PID_t));
    }
    else {
        ProgramFlash(Speed_PID_Addr, &i,1);
        ProgramFlash(Speed_PID_Addr+1,(uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));
    }
}

/*
 读取PID参数，参数没有更新则读取默认值
*/
void PIDRead()
{
    uint8_t i;

    ReadFlash(Angle_PID_Addr, &i, 1);
    if(i==1)
        ReadFlash(Angle_PID_Addr+1, (uint8_t*)&g_tCarAnglePID, sizeof(PID_t));
    else
        ReadFlash(original_PID_Addr+2, (uint8_t*)&g_tCarAnglePID, sizeof(PID_t));

    ReadFlash(Speed_PID_Addr, &i, 1);
    if(i==1)
        ReadFlash(Speed_PID_Addr+1, (uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));
    else
        ReadFlash(original_PID_Addr+2+16, (uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));
}

/*
 将PID参数恢复到默认值
*/
void PIDReset(uint8_t flag)
{
    uint8_t i=0;
    if(flag==0) {
        ProgramFlash(Angle_PID_Addr,  &i, 1);
        ReadFlash(original_PID_Addr+2, (uint8_t*)&g_tCarAnglePID, sizeof(PID_t));
    }
    else {
        ProgramFlash(Speed_PID_Addr, &i, 1);
        ReadFlash(original_PID_Addr+2+16, (uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));
    }
}

char StatusFlag=0;

/*
 上报姿态数据
*/
void ResponseIMU(void)
{
    char temp[32];
    sprintf(temp, "V,%.0f,%.0f,%.0f\r\n",
            (accel[0])/182.0f+180,
            gyro[1]/16.3f+180,
            Pitch+180);
    BluetoothOut(temp);
}

/*
 上报PID值
*/
void ResponsePID(PIDType type)
{
    char temp[32];
    if(type==AnglePID)
        sprintf(temp, "AP,%.2f,%.2f,%.2f\r\n",
                g_tCarAnglePID.P,
                g_tCarAnglePID.I,
                g_tCarAnglePID.D);
    else if(type == SpeedPID)
        sprintf(temp, "SP,%.2f,%.2f,%.2f\r\n",
                g_tCarSpeedPID.P,
                g_tCarSpeedPID.I,
                g_tCarSpeedPID.D);
    BluetoothOut(temp);
}

/*
 上报版本信息
*/
void ResponseInfo(void)
{
    char temp[32];
    sprintf(temp, "I,%s,%s,%s\r\n",
            FirmwareVer,
            EEPROMVer,
            MCUVer);
    BluetoothOut(temp);
}


/*
 上报状态信息
*/
void ResponseStatus(void)
{
    char temp[32];
    sprintf(temp, "R,%d,%d\r\n",
            g_BatVolt*10,
            g_RunTime);
    BluetoothOut(temp);
}

/*
 APP数据解析函数
 协议规范:帧头+长度+内容+校验
 如:#17,CM,-27.314,-5.716*56;  帧头# 长度17  内容CM,-27.314,-5.716 校验56，长度为内容字节数
*/
void Parse(char *dataInput)
{
    uint8_t len,check,ret;
    char *p = dataInput;
    float fTemp1, fTemp2;

#if COMMUNICATE_DEBUG_EN > 0
    if(Uart2Index) {
        DebugOutBuff(Uart2Buffer, Uart2Index);
        DebugOutStr("\r\n");
    }
#endif
    while(((p-dataInput)<Uart2Index))
    {
        if(*p == '#')//
        {
            len = (uint8_t)atof(strtok(p+1, ","));//length of data
            if((len>0)&&(len<100))//长度大于100的数据忽略
            {
                if(len<10)
                {
                    check = (uint8_t)atof(strtok(p+len+4, ";"));
                    ret = XOR_Check(p+3, len, check);
                    p += 3;
                }
                else
                {
                    check = (uint8_t)atof(strtok(p+len+5, ";"));
                    ret = XOR_Check(p+4, len, check);
                    p += 4;
                }
                if(ret) //  校验正确，开始解析数据
                {
                    if(g_CarRunningMode == CONTROL_MODE)
                    {
                        if(strstr(p, "CS"))
                        {
                            Steer(0, 0);   //停止
                        }
                        else if(strstr(p, "CJ"))  // 摇杆控制
                        {
                            strtok(p, ",");
                            fTemp1= atof(strtok(NULL,",")); // X轴坐标
                            fTemp2 = atof(strtok(NULL,"*"));// Y轴坐标
                            Steer(fTemp1 * 10, -fTemp2 * 10);
                        }
                        else if(strstr(p, "CM"))  //重力感应控制
                        {
                            strtok(p, ",");
                            fTemp1 = atof(strtok(NULL, ","));   // Pitch
                            fTemp2 = atof(strtok(NULL, "*"));  // Roll
                            Steer(fTemp2 / 5, fTemp1 / 2);
                        }
                    }

                    if(strstr(p,"SNK")) // 设置为普通遥控模式
                    {
                        g_CarRunningMode = CONTROL_MODE;
                    }
                    else if(strstr(p,"SUF"))  // 设置为超声波跟随模式
                    {
                    }
                    else if(strstr(p,"SIT"))  // 设置为红外寻迹模式
                    {
                    }
                    else if(strstr(p,"SAP")) // 设置角度环PID
                    {
                        strtok(p, ",");
                        g_tCarAnglePID.P = atof(strtok(NULL,","));
                        g_tCarAnglePID.I = atof(strtok(NULL,","));
                        g_tCarAnglePID.D = atof(strtok(NULL,"*"));
                        //PIDWrite(AnglePID);
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p,"SSP"))  // 设置速度环PID
                    {
                        strtok(p, ",");
                        g_tCarSpeedPID.P = atof(strtok(NULL,","));
                        g_tCarSpeedPID.I = atof(strtok(NULL,","));
                        g_tCarSpeedPID.D = atof(strtok(NULL,"*"));
                        //PIDWrite(SpeedPID);
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p, "GAP"))  //查询角度环PID
                    {
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p, "GSP"))  //查询速度环PID
                    {
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p,"RA"))  // 角度环pid恢复默认值
                    {
                        PIDReset(AnglePID);
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p,"RV"))  //速度环pis恢复默认值
                    {
                        PIDReset(SpeedPID);
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p,"RB"))  //查询版本及开启状态上报信息
                    {
                        StatusFlag = 1;
                        ResponseInfo();
                    }
                    else if(strstr(p,"RS")) //关闭状态上报
                    {
                        StatusFlag = 0;
                    }
                }
            }

        }
        p++;
    }
    Uart2Index=0;
    memset(Uart2Buffer, 0, Uart2BufferSize);
}

static float IMUData[4] = {0, 0, 0, 0};

void IMUToScope(void)
{
    int temp[4] = {0};
    uint32_t temp1[4] = {0};
    uint8_t databuf[10] = {0};
    uint8_t i;
    uint16_t CRC16 = 0;
    for(i = 0; i < 4; i++)
    {
        temp[i]  = (int)IMUData[i];
        temp1[i] = (uint32_t)temp[i];
    }
    for(i = 0; i < 4; i++)
    {
        databuf[i*2]   = (uint8_t)(temp1[i]%256);
        databuf[i*2+1] = (uint8_t)(temp1[i]/256);
    }
    CRC16 = CRC16Calculate(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;
    DebugOutBuff((char*)databuf, 10);
}


void IMUToFourAxisMonitor(void)
{
    short temp[3];
    uint8_t cTemp[32], Index = 0;
    uint8_t i = 0;

    temp[0] = IMUData[0];
    temp[1] = IMUData[1];
    temp[2] = IMUData[2];

    cTemp[Index++] = 0xaa;
    cTemp[Index++] = 0x55;
    cTemp[Index++] = 0x01;
    cTemp[Index++] = 0x06;

    for(i = 0; i < 3;  i ++)
    {
        cTemp[Index++] = (temp[i] >> 8);
        cTemp[Index++] = (temp[i]);
    }
    cTemp[Index++]= Sum_Get((char*)cTemp, Index);
    DebugOutBuff((char*)cTemp, Index);
}

void DebugService(void)
{
    IMUData[0] = accel[0];
    IMUData[1] = gyro[1];
    IMUData[2] = Pitch;

#if IMU_SCOPE_EN
    IMUToScope();
#endif
}
