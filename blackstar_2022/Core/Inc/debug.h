
#ifndef __DEBUG_H
#define __DEBUG_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/

/********************************************************************************
 * @brief 调试不同的模块对应的宏定义
 * 通过改变宏定义来决定编译的代码块
 * 
 ********************************************************************************
 */


#define RED_OR_YELLOW (0) /*发射黄壶1;还是红壶0*/

#if(RED_OR_YELLOW==0)
	#define LAUNCH_ALL_LEFT (0)	/*左右各发射两个冰壶 */
#else
	#define LAUNCH_ALL_LEFT (1)	/*全部在左侧发射冰壶*/
#endif

#define __DEBUG /*调试用的函数或者变量标记，最终工程需要删除该define todo: 以找出所有调试函数*/

#ifdef __DEBUG
	#define MODULE_TEST (0)/*单项检测，不跑全程，进入主函数中的while(1)循环*/

	#define STATIC_DEBUG (0) /*让小车在静止时进行调试*/

	#define BACKUP_PLAN (0) /*使用备用方案*/

	#define WHOLE_PROCEDURE (1)/*测试全程，从起点启动，用于后半程的测试*/

	#if(BACKUP_PLAN==0)

		#define VISION_MODULE (1)	/*开启视觉模块*/

		#define NEW_GRIPPER (1)	/*使用全新的机械爪，可以在下降后直接发射*/

		#define CMD_QUEUE (0)	/*启用新的通信指令队列模块(目前存在问题)*/

	#elif (BACKUP_PLAN==1)

			#define EASY_CURLING_NODE_1 (2)
			#define HARD_CURLING_NODE_1 (3)
			#define HARD_CURLING_NODE_2 (5)

	#elif(BACKUP_PLAN==2)

			extern uint8_t bp_easyCurlingNode;
			extern uint8_t bp_hardCurlingNode_1;
			extern uint8_t bp_hardCurlingNode_2;

	#endif

#endif


/* Exported types ------------------------------------------------------------*/

#if(BACKUP_PLAN == 1)
/*基准线上的节点名称*/
enum baselineNode_t
{
	lowerRightTurning = 0,
	easyCurlingRight_1 = EASY_CURLING_NODE_1,
	easyCurlingRight_2 = 2,
	launchRight = 3,
	hardCurling_1 = HARD_CURLING_NODE_1,
	easyCurlingMide = 4,
	hardCurling_2 = HARD_CURLING_NODE_2,
	launchLeft = 5,
	hardCurling_3 = 5,
	deadend = 6,
};
#elif(BACKUP_PLAN == 2)
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

/* Exported constants ------------------------------------------------------------*/

/*调试用的信息最大长度*/
static const uint8_t msgSizeDefault = 10;	
/* @brief 通信上面的默认超时毫秒常量*/
static const uint32_t timeoutDefault = 0x00ff;

/* Exported macro ------------------------------------------------------------*/

/*出现了致命问题，用于在基准线附近移动时脱线的标志*/
extern uint8_t flag_fatalError;

/*用于和电脑通信，单向传输调试信息*/
extern UART_HandleTypeDef huart3;

/* Exported functions prototypes ---------------------------------------------*/

/*	@brief 自定义的调试输出函数，不能像printf一样输出更多类型的数值
    @param newMsg: uint8型的字符信息
*/
HAL_StatusTypeDef printMsg(uint8_t *newMsg, uint8_t msgSize = msgSizeDefault, UART_HandleTypeDef printUart = huart3, uint32_t timeout = timeoutDefault);
/*	@breif 直接输出一段文字方便调试,似乎不支持函数重载
    @param newMsg: 标准字符串型
    @example  printText("hello"）;
*/

/*提供给main.c的调试接口函数*/
void testModule(void);


#ifdef __cplusplus
}
#endif

#endif	/*__DEBUG_H*/
