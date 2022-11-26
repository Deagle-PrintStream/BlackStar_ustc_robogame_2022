
#ifndef __CONNECTIVITY_H
#define __CONNECTIVITY_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions prototypes ---------------------------------------------*/

/*检测是否收到了正确冰壶的反馈，如果是，则将全局变量flagDetectCode置为1*/
status_t curlingDetect(uint8_t * buff,uint8_t buffLength);

/*检测冰壶条形码指令*/
void detectCurlingCmd(void);
/*机械臂上升或者下降指令*/
void raiseArmCmd(status_t newDir, uint8_t distance);

/* Private defines -----------------------------------------------------------*/

#define __CONNECTIVITY

#ifdef __cplusplus
}
#endif

#endif
