/**
 * @file common.h
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 常量定义，类型定义，wait_for宏定义
 * 
 * 数组大小的常量定义，时间常量定义（待测），
 * 方向类型定义，基准线上的节点名称enum类定义，
 * 通信的指令enum定义，通信结构体定义，
 * 全局标志类变量定义，flag_init表示是否硬件初始化完毕，
 * flag_fatalError表示是否脱线需要矫正,
 * 方向的转换函数，脱线处理函数,
 * 条件等待宏定义函数
 * 
 * @version 1.1
 * @date 2022-10-07
 * @see https://github.com/Deagle-PrintStream/connectivity_merge/tree/project_struct_rebuild
 * 
 * @note all the other head files include common.h for type defines and global constants,
 * 	common.h includes main.h for defines of GPIOs,
 * 	common.h includes debug.h for macro defines about which path plan to compile
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __COMMON_H
#define __COMMON_H
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "debug.h"

/**
 * @brief TODO list
 * 
 * 在发射阶段调整角度发射
 * 修改部分阶段的移动速度，尽可能减少无操作的等待时间
 * 
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Global defines -----------------------------------------------------------*/

#define MIN(__a, __b) \
	((__a) < (__b) ? (__a) : (__b))

#define MAX(__a, __b) \
	((__a) > (__b) ? (__a) : (__b))

#define ABS(__a) \
	((__a < 0 )? -(__a):(__a) )

#define __COMMON

	/* Exported constants ------------------------------------------------------------*/

	static const uint8_t directionCount = 4;	/*这个世界有四个方向*/
	static const uint8_t argCountMax = 3;		/*指令的参数最多有3个*/
	static const uint8_t baselineNodeCount = 6; /*基准线上的节点数，包括最左端的T字口*/
	static const float floatZeroThrehold=0.00001;/*浮点数类型为0的判断*/

	/*时间常量单位一律为毫秒ms*/

	/* @brief 在中间无巡线阶段的冲刺速度*/
	static const uint8_t superDash = 150; /*old :120*/
	/* @brief 一般行进速度*/
	static const uint8_t speedHigh = 105;	/*old :80*/
	/* @brief 在检测到节点后的减速运动；定距离移动的速度*/
	static const uint8_t speedLow = 40;
	/*@brief 旋转速度默认值，在180度旋转时的效果还不错*/
	static const uint8_t rotateSpeedDefault=120;
	/* @brief 微调时方向的偏移程度默认值*/
	static const float deflectValDefault = 0.2;

	/* @brief 等待挂起时间最大数值，对于单个模块而言即无穷*/
	static const uint32_t timeoutMax = 3*60*1000;
	/* @brief waitFor函数的周期延时常量*/
	static const uint16_t waitforDelayTime = 4;
	/* @brief 定距离移动的默认移动时间*/
	static const uint32_t moveDistanceTime = 2000;
	/* @brief 检测冰壶颜色的发送指令时间间隔*/
	static const uint32_t detectInterval = 20;
	/* @brief 发射冰壶时，继电器的给高压时间*/
	static const uint32_t pushCurlingTime = 600;
	/*一个很长的时间，调试阶段没有确定timeout时使用*/
	static const uint32_t quiteLongTime = 15000;
	/*等待树莓派摄像头数据稳定的时间*/
	static const uint32_t respberryTime_1 = 2000;
	static const uint32_t respberryTime_2 = 4000;
	/*发射冰壶时的延时时间*/
	static const uint32_t pushCurlingWaitTime = 1000;
	/*调试时测试信息的发送间隔*/
	static const uint32_t testPrintInterval=2000;
	/*机械臂运动指令发送后的延迟时间默认值*/
	static const uint32_t armRaiseDelayTime=500;
	/*发送指令的最小间隔*/
	static const uint32_t sendCommandInterval=3000;
	/*和寻径相关的时间常量*/
	namespace timeout_nsp
	{
		static const uint32_t startLineWaitingTime = 2000; /*在起点等待时间，调试时使用*/
		static const uint32_t leavingStartLineTime = 2000; /*离开起点方框所需时间*/
		static const uint32_t upperTurningTime = 13000;	   /*上端纵向巡线所需时间最大值*/
		static const uint32_t guideLineTime = 1600;		   /*到达上端直角弯后，向下移动的巡线指导时间*/
		static const uint32_t wasteLandTime = 14000;	   /*无巡线行进时间*/
		static const uint32_t rotateTime_180degree = 5500; /*原地旋转180度的最大时间*/
		static const uint32_t decelerateTime = 1800;	   /*检测到节点后的减速时间*/
		static const uint32_t curlingIntervalTime = 3000;  /*两个岔口间移动所需最大时间*/
		static const uint32_t passingForkTime = 1000;	   /*通过一个岔口所需最大时间*/
		static const uint32_t easyCurlingTakeTime = 1000;  /*取简单冰壶的前进时间*/
		static const uint32_t hardCurlingTakeTime = 1200;  /*取困难冰壶的前进时间TODO*/
		static const uint32_t adjustCrossingTime=800;	/*在节点调整的时间*/
	}

	/* Exported types ------------------------------------------------------------*/
	/*状态类型*/

	/*方向类型*/
	enum direction_t
	{
		dirFront = 0,
		dirRight = 1,
		dirLeft = 2,
		dirBack = 3,
		dirNowhere = 4,
		dirAll = 5,
	};

	/*状态类型，即布尔值，但是boolean是int型，太长了*/
	typedef uint8_t status_t;

	/*tracer相关的类型*/
	namespace tracer_nsp
	{
		enum on_off_t
		{
			off = 0,
			on = 1,
		};
		enum up_down_t /*由于和机械臂的通信，保留最初的设计，历史遗留*/
		{
			up = 0,
			down = 1,
		};
		enum hit_leave_t
		{
			leave = 0,
			hit = 1,
		};

		enum sensorOrder_t /*巡线模块的传感器编号，从左到右为0到4*/
		{
			L2 = 0,
			L1 = 1,
			M = 2,
			R1 = 3,
			R2 = 4,
		};
		enum traceStatus_t{/*当前巡线模块的偏移类型*/
			exactTrace=2,
			onTrace=1,
			lostTrace=0,
		};
		static const status_t blackParcel = 1; /*黑色对应1*/
		static const status_t whiteParcel = 0; /*白色对应0*/
	}

	/*指令类型，前期巡线和运动模块是分离在两个单片机上进行的，故相关指令历史遗留*/
	enum command_t
	{
		errorCmd = 0, /*发生错误，在使用*/
		moveCmd = 1,
		rotateCmd = 2,
		trimCmd = 3,
		stopCmd = 4,
		pushCurlingCmd = 5,
		armRaiseCmd = 6,   /*机械臂上升或者下降，在使用*/
		detectCodeCmd = 7, /*检测冰壶颜色，在使用*/
		detectCodeAns = 8, /*检测到的颜色的应答，在使用*/
		reservedCmd = 9,
		testCmd = 10,
	};

	/*机械臂高度的定义*/
	enum gripperHeight_t{
		noCurlingHeight=0,
		launchCurlingHeight=0,
		takeEasyCurlingHeight=0,
		holdEasyCurlingHeight=5,
		takeHardCurlingHeight=5,
		holdHardCurlingHeight=7,
	};

