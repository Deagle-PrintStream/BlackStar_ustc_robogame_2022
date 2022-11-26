/**
 * @file gyroscope.cpp
 * @author @long-knight
 * @brief 陀螺仪相关计算模块
 * @todo 实现类的封装 
 * @version 0.8
 * @date 2022-11-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gyroscope.h"

extern UART_HandleTypeDef huart1;

/*一次接收数据的uint8_t个数*/
static int ZAngleDateCount=0;
/*当前旋转角度的绝对值,-180 ~ +180 */
static float ZAngleDate=0;

void enableGyroscope(void){
	//启动uart1的空闲接收 
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn,3,2);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	HAL_Delay(10);
}

void disableGyroscope(void){
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	__HAL_UART_DISABLE_IT(&huart1,UART_IT_RXNE);
	__HAL_UART_DISABLE_IT(&huart1,UART_IT_IDLE);
	HAL_Delay(10);

}

void CopeSerial2Data(uint8_t ucData)
{
	static uint8_t ucRxCnt=0;
	static uint8_t ucRxBuffer[12]={0};
	ucRxBuffer[ucRxCnt]=ucData;	//将收到的数据存入缓冲区中
	ucRxCnt++;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {//数据不满11个，则返回
    return;
	}
	else
	{
		if(ucRxBuffer[1]==0x53)
		{
			ZAngleDate=((short)((short)ucRxBuffer[7]*256+(short)ucRxBuffer[6]))*180.0/32768.0;//获得z轴角速度,并计入数组
			if(ZAngleDateCount==11){
				ZAngleDateCount=0;
			}
		}
		ucRxCnt=0;//清空缓存区
	}
}

float getAngle(void){
	return ZAngleDate;
}
