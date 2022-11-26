
#ifndef __TRACER_H
#define __TRACER_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

namespace tracer_nsp{
	enum logic_t{
		single=0,
		both=1,
		either=2,
		respectively=3,
		reserved=4,	
	};
}

/* Exported constants ------------------------------------------------------------*/

static const uint8_t sensorCount=5;/*每一个巡线模块有5个传感器*/

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief 判断指定方向上的巡线模块是否为指定的巡线偏移类型
 * 
 * @details  intended for WAIT_FOR param of __exp, replacing "onpath==1"-like expressions  
 * @todo 将该函数取消extern，仅保留waitforTracerdetect的extern即可
 * 
 * @param newDir 指定方向的巡线模块
 * @param newStatus 巡线偏移类型
 * @return status_t 是或否
 */
status_t tracerDetect(uint8_t newDir,tracer_nsp::traceStatus_t newStatus=tracer_nsp::onTrace);

/**
 * @brief 等待传感器返回某个寻径状态，期间将主程序挂起
 * 
 * @details 在path.cpp中由各个阶段的寻径函数调用 @n 
 * 	输入一个运动状态，例如move(dirRight,speedHigh);后 @n 
 * 	等待某个传感器撞线或者离线，此时使用： @n 
 * 	waitforTracerDetect(dirFront,exactTrace,3000); @n 
 * 用以在前方传感器完美在线上时停止挂起状态，然后停止 @n 
 * stop();
 * 
 * @example waitforTracerDetect(dirRight,lostTrace,timeout_nsp::upperTurningTime);
 * @todo 替换common.h中的WAIT_FOR()的宏定义
 * @todo 检测coutner_t是否能正常工作
 * 
 * @param newDir 指定方位上的传感器
 * @param newTraceStatus 指定的寻径状态
 * @param timeout 最大挂起时间，超时后无条件退出函数
 * @return 1=传感器返回了正确值，0=超时退出
 */
status_t waitforTracerDetect(uint8_t newDir,tracer_nsp::traceStatus_t newTraceStatus,const uint32_t timeout=timeoutMax);

/**
 * @brief 等待指定的两个传感器均返回某个寻径状态，期间将主程序挂起
 * 
 * @note 由于不支持函数重载，故单命名一个函数
 * @param logic 两个传感器的耦合逻辑
 * @param tracerDir1 第一个传感器
 * @param tracerDir2 第二个传感器
 * @param newTraceStatus 寻径状态
 * @param timeout 最大挂起时间
 * @return 1=传感器返回了正确值，0=超时退出
 */
status_t waitforDuelTracerDetect\
(tracer_nsp::logic_t logic,uint8_t tracerDir1,uint8_t tracerDir2,tracer_nsp::traceStatus_t newTraceStatus,const uint32_t timeout=timeoutMax);

/* Private defines -----------------------------------------------------------*/

#define __TRACER

/* Class defines -----------------------------------------------------------*/

using namespace tracer_nsp;

class tracer_t{
private:
	/*5个传感器对象*/
	sensor_t sensor[sensorCount];

	/*是否开启传感器检测*/
	status_t detectOn;
	/*偏移角度*/
	float deflectValue;
	/*uint8_t型变量用于表示5个传感器的返回二值化结果，最高位为左2*/
	uint8_t sensorValArray;
	/*表示当前巡线模块的偏移类型，包括 exactTrace,onTrace, lostTrace*/
	status_t traceStatus;

	/*更新传感器数值*/
	void updateSensorVal(void);
	/*更新传感器数值数组*/
	void updateValueArray(void);
	/*计算大致偏移角度*/
	void calcDeflection(void);
	/*更新tracer的路径检测数值*/
	void updatePathStatus(void);

	/*取得指定传感器的二值结果*/
	status_t sensorVal(uint8_t order)const;

	/*重置所有传感器历史数值*/
	void clearSensorVal(void);
	/*重置tracer统计的路线状况*/
	void clearStatus(void);

public:
	/*该tracer是否在线上,下一步弃用TODO*/
	status_t onPath;
	/*该tracer是否完美的在线上*/
	status_t exactOnPath;

	tracer_t();
	~tracer_t();

	/*更新tracer所有数值*/
	void updateData(void);	
	/*开启或关闭检测*/
	void togglePause(status_t newStatus);
	/*取得偏转值*/
	float getDeflectVal(void)const;
	/*取得当前的路径情况*/
	status_t getTraceStatus(void)const;

	/*调试用函数，输出相关变量*/
	__DEBUG void printSensorVal(void)const;
};

/* Exported macro ------------------------------------------------------------*/

extern tracer_t tracer[];

/* Private defines -----------------------------------------------------------*/

#define FATAL_ERROR (1)	/*致命错误，目前表示在取壶区脱线*/

#define __TRACER

#ifdef __cplusplus
}
#endif

#endif	/*__TRACER_H*/
