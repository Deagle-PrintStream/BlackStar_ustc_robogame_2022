/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"
#include "bsp_stepper_T_speed.h"
/* USER CODE BEGIN 0 */




void sendSpeedCmdRaw(void)
{
  uint8_t newMsg[4]={0};
  newMsg[0]=(uint8_t)moveCmd;
  newMsg[1]=1;
  newMsg[2]=5;
  HAL_UART_Transmit_DMA(&huart2,newMsg,3);
}





void sendStopCmdRaw(void)
{
  uint8_t newMsg[4]={0};
  newMsg[0]=(uint8_t)stopCmd;
  newMsg[1]=0;
  newMsg[2]=0;
  HAL_UART_Transmit_DMA(&huart2,newMsg,3);
}







void receiveCmd(uint8_t *buff)
{
	for(int i=0;i<9;i++)
	if(buff[i]==7)
	{
		Usart3_Handle(); 
		break;
	}
  switch (buff[0])
  {
	  case armRaiseCmd:
		  if(buff[1]==0&&buff[2]==2)
		  {
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(2480*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
		  }
		  
		  if(buff[1]==0&&buff[2]==5)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(6220*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==0&&buff[2]==7)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(8700*10, accel_val, decel_val, set_speed);
				HAL_Delay(3000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==0&&buff[2]==8)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(9920*10, accel_val, decel_val, set_speed);
				HAL_Delay(4000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==1&&buff[2]==2)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(-2480*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==1&&buff[2]==5)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(-6220*10, accel_val, decel_val, set_speed);
				HAL_Delay(2000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==1&&buff[2]==7)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(-8700*10, accel_val, decel_val, set_speed);
				HAL_Delay(3000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  if(buff[1]==1&&buff[2]==8)
		  {
			if(1)	
			{
				HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
				stepper_move_T(-9920*10, accel_val, decel_val, set_speed);
				HAL_Delay(4000);
				HAL_TIM_OC_Stop_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
			}
		  }
		  break;
		  
		  
	  
		  
		  
  case errorCmd:
    //printf("errorCmd\t");
    break;
  
  default:
    //printf("otherCmd\t");
    break;
  }
  return;
 }
  
volatile uint8_t rx3_len = 0;  //接收一帧数据的长度
volatile uint8_t rec3_end_flag = 0; //一帧数据接收完成标志
uint8_t rx3_buffer[BUFFER_SIZE]={0};  //接收数据缓存数组
volatile uint8_t rx2_len = 0;  //接收一帧数据的长度
volatile uint8_t rec2_end_flag = 0; //一帧数据接收完成标志
uint8_t rx2_buffer[BUFFER_SIZE]={0};  //接收数据缓存数组
#if 1  
struct __FILE { 
  int handle; 
}; 

FILE __stdout;  //定义_sys_exit()以避免使用半主机模式   
void _sys_exit(int x) 
{ 
  x = x; 
} 
//重定义fputc函数
int fputc(int ch, FILE *f){ 	
  while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
  USART2->DR=(uint8_t)ch;      
  return ch;
}
#endif

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Channel7;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void Usart2_IDLE(void)      //USART1的IDLE接收
{   
	uint32_t tmp_flag = 0;   
	uint32_t temp;
   	tmp_flag =__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE); //获取IDLE标志位
   if((tmp_flag != RESET))//idle标志被置位
   {
   		__HAL_UART_CLEAR_IDLEFLAG(&huart2);//清除标志位
   		HAL_UART_DMAStop(&huart2); //  停止DMA传输，防止
   		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中未传输的数据个数   
   		rx2_len =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
   		rec2_end_flag = 1;	// 接受完成标志位置1	
   }
}

void Usart2_Handle(uint8_t *rx2_buffer)     //USART1对接收的一帧数据进行处理
{	
	receiveCmd(rx2_buffer);
   rx2_len = 0;//清除计数
   rec2_end_flag = 0;//清除接收结束标志位
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); //使能IDLE中断	
   HAL_UART_Receive_DMA(&huart2,rx2_buffer,BUFFER_SIZE);//重新打开DMA接收
}

void DMA_Usart2_Send(uint8_t *buf,uint8_t len) //串口发送封装
{   
	if(HAL_UART_Transmit_DMA(&huart2,buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
	{
  		Error_Handler();
 	}
}





void Usart3_IDLE(void)      //USART1的IDLE接收
{   
	uint32_t tmp_flag = 0;   
	uint32_t temp;
   	tmp_flag =__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE); //获取IDLE标志位
   if((tmp_flag != RESET))//idle标志被置位
   {
   		__HAL_UART_CLEAR_IDLEFLAG(&huart3);//清除标志位
   		HAL_UART_DMAStop(&huart3); //  停止DMA传输，防止
   		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);// 获取DMA中未传输的数据个数   
   		rx3_len =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
   		rec3_end_flag = 1;	// 接受完成标志位置1	
   }
}

void Usart3_Handle()     //USART1对接收的一帧数据进行处理
{
	for(int i=0;i<10;i++)
		rx3_buffer[i]=0;
	rx3_len = 0;//清除计数
   rec3_end_flag = 0;//清除接收结束标志位
   	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); //使能IDLE中断	
  	HAL_UART_Receive_DMA(&huart3,rx3_buffer,BUFFER_SIZE);  //开启DMA接收
	HAL_Delay(2000);
	int j=0;
	int k=0;
	int ans=0;
	while(1)
	{
		int flag=0;
		for(int i=0;i<10;i++)
		
		if((rx3_buffer[i]=='1')||(rx3_buffer[i]==0x01)||(rx3_buffer[i]==0x11)||(rx3_buffer[i]==0x31)||(rx3_buffer[i]==1))
			flag=1;
		if(flag)
		{	
			ans=1;
			uint8_t newMsg[2]={0};
			newMsg[0]=0x08;
			newMsg[1]=0x01;
			HAL_UART_Transmit(&huart2,newMsg,2,0x00ff);
			HAL_UART_Transmit(&huart2,newMsg,2,0x00ff);
			HAL_UART_Transmit(&huart2,newMsg,2,0x00ff);
			HAL_UART_Transmit(&huart2,newMsg,2,0x00ff);
			HAL_UART_Transmit(&huart2,newMsg,2,0x00ff);
			rx3_buffer[0]=0;
			break;
		}
		else {
			ans=0;
			rx3_buffer[0]=0;
			break;
		}
	}
	
}

void DMA_Usart3_Send(uint8_t *buf,uint8_t len) //串口发送封装
{   
	if(HAL_UART_Transmit_DMA(&huart3,buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
	{
  		Error_Handler();
 	}
}

/* USER CODE END 1 */
