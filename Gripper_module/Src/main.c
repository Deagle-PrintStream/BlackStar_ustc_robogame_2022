/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "bsp_stepper_init.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_stepper_T_speed.h"



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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	__HAL_RCC_SYSCFG_CLK_ENABLE();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  rx2_buffer[0]=1;
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); //使能IDLE中断	
  	HAL_UART_Receive_DMA(&huart2,rx2_buffer,BUFFER_SIZE);  //开启DMA接收
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); //使能IDLE中断	
  	HAL_UART_Receive_DMA(&huart3,rx3_buffer,BUFFER_SIZE);  //开启DMA接收
	stepper_Init();
	int i=0;
	int dir_val=1;
	TIM_PWMOUTPUT_Config();
	HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);	
	/*
	HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
	stepper_move_T(2500*10, accel_val, decel_val, set_speed);
	HAL_Delay(2000);
	HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
	*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {			/*	
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(6220*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				HAL_Delay(5000);
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(-6220*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				HAL_Delay(5000);
			*/
	  /*
	  for(int i=0;i<10;i++)
		rx3_buffer[i]=0;
		rx3_len = 0;//清除计数
		rec3_end_flag = 0;//清除接收结束标志位
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); //使能IDLE中断	
		HAL_UART_Receive_DMA(&huart3,rx3_buffer,BUFFER_SIZE);  //开启DMA接收
		HAL_Delay(2000);
		int flag=0;
		for(int i=0;i<10;i++)
		
		if((rx3_buffer[i]=='1')||(rx3_buffer[i]==0x01)||(rx3_buffer[i]==0x11)||(rx3_buffer[i]==0x31))
		
			flag=1;
		if(flag)
		{
			uint8_t newMsg[2]={0};
			newMsg[0]=0x08;
			newMsg[1]=0x01;
			HAL_UART_Transmit(&huart3,newMsg,2,0x00ff);
			HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(2480*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
		}
	  */
	  
	  if(rec2_end_flag)  //判断是否接收到1帧数据
			{
				Usart2_Handle(rx2_buffer);  //前往数据处理函数处理接收到的数据。
			}
	  
    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
