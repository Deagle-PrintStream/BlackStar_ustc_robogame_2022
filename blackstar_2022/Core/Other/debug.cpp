/**
 * @file debug.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 调试阶段的单项测试函数
 * @version 0.5
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * @warning 需要在debug.h中修改对应的宏定义，才可实现静止状态下的调试，否则可能车子会移动
 */
#include "common.h"
#include "path.h"
#include "chassis.h"
#include "tracer.h"
#include "gripper.h"
#include "gyroscope.h"
#include "counter.h"

#include <stdlib.h>

/* Exported macro ------------------------------------------------------------*/

uint8_t flag_fatalError=0;

#ifdef __DEBUG

/*备用方案，在视觉无法实现时启用*/
#if (BACKUP_PLAN==1)


#elif(BACKUP_PLAN==2)

uint8_t bp_easyCurlingNode=1;
uint8_t bp_hardCurlingNode_1=3;
uint8_t bp_hardCurlingNode_2=4;

#endif

/* Private functions prototypes ---------------------------------------------*/

/*测试四个巡线模块是否正常工作*/
void testSensors(void);
/*测试节点识别算法能否工作*/
void testPatrol(void);
/*测试机械臂能否正常工作*/
void testDetectCurling(void);
/*检测基本运动方式是否正常工作*/
void testMovement(void);
/*检测节点之间的运动方式是否正常工作*/
void testForkMovement(void);
/*测试偏转发射*/
void testLaunch(void);
/*测试新的机械臂高度函数*/
void testGripperHeight(void);
/*测试新的巡线模块检测等待函数*/
void testWaitforTracerDetect(void);
/*测试分别检测到的逻辑实现*/
void testRespectiveDetect(void);
/*测试陀螺仪*/
void testGyroscope(void);
/* Exported functions defines ---------------------------------------------*/

void testModule(void){
  HAL_Delay(timeout_nsp::startLineWaitingTime);
  
  //testSensors();
  //testMovement();
  //testForkMovement();
  //testLaunch();  
  //testWaitforTracerDetect();
  testGripperHeight();
  //testGyroscope();
}

__DEBUG HAL_StatusTypeDef \
printMsg(uint8_t *newMsg,uint8_t msgSize,UART_HandleTypeDef printUart,uint32_t timeout){
  if(sizeof(newMsg)<msgSize)
      msgSize=sizeof(newMsg)+1;
  return HAL_UART_Transmit(&printUart,newMsg,msgSize,timeout);
}

/* Private functions defines ---------------------------------------------*/

void testSensors(void){
  while(1){
    static uint8_t testDir=dirFront;
    uint8_t tempMsg[]={(uint8_t)(testDir+48),':'};
    printMsg(tempMsg,2);
    tracer[testDir].printSensorVal();
    //tracer[testDir].printPathStatus();
    testDir=getRightDir((direction_t)testDir);
    HAL_Delay(testPrintInterval);
  }
}

void testDetectCurling(void){
    if(chassis.detectCode(1)==1){
      chassis.setGripperHeight(holdEasyCurlingHeight);
      HAL_Delay(2000);
      chassis.setGripperHeight(launchCurlingHeight);
    }else{
      //do nothing
    }
    HAL_Delay(testPrintInterval);
}

void testMovement(void){

  chassis.onTrail=0;
  chassis.stop();
  chassis.rotate(dirRight,rotateSpeedDefault,12000);
  chassis.stop();
}

void testForkMovement(void){
  uint8_t newNode=0;

  path.testBaselineInit();
  while(1){
    newNode=std::rand()% baselineNodeCount;
    path.gotoBaselineNode(newNode);
    HAL_Delay(1000);
    if(newNode<baselineNodeCount/2){
      path.turnBack(dirRight);
      HAL_Delay(1000);
    }
  }
}

void testLaunch(void){

  path.testBaselineInit();
  while(1){
    path.gotoBaselineNode(launchRight);
    path.goLaunchCurling(launchRight,TRIM_LAUNCH,0);
    path.gotoBaselineNode(launchLeft);
    path.turnBack(dirRight);
  }

}

void testGripperHeight(void){
  chassis.stop();
  chassis.setGripperHeight(5,3000);
  chassis.setGripperHeight(0,3000);
    
}

void testWaitforTracerDetect(void){
  path.testBaselineInit();
  chassis.move(dirRight);
  waitforTracerDetect(dirRight,lostTrace,5000);
  chassis.stop();

}

void testRespectiveDetect(void){
  path.testBaselineInit();
  chassis.move(dirLeft);
  waitforDuelTracerDetect(tracer_nsp::respectively,dirFront,dirBack,onTrace,10000);
  chassis.stop();
}

void testGyroscope(void){
  static const uint32_t rotateTimeout=5000;
  static float targetAngle=10.0;

  chassis.onTrail=0;
  chassis.stop();
  enableGyroscope();

  float oldAngle=getAngle();
  float newAngle=oldAngle;

  chassis.rotate(dirRight,rotateSpeedDefault/4);  
  tracerCounter.setup(rotateTimeout);

  while(tracerCounter.ifTimeout()==0){
    newAngle=getAngle();
    if(ABS(newAngle-oldAngle)>targetAngle){
      break;
    }
    HAL_Delay(10);
  }
  
  disableGyroscope();
  chassis.stop();

}

#endif
