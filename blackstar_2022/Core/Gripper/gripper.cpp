/**
 * @file armRaise.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 预留给机械臂运动相关代码
 * @version 0.1
 * @date 2022-10-06
 * 
 * @todo rename the cpp file to gripper.cpp
 * @copyright Copyright (c) 2022
 * 
 */
#include "gripper.h"
#include "counter.h"

/*Object defines ---------------------------------------------*/

/*发送指令的等待队列,没有启用*/
commandQueue_t commandQueue;

/*空message_t的对象,没有启用*/
static const message_t errorMsg{errorCmd,0,0};

/*Private functions definations ---------------------------------------------*/

/*检测新的指令是否符合规范*/
static inline status_t 
checkMsg(message_t &newMsg)
{
	if (newMsg.command != errorCmd && newMsg.command != armRaiseCmd && newMsg.command != detectCodeCmd)
	{
		return 0;
	}
	switch (newMsg.command)
	{
	case errorCmd:
		return 1;
	case armRaiseCmd:
		if (newMsg.argList[0] >= 2 || newMsg.argList[1] > 10)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	case detectCodeCmd:
		return 1;
	default:
		return 0;
	}
}

/*发送原始uint8_t型的指令，跳过了message_t的封装过程,实际使用的是该函数发送指令，因为比较稳定*/
HAL_StatusTypeDef 
sendCommadbyRaw(command_t newCmd, uint8_t arg1=0, uint8_t arg2=0)
{
	/*backup plan for communication with respberry: send raw uint8_t*/
	uint8_t newMsg[3] = {(uint8_t)newCmd, arg1, arg2};
	HAL_StatusTypeDef flag = HAL_ERROR;
	static const uint8_t attemptTime = 5;
	static const uint32_t retryInterval = 100;

	/*如果uart没有返回成功标志，则多次尝试发送*/
	for (uint8_t i = 0; i < attemptTime; i++)
	{
		flag = HAL_UART_Transmit(&huart2, newMsg, 3, retryInterval);
		if (flag == HAL_OK)
		{
			break;
		}
		else
		{
			HAL_Delay(retryInterval);
		}
	}
	return flag;
}

/*messageQueue_t construct & destruct functions*/
messageQueue_t::messageQueue_t()
{
	clearAll();
}

messageQueue_t::~messageQueue_t()
{
	clearAll();
}

inline uint8_t messageQueue_t::nextPos(uint8_t ptr)const{
	if(ptr<queueSizeMax){
		return ptr+1;
	}else{
		return 0;
	}
}

/*messageQueue_t public functions*/

status_t messageQueue_t::push(message_t& newMsg)
{
	if(isFull()==0){
		stack[tail]=newMsg;
		tail=nextPos(tail);
		return 1;
	}else{
		return 0;
	}

}

message_t messageQueue_t::pop(void)
{
	if(isEmpty()==1){
		return errorMsg;
	}
	message_t newMsg=stack[head];
	head=nextPos(head);
	if(isEmpty()==1){
		clearAll();
	}
	return newMsg;
}

status_t messageQueue_t::isEmpty(void) const
{
	return (head==tail);
}

status_t messageQueue_t::isFull(void) const
{
	return (head==nextPos(tail));
}

void messageQueue_t::clearAll(void)
{
	for(uint8_t i=0;i<queueSizeMax;i++){
		stack[i]=errorMsg;
	}
	head=0;
	tail=0;

}

/*commandQueue_t construct & destruct functions*/

commandQueue_t::commandQueue_t() : intervalTime(sendCommandInterval)
{
	clearAll();
	gripperCounter.setup(sendCommandInterval);
}

commandQueue_t::~commandQueue_t()
{
	clearAll();
}

/*commandQueue_t public functions*/

status_t commandQueue_t::newCommand(message_t newMsg)
{
	if (checkMsg(newMsg) == 0)
	{
		//return 0;
	}
	cmdQueue.push(newMsg);
	return 1;
}

status_t commandQueue_t::sendCommand(void)
{
	if (cmdQueue.isEmpty() == 1)
	{
		return 0;
	}

	message_t newMsg = cmdQueue.pop();

	switch (newMsg.command)
	{
	case armRaiseCmd:
		sendCommadbyRaw(armRaiseCmd, newMsg.argList[0], newMsg.argList[1]);
		break;
	case detectCodeCmd:
		sendCommadbyRaw(detectCodeCmd, 0);
		break;
	default:
		return 0;
	}
	return 1;
}

void commandQueue_t::clearAll(void)
{
	cmdQueue.clearAll();
}

status_t commandQueue_t::check(void){
	if(gripperCounter.ifTimeout()==1){
		sendCommand();
		gripperCounter.setup(sendCommandInterval);
		return 1;
	}else{
		return 0;
	}
}




