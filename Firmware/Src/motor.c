#include "motor.h"
#include "tim.h"
#include "mpu6050.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"

const char FirmwareVer[] = "3.0";
const char EEPROMVer[]  = "2.00";
const char MCUVer[] = "STM32F103C8T6";

uint16_t  g_RunTime = 0;//ϵͳ����ʱ���������������ʼ������ÿ����1
uint16_t  g_BatVolt=0;//��ص�ѹ��ʵ��ֵ*100

//С������ģʽ:ң��ģʽ������Ѱ��ģʽ������������ģʽ
uint8_t g_CarRunningMode = CONTROL_MODE;

uint8_t g_u8MainEventCount;
uint8_t g_u8SpeedControlCount;
uint8_t g_u8SpeedControlPeriod;
uint8_t g_u8DirectionControlPeriod;
uint8_t g_u8DirectionControlCount;

uint8_t g_cMotorDisable = 0;//ֵ����0ʱ�������ת��������ֹͣת��

/******������Ʋ���******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;
float g_fCarAngle;

/******�ٶȿ��Ʋ���******/
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

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
PID_t g_tCarAnglePID = {470.0f, 0.00f, 0.85f};
PID_t g_tCarSpeedPID = {3.25f,  0.1f, 0.0f};//2.9 0.14

/******�������Ʋ���******/
float g_fBluetoothSpeed = 0.0f;
float g_fBluetoothDirection = 0.0f;
float g_fBluetoothDirectionOld = 0.0f;
float g_fBluetoothDirectionNew = 0.0f;
float g_fBluetoothDirectionOut = 0.0f;

//ȫ�ֱ�����ʼ������
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