#if (BACKUP_PLAN == 0)
	/*基准线上的节点名称*/
	enum baselineNode_t
	{
		lowerRightTurning = 0,
		easyCurlingRight_1 = 1,
		easyCurlingRight_2 = 2,
		launchRight = 3,
		hardCurling_1 = 3,
		easyCurlingMide = 4,
		hardCurling_2 = 4,
		launchLeft = 5,
		hardCurling_3 = 5,
		deadend = 6,
	};
#endif

	/*通信结构体封装*/
	struct message_t
	{
		command_t command;
		uint8_t argList[argCountMax];
	};

	/* Exported macro ------------------------------------------------------------*/

	/*用于和respberry通信，双向*/
	extern UART_HandleTypeDef huart2;
	/*类对象和硬件是否初始化完毕，用于避免callback在未初始化之前进入中断，TODO放置到init.h中*/
	extern status_t flagInitReady;
	/*是否检测到了正确的冰壶*/
	extern status_t flagDetectCode;
	/*是否开启陀螺仪模块检测*/
	extern status_t flagGyroscope;
	/*基准角度*/
	extern float baseAngle;
	/*上一个偏转指令*/
	extern uint8_t lastTrim;
	/*累计偏转角度*/
	extern float trimAngle;
	/* Exported functions prototypes ---------------------------------------------*/

	/*取得当前方向的反方向*/
	direction_t oppositeDir(uint8_t newDir);
	/*取得当前方向的右边的方向*/
	direction_t getRightDir(direction_t newDir);
	/*取得当前方向的左边的方向*/
	direction_t getLeftDir(direction_t newDir);

	/*	@brief 延迟函数，实现微秒级别的延迟功能,未被使用
		@param us: 延时的微秒数*/
	void Delay_us(uint16_t us);
	
	/*在基准线附近脱线后的处理函数入口 */
	void lostTrace_Handler(void);

/* Private defines -----------------------------------------------------------*/

/*__USED is unable to download into the chip*/
/*__STATIC_FORCEINLINE is not so necessary*/

#ifdef __cplusplus
}
#endif

#endif /*__COMMON_H*/
