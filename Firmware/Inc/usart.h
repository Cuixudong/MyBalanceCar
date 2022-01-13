/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define DEBUG_EN                1

#define IMU_SCOPE_EN            0   // 使能输出模拟示波器调试信息

#define COMMUNICATE_DEBUG_EN    0   // 使能输出蓝牙通信信调试信息

#if (DEBUG_EN == 1)

#define Uart2BufferSize 128

extern char Uart2Buffer[];      //蓝牙通信串口接收缓冲区
extern unsigned char Uart2Index;

void Uart1SendByte(char byte);
void Uart1SendBuff(char *buf, unsigned short len);
void Uart1SendStr(char *str);

#define DebugOutByte(byte)      Uart1SendByte(byte)
#define DebugOutBuff(buff, len) Uart1SendBuff(buff, len)
#define DebugOutStr(str)        Uart1SendStr(str)
#define BluetoothOut(str)       Uart1SendStr(str)

#else
#define DebugOutByte(byte)
#define DebugOutBuff(buff,len)
#define DebugOutStr(str)
#endif

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void DebugService(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
