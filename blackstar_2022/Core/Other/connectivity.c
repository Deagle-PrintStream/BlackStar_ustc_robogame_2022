/**
 * @file connectivity.c
 * @author team_BlackStar
 * @brief 通信相关函数
 * @version 0.2
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 * @warning message_t的封装目前有点小问题，目前使用裸发送uint8_t型字符串的函数
 */
#include "connectivity.h"
#include "counter.h"
#include "gripper.h"

/*Exported functions definations ---------------------------------------------*/

void detectCurlingCmd(void)
{
#if (CMD_QUEUE==0)
  sendCommadbyRaw(detectCodeCmd,detectCodeCmd,detectCodeCmd); /*临时方案，新的模块无法工作,会导致大量时间浪费*/

#else
	message_t newMsg{detectCodeCmd, 0};
	commandQueue.newCommand(newMsg);
#endif
}

void raiseArmCmd(status_t newDir, uint8_t distance)
{
#if (CMD_QUEUE==0)
  sendCommadbyRaw(armRaiseCmd,newDir,distance);
  HAL_Delay(1000);  /*临时方案，新的模块无法工作,会导致大量时间浪费*/

#else
	message_t newMsg{armRaiseCmd, newDir, distance};
	commandQueue.newCommand(newMsg);
#endif
}

status_t curlingDetect(uint8_t * buff,uint8_t buffLength){
  flagDetectCode=1;
  if(buff[0]==detectCodeAns){
    flagDetectCode=1;
    return 1;
  }else{
    return 0;
  }
}
