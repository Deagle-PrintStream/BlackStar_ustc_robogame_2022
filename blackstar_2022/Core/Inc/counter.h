
#ifndef __COUNTER_H
#define __COUNTER_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/


/* Exported constants ------------------------------------------------------------*/

/*不同模块需要的计数器最大个数*/
static const uint8_t counterNumberMax=3;

/* Exported functions prototypes ---------------------------------------------*/


/* Class defines -----------------------------------------------------------*/

class counter_t
{
private:
    /*是否计数到达预设值*/
    status_t flag_timeout;
    /*当前计数器是否开启*/
    status_t flag_working;
    /*预设计数值，即等待时常*/
    uint32_t prescaler;
    /*计数变量*/
    uint32_t tick;

    /*清空当前所有变量数值*/
    void clearData(void);    
    /*计数器达到阈值*/
    status_t timeUp(void);


public:
    /*默认的构造函数和解构函数*/
    counter_t();
    ~counter_t();
    /*@brief 开启计数器
      @param newTimeout 计时量，到时间后停止并通过ifTimeout()返回1
    */
    void setup(uint32_t newTimeout);
    /*手动关闭计数器，在到时间后会自动关闭计数器*/
    void turnOff(void);
    /*更新计数器，由 tim6_callback 调用 */
    void updateDate(void);
    /*是否已经超时*/
    status_t ifTimeout(void)const;
};

class counterList_t{
private:
  counter_t* listHead[counterNumberMax];
  uint8_t counterNumber;

public:
  counterList_t();
  ~counterList_t();

  /*新增计时器*/
  status_t add(counter_t * newCounterPtr);
  /*更新所有使能的计时器*/
  void update(void);
  /*清空所有计数器*/
  void clearAll(void);
};

/* Exported macro ------------------------------------------------------------*/

/* @brief 被tim6_callback()调用，用于更新所有计数器的数据*/
extern counterList_t counterList;

/* @brief 巡线等待使用的计时器*/
extern counter_t tracerCounter;

/*机械臂运动需要的计时器,未使用 unused*/
extern counter_t gripperCounter;

/*视觉模块等待返回值需要的计时器*/
extern counter_t visionCounter;


/* Private defines -----------------------------------------------------------*/

#define __COUNTER

#ifdef __cplusplus
}
#endif

#endif	/*__COUNTER_H*/
