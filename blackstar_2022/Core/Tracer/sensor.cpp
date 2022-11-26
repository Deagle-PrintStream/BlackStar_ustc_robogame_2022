/**
 * @file sensor.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 单个灰度传感器类方法
 * 
 * 传感器类对象的初始化，读取新的灰度数值，
 * 历史数值的更新，历史数值阈值比较，
 * 清空缓存区数值，设置置信系数（未使用）
 * 
 * @version 1.0
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "sensor.h"

using namespace tracer_nsp;

/*sensor_t construction &destruction func*/
sensor_t::sensor_t():valThrehold(valThreholdDefault){
	clearData();
}

sensor_t::~sensor_t(){
	clearData();
}

/*sensor_t private func*/

void sensor_t::getNewVal(GPIO_TypeDef *gpioPort,uint16_t pin){
  newSensorVal=(HAL_GPIO_ReadPin(gpioPort,pin)==blackParcel);
}

void sensor_t::updateCache(void){
  valCache[cachePtr]=newSensorVal;
  cachePtr++;
  if(cachePtr>=valCacheMaxDefault){
    cachePtr=0;
  }
}

void sensor_t::updateValAverage(void){
  uint8_t cacheSum=0;
  for(uint8_t i=0;i<valCacheMaxDefault;i++){
    cacheSum+=valCache[i];
  }
  valAverage=((float)cacheSum)/valCacheMaxDefault;
}

void sensor_t::updateValBinary(void){
  valBinary=(valAverage>=valThrehold);
}

void sensor_t::clearCache(void){
  for(uint8_t i=0;i<valCacheMaxDefault;i++){
    valCache[i]=0;
  }
  cachePtr=0;
}

/*sensor_t public func*/

status_t sensor_t::updateData(GPIO_TypeDef *gpioPort,uint16_t pin){
  getNewVal(gpioPort,pin);
  updateCache();
  updateValAverage();
  updateValBinary();
  return getValBinary();
}

status_t sensor_t::getValBinary(void)const{
  return valBinary;
}

void sensor_t::clearData(void){
  newSensorVal=0;
  valAverage=0;
  valBinary=0;
  clearCache();
}

__DEBUG status_t sensor_t::getNewVal(void)const{
  return newSensorVal;
}