//���ת���쳣���
static int AbnormalSpinFlag = 0;
void AbnormalSpinDetect(short leftSpeed,short rightSpeed)
{
    #define SPD_M 300
    static uint16_t count = 0;

    //�ٶ�����Ϊ0ʱ��⣬���򲻼��
    if(g_iCarSpeedSet==0)
    {
        if(((leftSpeed>SPD_M)&&(rightSpeed>SPD_M))||((leftSpeed<-SPD_M)&&(rightSpeed<-SPD_M)))
        {
            // ���ҵ��ת�ٴ���SPD_M��������ͬ������ʱ�䳬��250ms�����ж�Ϊ��ת
            count++;
            if(count>50)
            {
                count = 0;
                AbnormalSpinFlag = 1;//ʧ��ͣת�������ŵؼ��
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

//С���ŵؼ��
void LandingDetect(void)
{
    static float lastCarAngle = 0;
    static uint16_t count = 0,count1 = 0;

    if(AbnormalSpinFlag == 0)return;

    // С���Ƕ�5��~-5���������
    if((g_fCarAngle > -5) && (g_fCarAngle < 5))
    {
        count1++;
        if(count1 >= 50)
        {   //ÿ��250ms�ж�һ��С���Ƕȱ仯�����仯��С��0.8������-0.8���ж�ΪС����ֹ
            count1 = 0;
            if(((g_fCarAngle - lastCarAngle) < 0.8) && ((g_fCarAngle - lastCarAngle) > -0.8))
            {
                count++;
                if(count >= 4) {
                    count = 0;
                    count1 = 0;
                    g_fCarPosition = 0;//λ�ƻ�������
                    AbnormalSpinFlag = 0;//�Ѿ��ŵأ�ȡ�����
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

//���ʹ��/ʧ�ܿ���
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

//���ת�ټ�������ƺ���
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

//����������
void MotorOutput(void)
{
    g_fLeftMotorOut  = g_fAngleControlOut + g_fSpeedControlOut + g_fBluetoothDirection ;
    g_fRightMotorOut = g_fAngleControlOut + g_fSpeedControlOut - g_fBluetoothDirection ;

    /*������������*/
    if((int)g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
    else if((int)g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
    if((int)g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
    else if((int)g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

    /*������ʹ�����ֹ����PWM��Χ*/
    if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX) g_fLeftMotorOut  = MOTOR_OUT_MAX;
    if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN) g_fLeftMotorOut  = MOTOR_OUT_MIN;
    if((int)g_fRightMotorOut > MOTOR_OUT_MAX) g_fRightMotorOut = MOTOR_OUT_MAX;
    if((int)g_fRightMotorOut < MOTOR_OUT_MIN) g_fRightMotorOut = MOTOR_OUT_MIN;

    SetMotorVoltageAndDirection((int)g_fLeftMotorOut,(int)g_fRightMotorOut);
}

//�ɼ�����ٶ�����
void GetMotorPulse(void)
{
    g_s16LeftMotorPulse = ( int32_t )__HAL_TIM_GET_COUNTER(&htim2);
    g_s16RightMotorPulse = -1 * ( int32_t )__HAL_TIM_GET_COUNTER(&htim4);
    TIM2->CNT = 0;
    TIM4->CNT = 0;   //����
    g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
    g_s32RightMotorPulseSigma += g_s16RightMotorPulse;
}

//�ٶȻ����ƺ���
void SpeedControl(void)
{
    float fP,fI;// �ٶȱ������ٶȻ���
    float fDelta;

    g_fCarSpeed = (g_s32LeftMotorPulseSigma  + g_s32RightMotorPulseSigma ) * 0.5 ;
    g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;   //ȫ�ֱ��� ע�⼰ʱ����

    g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;
    g_fCarSpeedOld = g_fCarSpeed;

    fDelta = CAR_SPEED_SET - g_fCarSpeed;

    fP = fDelta * (g_tCarSpeedPID.P);
    fI = fDelta * (g_tCarSpeedPID.I);

    g_fCarPosition += fI;

    //������������
    if((int32_t)g_fCarPosition > CAR_POSITION_MAX)    g_fCarPosition = CAR_POSITION_MAX;
    if((int32_t)g_fCarPosition < CAR_POSITION_MIN)    g_fCarPosition = CAR_POSITION_MIN;

    g_fSpeedControlOutOld = g_fSpeedControlOutNew;
    g_fSpeedControlOutNew = fP + g_fCarPosition;
}

//�ǶȻ����ƺ���
void AngleControl(void)
{
    g_fCarAngle = Pitch - CAR_ZERO_ANGLE;
    g_fAngleControlOut =  g_fCarAngle * g_tCarAnglePID.P + gyro[1] * (g_tCarAnglePID.D);
}

void SpeedControlOutput(void)
{
    float fValue;// �����ٶȻ�������ٶȲ�
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

//���������
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

//����������Ƿ���ȷ
uint8_t XOR_Check(char * str, uint8_t  len,uint8_t checksum)
{
    uint8_t  sum=0;
    sum = XOR_Get(str, len);
    if(sum == checksum)
        return 1;
    else
        return 0;
}

//���ۼӺ�
uint8_t Sum_Get(char *dat,char len)
{
    char i,sum=0;
    for(i=0; i<len; i++)
    {
        sum += *dat++;
    }
    return sum;
}

//16λcrc����
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
 ����˳��:
 - ÿ��д����֮ǰ����Ҷ���ݶ�ȡ��������
 - ��Ҫ�洢�����ݰ�ָ����ַд��������
 - Ҷ����
 - ����������������д��flash

 ��������СΪ128�ֽڣ����ʵ����Ч�洢��ַ��Χ0x00-0x7F
 ���Ҫ���Ӵ洢��������ͨ�����󻺳���ʵ��
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
 flash����������ַ��Χ0x00--0x7F
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
 PID ������ʼ��
 ������Щ��Ĭ��ֵд��flash
 �ǳ����������flash�ж�ȡpid����
*/
void PIDInit()
{
#define PARA1 0x55
#define PARA2 0x52
    uint8_t flag[2];

    ReadFlash(original_PID_Addr, flag, 2);
    if((flag[0] == PARA1)&&(flag[1] == PARA2))
    {   // �ǳ�������
        PIDRead();
    }
    else
    {
        // ��������
        flag[0] = PARA1;
        flag[1] = PARA2;
        ProgramFlash(original_PID_Addr, flag, 2);
        ProgramFlash(original_PID_Addr+2, (uint8_t*)&g_tCarAnglePID, sizeof(PID_t));// ����ǶȻ�Ĭ��pid����
        ProgramFlash(original_PID_Addr+2+16, (uint8_t*)&g_tCarSpeedPID, sizeof(PID_t));// �����ٶȻ�Ĭ��PID����
    }
}

/*
 �����޸ĵ�PID����
 flag��0--���ֽǶȻ�pid��1--�����ٶȻ�pid
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
 ��ȡPID����������û�и������ȡĬ��ֵ
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
 ��PID�����ָ���Ĭ��ֵ
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
 �ϱ���̬����
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
 �ϱ�PIDֵ
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
 �ϱ��汾��Ϣ
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
 �ϱ�״̬��Ϣ
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
 APP���ݽ�������
 Э��淶:֡ͷ+����+����+У��
 ��:#17,CM,-27.314,-5.716*56;  ֡ͷ# ����17  ����CM,-27.314,-5.716 У��56������Ϊ�����ֽ���
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
            if((len>0)&&(len<100))//���ȴ���100�����ݺ���
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
                if(ret) //  У����ȷ����ʼ��������
                {
                    if(g_CarRunningMode == CONTROL_MODE)
                    {
                        if(strstr(p, "CS"))
                        {
                            Steer(0, 0);   //ֹͣ
                        }
                        else if(strstr(p, "CJ"))  // ҡ�˿���
                        {
                            strtok(p, ",");
                            fTemp1= atof(strtok(NULL,",")); // X������
                            fTemp2 = atof(strtok(NULL,"*"));// Y������
                            Steer(fTemp1 * 10, -fTemp2 * 10);
                        }
                        else if(strstr(p, "CM"))  //������Ӧ����
                        {
                            strtok(p, ",");
                            fTemp1 = atof(strtok(NULL, ","));   // Pitch
                            fTemp2 = atof(strtok(NULL, "*"));  // Roll
                            Steer(fTemp2 / 5, fTemp1 / 2);
                        }
                    }

                    if(strstr(p,"SNK")) // ����Ϊ��ͨң��ģʽ
                    {
                        g_CarRunningMode = CONTROL_MODE;
                    }
                    else if(strstr(p,"SUF"))  // ����Ϊ����������ģʽ
                    {
                    }
                    else if(strstr(p,"SIT"))  // ����Ϊ����Ѱ��ģʽ
                    {
                    }
                    else if(strstr(p,"SAP")) // ���ýǶȻ�PID
                    {
                        strtok(p, ",");
                        g_tCarAnglePID.P = atof(strtok(NULL,","));
                        g_tCarAnglePID.I = atof(strtok(NULL,","));
                        g_tCarAnglePID.D = atof(strtok(NULL,"*"));
                        //PIDWrite(AnglePID);
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p,"SSP"))  // �����ٶȻ�PID
                    {
                        strtok(p, ",");
                        g_tCarSpeedPID.P = atof(strtok(NULL,","));
                        g_tCarSpeedPID.I = atof(strtok(NULL,","));
                        g_tCarSpeedPID.D = atof(strtok(NULL,"*"));
                        //PIDWrite(SpeedPID);
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p, "GAP"))  //��ѯ�ǶȻ�PID
                    {
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p, "GSP"))  //��ѯ�ٶȻ�PID
                    {
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p,"RA"))  // �ǶȻ�pid�ָ�Ĭ��ֵ
                    {
                        PIDReset(AnglePID);
                        ResponsePID(AnglePID);
                    }
                    else if(strstr(p,"RV"))  //�ٶȻ�pis�ָ�Ĭ��ֵ
                    {
                        PIDReset(SpeedPID);
                        ResponsePID(SpeedPID);
                    }
                    else if(strstr(p,"RB"))  //��ѯ�汾������״̬�ϱ���Ϣ
                    {
                        StatusFlag = 1;
                        ResponseInfo();
                    }
                    else if(strstr(p,"RS")) //�ر�״̬�ϱ�
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
