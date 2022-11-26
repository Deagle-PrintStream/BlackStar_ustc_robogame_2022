
#ifndef __TRACER_SELECTOR_H
#define __TRACER_SELECTOR_H

/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/* Exported types ------------------------------------------------------------*/

/* Class defines -----------------------------------------------------------*/

class selector_t{
private:
  /*当前选择器返回值对应的的tracer*/
  uint8_t currDir;

  /*根据当前方向设置gpio输出*/
  void updateOutput(void)const;
public:
  selector_t();
  ~selector_t();

  /*手动设置当前选择器对应返回值*/
  void setCurrDir(direction_t newDir=dirFront);
  /*周期性更新返回值,时钟周期中断里直接调用该函数即可*/
  direction_t update(void);

};

/* Exported macro ------------------------------------------------------------*/

/*选择器74hc153的类对象*/
extern selector_t tracerSelector;

#define __TRACER_SELECTOR

#ifdef __cplusplus
}
#endif

#endif	/*__TRACER_SELECTOR_H*/
