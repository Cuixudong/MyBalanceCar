#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "main.h"

//IO方向设置
#define MPU_SDA_IN()  {MPU_SDA_GPIO_Port->CRH&=0XFFFFFF0F;MPU_SDA_GPIO_Port->CRH|=0x00000080;}
#define MPU_SDA_OUT() {MPU_SDA_GPIO_Port->CRH&=0XFFFFFF0F;MPU_SDA_GPIO_Port->CRH|=0x00000030;}

#define SCL_H HAL_GPIO_WritePin(MPU_SCL_GPIO_Port,MPU_SCL_Pin,GPIO_PIN_SET)
#define SCL_L HAL_GPIO_WritePin(MPU_SCL_GPIO_Port,MPU_SCL_Pin,GPIO_PIN_RESET)

#define SDA_H HAL_GPIO_WritePin(MPU_SDA_GPIO_Port,MPU_SDA_Pin,GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(MPU_SDA_GPIO_Port,MPU_SDA_Pin,GPIO_PIN_RESET)

#define SDA_read HAL_GPIO_ReadPin(MPU_SDA_GPIO_Port,MPU_SDA_Pin)  //输入SDA

#define CLI()      __set_PRIMASK(1)
#define SEI()      __set_PRIMASK(0)

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define	true	1
#define	false	0
#define	bool	uint8_t

#define TRUE  0
#define FALSE -1

//IIC所有操作函数
bool i2cWriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
bool i2cWrite(uint8_t addr_, uint8_t reg_, uint8_t data);
bool i2cRead(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
void i2cInit(void);
static void i2cUnstick(void);

int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif
