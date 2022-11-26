/**
 * @file tracer.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 巡线模块的类方法
 *
 * 巡线模块的初始化，更新adc二值，开启或者关闭浮点计算，计算偏移角度等
 *
 * @version 1.2
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "tracer.h"
#include "counter.h"

/* Exported macro ------------------------------------------------------------*/

/*tracer_t objects for four directions*/
tracer_t tracer[directionCount];

/* Private functions definens ---------------------------------------------*/

/**
 * @brief Get the Detect Flag based on logic
 *
 * @param logic 传感器耦合逻辑
 * @param tracerDir1 第一个传感器
 * @param tracerDir2 第二个传感器
 * @param newTraceStatus 寻径状态
 * @return 1=满足当前逻辑,0=不满足
 */
inline status_t getDetectFlag(tracer_nsp::logic_t logic, uint8_t tracerDir1, uint8_t tracerDir2, tracer_nsp::traceStatus_t newTraceStatus)
{
  using namespace tracer_nsp;

  status_t flag_detected = 0;
  static status_t (*ptrTracerDetect)(uint8_t, tracer_nsp::traceStatus_t) = &tracerDetect;
  static status_t respectiveDetect1 = 0;
  static status_t respectiveDetect2 = 0;

  switch (logic)
  {
  case both: /*同时检测到*/
    flag_detected = ((*ptrTracerDetect)(tracerDir1, newTraceStatus) && (*ptrTracerDetect)(tracerDir2, newTraceStatus));
    break;
  case either: /*任意一个检测到*/
    flag_detected = ((*ptrTracerDetect)(tracerDir1, newTraceStatus) || (*ptrTracerDetect)(tracerDir2, newTraceStatus));
    break;
  case respectively: /*各自分别检测到(允许二者检测到的有时间差)*/
    if ((*ptrTracerDetect)(tracerDir1, newTraceStatus))
    {
      respectiveDetect1 = 1;
    }
    if ((*ptrTracerDetect)(tracerDir2, newTraceStatus))
    {
      respectiveDetect2 = 1;
    }
    if (respectiveDetect1 == 1 && respectiveDetect2 == 1)
    {
      flag_detected = 1;
      respectiveDetect1 = 0;
      respectiveDetect2 = 0;
    }
    else
    {
      flag_detected = 0;
    }

    break;
  default:

    break;
  }
  return flag_detected;
}

/* Exported functions definens ---------------------------------------------*/

status_t tracerDetect(uint8_t newDir, tracer_nsp::traceStatus_t newStatus)
{
  return (tracer[newDir].getTraceStatus() == newStatus);
}

status_t waitforTracerDetect\
(uint8_t newDir, tracer_nsp::traceStatus_t newTraceStatus, const uint32_t timeout){

  status_t (*ptrTracerDetect)(uint8_t, tracer_nsp::traceStatus_t)= &tracerDetect;  /*@note : keil C51对于函数指针的支持似乎存在问题*/

  tracerCounter.setup(timeout);
  while ((*ptrTracerDetect)(newDir, newTraceStatus) == 0 && tracerCounter.ifTimeout() == 0){
    if (flag_fatalError == FATAL_ERROR){/*在前进过程中脱线了*/
      lostTrace_Handler();
    }
    HAL_Delay(waitforDelayTime);
  }
  if (tracerCounter.ifTimeout() == 0){
    tracerCounter.turnOff();
    return 1;
  }else{
    return 0;
  }

}

status_t waitforDuelTracerDetect\
(tracer_nsp::logic_t logic, uint8_t tracerDir1, uint8_t tracerDir2, tracer_nsp::traceStatus_t newTraceStatus, const uint32_t timeout){
  using namespace tracer_nsp;

  tracerCounter.setup(timeout);
  while (getDetectFlag(logic, tracerDir1, tracerDir2, newTraceStatus) == 0 && tracerCounter.ifTimeout() == 0)
  {
    if (flag_fatalError == FATAL_ERROR)
    { /*在前进过程中脱线了*/
      lostTrace_Handler();
    }
    HAL_Delay(waitforDelayTime);
  }

  if (tracerCounter.ifTimeout() == 0)
  {
    tracerCounter.turnOff();
    return 1;
  }
  else
  {
    return 0;
  }
}

/*tracer_t construction &destruction func*/

tracer_t::tracer_t()
{
  clearSensorVal();
  clearStatus();
}

tracer_t::~tracer_t()
{
  clearSensorVal();
  clearStatus();
  for(uint8_t i=0;i<sensorCount;i++){
    sensor[i].~sensor_t();
  }
}

/*tracer_t private func*/

void tracer_t::updateSensorVal(void)
{
#define PORT(__order__) (SENSOR##__order__##_GPIO_Port)
#define PIN(__order__) (SENSOR##__order__##_Pin)
  using namespace tracer_nsp;
  sensor[L2].updateData(PORT(0), PIN(0));
  sensor[L1].updateData(PORT(1), PIN(1));
  sensor[M].updateData(PORT(2), PIN(2));
  sensor[R1].updateData(PORT(3), PIN(3));
  sensor[R2].updateData(PORT(4), PIN(4));

#undef PORT
#undef PIN
}

void tracer_t::updateValueArray(void)
{
  sensorValArray = 0;
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    sensorValArray += sensorVal(i);
    sensorValArray *= 2;
  }
}

void tracer_t::calcDeflection(void)
{
  /*偏移量的矫正单位，TODO需要尽可能的小，以实现平滑的调整，但是过小会导致矫正速度太慢*/
  static const float deflectUnit = 0.02;
  /*上限和下限*/
  static const float bound = 0.4;
  /*如果完美在线上则不需要处理*/
  if (exactOnPath == 1)
  {
    deflectValue = 0;
    return;
  }

  /*根据5个巡线数据的数组处理*/
  switch (sensorValArray)
  {
  case 12:                           /*01100*/
    deflectValue += 3 * deflectUnit; /*似乎是搞反了，但是这样的效果是正确的*/
    break;
  case 28: /*11100*/
    deflectValue -= 10 * deflectUnit;
    break;
  case 24: /*11000*/
    deflectValue -= 20 * deflectUnit;
    break;
  case 16: /*10000*/
    //deflectValue -= 1 * deflectUnit;
    break;
  case 6:                            /*00110*/
    deflectValue -= 3 * deflectUnit; /*似乎是搞反了，但是这样的效果是正确的*/
    break;
  case 7: /*00111*/
    deflectValue += 10 * deflectUnit;
    break;
  case 3: /*00011*/
    deflectValue += 20 * deflectUnit;
    break;
  case 1: /*00001*/
    deflectValue += 1 * deflectUnit;
    break;
  default:
    //deflectValue *= 1.04;
    break;
  }
  /*约束到上下限内*/
  deflectValue = MIN(deflectValue, bound);
  deflectValue = MAX(deflectValue, -bound);
}

void tracer_t::updatePathStatus(void)
{
  using namespace tracer_nsp;

  uint8_t tracingSensorCount = sensorVal(R1) + sensorVal(M) + sensorVal(L1);
  switch (tracingSensorCount)
  {
  case 0:
    traceStatus = lostTrace;
    break;
  case 3:
    traceStatus = exactTrace;
    break;
  default:
    traceStatus = onTrace;
    break;
  }

  /*判读是否在线上*/
  if (sensorVal(R1) + sensorVal(M) + sensorVal(L1) >= 2)
  {
    onPath = 1;
  }
  else
  {
    onPath = 0;
  }
  /*判断是否完美的在线上*/
  if (sensorVal(R1) + sensorVal(M) + sensorVal(L1) == 3)
  {
    exactOnPath = 1;
  }
  else
  {
    exactOnPath = 0;
  }
}

void tracer_t::clearSensorVal(void)
{
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    sensor[i].clearData();
  }
  sensorValArray = 0;
}

void tracer_t::clearStatus(void)
{
  /*TODO delete*/
  onPath = 0;
  exactOnPath = 0;

  deflectValue = 0;
  traceStatus = lostTrace;
}

/*tracer_t public func*/

status_t tracer_t::sensorVal(uint8_t order) const
{
  if (order < sensorCount)
  {
    return sensor[order].getValBinary();
  }
  else
  {
    return (status_t)(-1);
  }
}

void tracer_t::updateData(void)
{
  if (detectOn == on)
  {
    updateSensorVal();
    updateValueArray();
    calcDeflection();
    updatePathStatus();
  }
}

void tracer_t::togglePause(status_t newStatus)
{
  /*默认传入参数为2，即为切换当前模式*/
  if (newStatus >= 2)
  {
    return;
  }
  detectOn = newStatus;
  if (newStatus == off)
  {
    clearSensorVal();
    clearStatus();
  }
}

float tracer_t::getDeflectVal(void) const
{
  return deflectValue;
}

status_t tracer_t::getTraceStatus(void) const
{
  return traceStatus;
}

__DEBUG void tracer_t::printSensorVal(void) const
{
  static const uint8_t msgSize = sensorCount + 2;
  uint8_t newMsg[msgSize] = {0};
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    newMsg[i] = '0' + sensorVal(i);
  }
  newMsg[sensorCount] = ' '; 
  printMsg(newMsg, msgSize);
}

/* Private functions defines ---------------------------------------------*/
