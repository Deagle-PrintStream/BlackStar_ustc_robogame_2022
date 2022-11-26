
#include "common.h"

#ifndef __GYROSCOPE_H
#define __GYROSCOPE_H

/*将接收到的uint8_t串计算并转换为float型角度*/
extern void CopeSerial2Data(unsigned char ucData);

/*取得当前绝对角度值*/
extern float getAngle(void);

/*开启uart1通信串口*/
extern void enableGyroscope(void);

/*关闭uart1通信串口*/
extern void disableGyroscope(void);

#endif
