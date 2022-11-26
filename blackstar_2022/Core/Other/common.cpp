/**
 * @file common.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 一些基本的辅助函数
 * 
 * 方向的转换，微妙级别的延时，脱线后的尝试矫正函数入口
 * 
 * @version 1.4
 * @date 2022-10-06
 * @see https://github.com/Deagle-PrintStream/connectivity_merge/tree/project_struct_rebuild
 * @copyright Copyright (c) 2022
 * 
 */
#include "common.h"
#include "tracer.h"
#include "tracerSelector.h"
#include "path.h"
#include "motor.h"
#include "pid.h"
#include "chassis.h"

/* Exported macro ------------------------------------------------------------*/

status_t flagInitReady=0;
status_t flagDetectCode=0;
status_t flagGyroscope=0;
float baseAngle=0;
uint8_t lastTrim=dirNowhere;
float trimAngle=0;

/* Private functions definations ---------------------------------------------*/

/* Exported functions definations ---------------------------------------------*/

direction_t oppositeDir(uint8_t newDir){
    uint8_t opDir=dirNowhere;
    switch ((direction_t)newDir)
    {
    case dirFront:
        opDir=dirBack;
        break;
    case dirRight:
        opDir=dirLeft;
        break;
    case dirLeft:
        opDir=dirRight;
        break;
    case dirBack:
        opDir=dirFront;
        break;
    default:
        opDir=dirNowhere;
        break;
    }
    return (direction_t)opDir;
}

direction_t getRightDir(direction_t newDir){
	uint8_t rightDir=dirNowhere;
    switch (newDir)
    {
    case dirFront:
        rightDir=dirRight;
        break;
    case dirRight:
        rightDir=dirBack;
        break;
    case dirLeft:
        rightDir=dirFront;
        break;
    case dirBack:
        rightDir=dirLeft;
        break;
    default:
        rightDir=dirNowhere;
        break;
    }
    return (direction_t)rightDir;
}

direction_t getLeftDir(direction_t newDir){
  return getRightDir(oppositeDir(newDir));
}

void Delay_us(uint16_t us)
{
  const uint16_t Delay_Factor_us = HAL_RCC_GetHCLKFreq() / 1000 / 1000; // ought to be defined in .h file
  uint32_t tmp;
  SysTick->LOAD = us * Delay_Factor_us;
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do
  {
    tmp = SysTick->CTRL;
  } while (tmp & 0x01 && !(tmp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}

void lostTrace_Handler(void){
    direction_t trimDir=(trimAngle>0)?(dirLeft):(dirRight);
    chassis.publicTrim(trimDir,0.4);
    HAL_Delay(10);
}
