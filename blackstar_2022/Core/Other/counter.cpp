/**
 * @file counter.cpp
 * @author Swayee2022(malygosa@mail.ustc.edu.cn)
 * @brief 定时器的类定义，定时器对象的列表定义
 * @version 2.1
 * @date 2022-11-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "counter.h"
/* Exported macro ------------------------------------------------------------*/

counterList_t counterList;

counter_t tracerCounter;

counter_t gripperCounter;

counter_t visionCounter;

/*counter_t construction &destruction func*/
counter_t::counter_t()
{
    flag_timeout=1;
    turnOff();
    counterList.add(this);
}

counter_t::~counter_t()
{
    turnOff();
}

/*counter_t private func*/

void counter_t::clearData(void){
    prescaler=0;
    tick=0;
}

status_t counter_t::timeUp(void){
    flag_timeout=1;
    turnOff();
    return flag_timeout;
}

/*counter_t public func*/

void counter_t::setup(uint32_t newTimeout){
    flag_timeout=0;
    flag_working=1;
    prescaler=newTimeout;
    tick=0;
}
void counter_t::turnOff(void){
    flag_working=0;
    clearData();
}

void counter_t::updateDate(void){
    if(flag_working==1){
        tick++;
        if(tick>=prescaler){
            timeUp();
        }
    }
}

status_t counter_t::ifTimeout(void)const{
    return flag_timeout;
}


/*counterList_t construction &destruction func*/

counterList_t::counterList_t()
{    
    clearAll();
}

counterList_t::~counterList_t(){
    clearAll();
    delete listHead;  
}

/*counterList_t public function defines*/

status_t counterList_t::add(counter_t * newCounter){
    if(counterNumber>=counterNumberMax){
        return 0;
    }
    listHead[counterNumber]=newCounter;
    counterNumber++;
    return 1;

}

void counterList_t::update(void){
    counter_t *ptr=listHead[0];
    for(uint8_t i=0;i<counterNumber;i++){
        ptr->updateDate();
        ptr++;
    }
}

void counterList_t::clearAll(void){
    counter_t *ptr=listHead[0];
    for(uint8_t i=0;i<counterNumber;i++){
        ptr=nullptr;
        ptr++;
    }
    counterNumber=0;
}
