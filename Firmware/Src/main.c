/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "mpu6050.h"
#include "motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
    /* Check the parameters */
    assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
    assert_param(IS_NVIC_OFFSET(Offset));

    SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

void Get_Battery_Message(float * vol,uint8_t * percent)
{
    static uint8_t flag = 0;
    uint32_t ADCConvertedValue;
    float battery=0;
    uint32_t temp=0;
    uint8_t i=0,j=0;

    for(i=0; i<5; i++)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 50);
        if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
        {
            ADCConvertedValue = HAL_ADC_GetValue(&hadc1);
            j++;
            temp += ADCConvertedValue;
        }
    }
    ADCConvertedValue = temp / j;
// HAL_ADC_Start(&hadc1);
// HAL_ADC_PollForConversion(&hadc1, 50);
// if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    {
//  ADCConvertedValue = HAL_ADC_GetValue(&hadc1);
        float bat = ADCConvertedValue * 3.3f * 11 / 4.096f;
        battery = (bat * 0.001f - 3.25f * 4) * 100 / (4.15f * 4 - 3.25f * 4);
        if(battery >= 100)
        {
            battery = 100;
        }
        else if(battery <= 0)
        {
            battery = 0;
        }
        if(!flag)
        {
            flag = 1;
            * vol = bat * 0.001f;
            * percent = (uint8_t)battery;
        }
        else
        {
            * vol = * vol * 0.9f + bat * 0.0001f;
            * percent = * percent * 0.9f + (uint8_t)battery * 0.1f;
        }
    }
}

#define LOW_POWER_EN 0

#if (LOW_POWER_EN > 0)

#define LOW_VOL 20    //低电量警告阈值
#define ENOUGH_VOL 35   //解除低电量阈值
#define POWER_TEST_TIME 1600 //一分钟持续检测

uint16_t low_power_count=0;  //低电量计数
uint8_t low_power_flag = 0;  //低电量标志

void low_power_check(void)
{
    float bat;
    uint8_t per;
    Get_Battery_Message(&bat,&per);
    if(per < LOW_VOL)
    {
        low_power_count ++;
        if(low_power_count > POWER_TEST_TIME)
        {
            low_power_count = POWER_TEST_TIME;
            low_power_flag = 1;
        }
    }
    else if(low_power_flag == 1)
    {
        if(per >= ENOUGH_VOL)
        {
            if(low_power_count > 0)
            {
                low_power_count --;
            }
            else if(low_power_count == 0)
            {
                low_power_flag = 0;
            }
        }
    }
}
#endif

extern unsigned short SoftTimer[5];
extern float Pitch,Roll,Yaw;
extern float g_fLeftMotorOut;
extern float g_fRightMotorOut;

volatile uint8_t system_ok_flag = 0;

//秒级任务
void SecTask()
{
    if(SoftTimer[0])
        return;
    else
    {
        SoftTimer[0] = 1000;
    }
    g_RunTime++;    // 记录运行时间

    g_BatVolt = 720; // 读取电池电压
    if(StatusFlag)
    ResponseStatus();

    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}

__IO uint8_t usart_temp;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(Uart2Index>=Uart2BufferSize)Uart2Index = 0;
        {
            Uart2Buffer[Uart2Index++] = usart_temp;
        }
        HAL_UART_Receive_DMA(&huart1,(uint8_t *)&usart_temp,1);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    char string_val[200];
    float battery=0;        //电压
    uint8_t persent=0;      //电量百分比
    uint8_t refresh_data=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    RTC_Init();

    i2cInit();    //初始化I2C
    MPU6050_Init();

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"Balance V1.3",16);
    OLED_Refresh_Gram();
    printf("hello world\r\n");

    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//CC2>CC1电机1轴顺时针旋转
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//CC4>CC3电机2轴顺时针旋转
    HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
    g_cMotorDisable = 0;

    //PIDInit();
    CarUpstandInit();

    system_ok_flag = 1;

    HAL_GPIO_WritePin(SHDN_GPIO_Port,SHDN_Pin,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    HAL_UART_Receive_DMA(&huart1,(uint8_t *)&usart_temp,1);

    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        SecTask();

        if(SoftTimer[1] == 0)
        {
            // 每隔50ms 执行一次
            SoftTimer[1] = 20;
            ResponseIMU();
            Parse(Uart2Buffer);
        }
        if(SoftTimer[2] == 0)
        {
            SoftTimer[2] = 20;
            HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
        }
        if(SoftTimer[3] == 0)
        {
            //低压告警,低于11.1V开启声光告警
            SoftTimer[3] = 100;
            Get_Battery_Message(&battery,&persent);
//            sprintf(string_val,"电压:%.2fV",battery);
//            show_str(30,16,(const uint8_t *)string_val,16,16);
            sprintf(string_val,"%3d%s",persent,"%");
            show_str(78 + 6,1,string_val,16,16);
            oled_showPicture(110+6,0,(uint8_t *)bmp_battery[persent*12/99],10,16);

            sprintf(string_val,"Ang:%4d.%02d",(int)Pitch,(int)fabsf((float)Pitch*100)%100);
            show_str(0,48,string_val,16,16);
            
            sprintf(string_val,"P:%5d,%5d",(int)g_fLeftMotorOut,(int)g_fRightMotorOut);
            show_str(0,16,string_val,12,16);
            
            sprintf(string_val,"E:%5d,%5d",(int)g_s16LeftMotorPulse,(int)g_s16RightMotorPulse);
            show_str(0,32,string_val,12,16);
            
            OLED_Refresh_Gram();
        }
#if (LOW_POWER_EN > 0)
        low_power_check();
        if(low_power_flag == 1)
        {
            refresh_data = 0;     //借用界面刷新数据标记
            OLED_Clear();
            show_str(0,16,"电量低请充电",16,16);
            while(1)
            {
                if((refresh_data % 30) == 0) //30 * 100 = 3S刷新一次显示
                {
                    Get_Battery_Message(&battery,&persent);
                    sprintf(string_val,"电压:%.03fV",battery);
                    show_str(0,32,(const uint8_t *)string_val,16,16);
                    sprintf(string_val,"%3d%s",(uint8_t)persent,"%");
                    show_str(78 + 6,1,(const uint8_t *)string_val,16,16);
                    oled_showPicture(110+6,0,bmp_battery[(uint8_t)persent*12/99],10,16);
                    sprintf(string_val,"%4d",(uint16_t)low_power_count);
                    show_str(0,48,(const uint8_t *)string_val,16,16);
                    OLED_Refresh_Gram();
                }
                refresh_data ++;
                low_power_check();

                if(low_power_flag == 0)
                {
                    OLED_Clear();
                    show_str(0,0,"Balance V1",16,16);
                    OLED_Refresh_Gram();
                    break;
                }
                HAL_Delay(99);
            }
        }
#endif
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
