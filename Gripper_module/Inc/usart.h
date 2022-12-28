/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "string.h"
#define BUFFER_SIZE  100  


typedef enum{
  errorCmd=0x00,  //预设的意外错误指令，或者通信过程中的噪声
  moveCmd=0x01,  //加速或减速的指令，预设一个速度
  rotateCmd=0x02, //原地旋转指令，默认此时是静止状态
  trimCmd=0x03, //前进过程中调整角度的指令，不是原地旋转
  stopCmd=0x04, //急刹指令
  pushCurlingCmd=0x05, 
  armRaiseCmd=0x06,  //步进电机运动指令
  detectCodeCmd=0x07, //条形码的检测指令
  detectCodeAns=0x08,
  reservedCmd=0x09,  //预留指令
  testCmd=0x010,  //前期测试指令，通信协议是否可以运行

}command_t;

typedef struct{
  command_t command;
  uint8_t argList[3];
}message_t;


void receiveCmd(uint8_t *buff);

typedef enum{
  sensorDirFront=0x00,
  sensorDirRight=0x01,
  sensorDirBack=0x02,
  sensorDirLeft=0x03,
}sensorDir_t;


typedef enum{
  goFront=0,
  goRight=1,
  goBack=2,
  goLeft=3,
}direction_t;

typedef uint8_t status_t;

/*平凡节点，即路程中的任意节点类型，四个传感器返回值的排列组合*/
typedef enum{
  nowhere=0,
  frontOnly=1,
  backOnly=2,
  rightOnly=3,
  leftOnly=4,
  straightLine=5,
  horizontalLine=6,
  rightTurn=7,
  leftTurn=8,
  rightFork=9,
  leftFork=10,
  T_crossing=11,
  crossing=12,
}plainNode_t;

/*关键节点，即巡线模块的定位节点*/
typedef enum{
  startLine=0,
  upperRightTurning=1,
  lowerRightTurning=2,
  baseNode=3,
  crossroadLeft=4,
  crossroadMid=5,
  crossroadRight=6,
  curlingDepositEasy=7,
  curlingDepositHard=8, 

}keyNode_t;


extern volatile uint8_t rx2_len;  //接收一帧数据的长度
extern volatile uint8_t rec2_end_flag; //一帧数据接收完成标志
extern uint8_t rx2_buffer[BUFFER_SIZE];  //接收数据缓存数组

void Usart2_Handle(uint8_t *rx2_buffer);
void DMA_Usart2_Send(uint8_t *buf,uint8_t len);//串口发送封装
void Usart2_IDLE(void);



extern volatile uint8_t rx3_len;  //接收一帧数据的长度
extern volatile uint8_t rec3_end_flag; //一帧数据接收完成标志
extern uint8_t rx3_buffer[BUFFER_SIZE];  //接收数据缓存数组

void Usart3_Handle(void);
void DMA_Usart3_Send(uint8_t *buf,uint8_t len);//串口发送封装
void Usart3_IDLE(void);


/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

