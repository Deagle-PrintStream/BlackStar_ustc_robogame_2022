/**
 * @file callback.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 回调函数
 * 
 * 计时器中断处理函数，串口接收的处理函数
 * 
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "callback.h"

#include "connectivity.h"
#include "counter.h"
#include "chassis.h"
#include "motor.h"
#include "tracer.h"
#include "tracerSelector.h"
#include "path.h"
#include "gripper.h"
#include "gyroscope.h"

/* Private functions defines ---------------------------------------------*/

/*更新tracer的数据*/
static inline void updateTracer(void)
{
  direction_t newDir = tracerSelector.update();
  tracer[newDir].updateData();
}

static inline void gyroscppeHandler(uint8_t *buff, uint8_t buffSize){
  uint8_t *ptr=buff;
  static const uint8_t top=99;
  uint8_t count=0;
  while(count<=top){
    CopeSerial2Data(*ptr);
    ptr++;
    count++;
  }
}

/* Exported functions definations ---------------------------------------------*/

void tim6_callback(void)
{
  /*进入中断间隔为1ms*/

  if(flagInitReady==0)
    return;
  updateTracer();
  counterList.update();
#if (CMD_QUEUE==1)
  commandQueue.check();
#endif

}

void tim6_50period_callback(void)
{
  /*进入中断间隔为50ms*/
  if(flagInitReady==0)
    return;

/*如果在静态调试阶段则直接退出，不用调整PWM*/
#if (STATIC_DEBUG==1)
  return;
#else
  updatePWM();
  chassis.adjustDirection();
  path.detectFatalError();  
#endif
}

void USART1_IdleCallback(uint8_t *buff, uint8_t buffSize)
{
  uint8_t buffLength = MIN(argCountMax, buffSize - 1);
  gyroscppeHandler(buff,buffLength);
}

void USART2_IdleCallback(uint8_t *buff, uint8_t buffSize)
{
  uint8_t buffLength = MIN(argCountMax, buffSize - 1);
  curlingDetect(buff,buffLength);
}
