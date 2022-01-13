#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

#define CONTROL_MODE                1
#define INFRARED_TRACE_MODE         2
#define ULTRA_FOLLOW_MODE           3

#define CAR_ANGLE_SET               0
#define CAR_ANGULARSPEED_SET        0
#define GYRO_OFFSET                 0   //陀螺仪零点偏移值
#define CAR_POSITION_SET            0

#define CAR_ZERO_ANGLE              (3.5f)

/******速度控制相关宏定义******/
#define CAR_SPEED_SET               g_iCarSpeedSet
#define MOTOR_LEFT_SPEED_POSITIVE   (g_fLeftMotorOut >0)
#define MOTOR_RIGHT_SPEED_POSITIVE  (g_fRightMotorOut>0)
#define SPEED_CONTROL_PERIOD        25  //速度环控制周期

#define CAR_POSITION_MAX            600
#define CAR_POSITION_MIN            (-600)
/******电机控制相关宏定义******/
#define MOTOR_OUT_DEAD_VAL          700        //死区值
#define MOTOR_OUT_MAX               7190        //占空比正最大值
#define MOTOR_OUT_MIN               (-7190)     //占空比负最大值

typedef struct _PID_t
{
    float P;
    float I;
    float D;
} PID_t;
typedef enum _PIDType {AnglePID, SpeedPID,} PIDType;

extern uint8_t  g_cMotorDisable;
extern short    g_s16LeftMotorPulse;
extern short    g_s16RightMotorPulse;

extern int      g_s32LeftMotorPulseSigma;
extern int      g_s32RightMotorPulseSigma;
extern float    g_fCarAngle;

extern PID_t    g_tCarAnglePID;
extern PID_t    g_tCarSpeedPID;

extern float    g_fBluetoothSpeed;
extern float    g_fBluetoothDirection;
extern uint8_t  g_u8MainEventCount;
extern uint8_t  g_u8SpeedControlCount;
extern uint8_t  g_u8SpeedControlPeriod;
extern uint8_t  g_u8DirectionControlPeriod;
extern uint8_t  g_u8DirectionControlCount;

extern const char FirmwareVer[];
extern const char EEPROMVer[];
extern const char MCUVer[];

extern uint16_t g_RunTime;
extern uint16_t g_BatVolt;
extern uint8_t  g_CarRunningMode;
extern char     StatusFlag;

void SetMotorVoltageAndDirection(int i16LeftVoltage,int i16RightVoltage);
void CarUpstandInit(void);
void MotorManage(void);
void AngleControl(void)	 ;
void MotorOutput(void);
void SpeedControl(void);
void BluetoothControl(void)	;
void GetMotorPulse(void);
void SpeedControlOutput(void);
void DirectionControlOutput(void);
void DirectionControl(void);
void Steer(float direct, float speed);
void UltraControl(void);
void TailingControl(void);

void ProgramFlash(uint32_t addr, uint8_t * data, uint16_t len);
void ReadFlash(uint32_t addr, uint8_t * data, uint16_t len);
void PIDInit(void);
void PIDWrite(uint8_t flag);
void PIDRead(void);
void PIDReset(uint8_t flag);

void Parse(char *dataInput);
void ResponseIMU(void);
void ResponsePID(PIDType type);
void ResponseStatus(void);
void ResponseInfo(void);


#endif
