
#ifndef __INITIALIZE_H
#define __INITIALIZE_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/


/* Exported constants ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/*初始化PID相关的参数*/
status_t motorInit(void);
/*初始化包括tracer,selector,patrol等类对象*/
status_t tracerInit(void);
/*backup_plan2对应的引脚高压决定冰壶位置*/
status_t curlingNodeInit(void);
/*解构包括tracer,selector,patrol等类对象*/
status_t tracerDestrcut(void);

/* Private defines -----------------------------------------------------------*/

#define __INITIALIZE

#ifdef __cplusplus
}
#endif

#endif	/*__INITIALIZE_H*/
