/**
 * @file chassis.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 地盘运动的类对象定义，集成了motor.c的代码实现
 * @version 1.0
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "chassis.h"
#include "tracer.h"
#include "motor.h"
#include "connectivity.h"

/* Exported macro ------------------------------------------------------------*/

/*global chassis object*/
chassis_t chassis;

/* Private functions defines ---------------------------------------------*/


/*计算trim指令的参数：偏转方向,数值越小，偏转程度越强*/
inline float calcTrimIntensity(float defelctFront,float defelctBack){
    /*前后的权重分配*/
    static const float weightFront=1.0;
    static const float weightBack=0.6;

    float trimIntensity=0;

    if( defelctFront+defelctBack<floatZeroThrehold && defelctFront+defelctBack>-floatZeroThrehold  ){
        trimIntensity=defelctFront * (weightFront/2) ;/*若二者之和为0，但是二者不为零，说明横向偏移了巡线*/
    }else{
        trimIntensity=defelctFront*weightFront+defelctBack*weightBack;/*一般情况偏移了巡线*/    
    }
    return trimIntensity;
}


/* Private chassis_t functions defines ---------------------------------------------*/

void chassis_t::trim(direction_t newDir, float deflectVal)
{
    if (newDir == dirRight || newDir == dirLeft)
    {
        motorTrim((direction_t)headingDir,newDir, moveSpeed, ABS(deflectVal)); /*根据上一次的move来觉得这次的微调的速度*/
    }else if(newDir==dirFront){
        motorMove((direction_t)headingDir,moveSpeed);
    }
}

void chassis_t::togglePauseTracer(status_t newStatus)const{
    using namespace tracer_nsp;
    if(newStatus==on || newStatus==off){
        for(uint8_t i=0;i<directionCount;i++){
            tracer[i].togglePause(newStatus);
        }
    }
}

/* Public functions defines ---------------------------------------------*/
chassis_t::chassis_t() : speedMax(speedMaxDefault), armHeightMax(armHeightDefault)
{
    armHeight = 0;
    headingDir=dirNowhere;
    onTrail=0;
}

chassis_t::~chassis_t()
{
    halt();
}

void chassis_t::move(direction_t newDir, uint8_t targetSpeed,uint32_t timeout)
{
    if (newDir == dirNowhere || newDir == dirAll){
        return;
    }
    headingDir = newDir;

    moveSpeed = MIN(targetSpeed, speedMax);
    motorMove(newDir, moveSpeed);
    if(timeout>0){
        HAL_Delay(timeout);
    }
}

void chassis_t::rotate(direction_t newDir, uint8_t targetSpeed,uint32_t timeout)
{
    if (headingDir != dirNowhere)
        return;
    if (newDir == dirRight || newDir == dirLeft)
    {
        rotateSpeed = MIN(targetSpeed, speedMax);
        motorRotate(newDir, rotateSpeed);
    }
    togglePauseTracer(off);
    if(timeout>0){
        HAL_Delay(timeout);
    }
    togglePauseTracer(on);
}

void chassis_t::stop(uint32_t delayTime)
{
    headingDir = dirNowhere;
    moveSpeed = 0;
    rotateSpeed = 0;

    motorStop();
    if(delayTime!=0){
        HAL_Delay(delayTime);
    }
}

void chassis_t::halt(void)
{
    stop();
    buzz();
    flagInitReady=0;
    /*TODO:添加停机相关的操作，目前看来还没啥需要的*/
}

void chassis_t::setGripperHeight(uint8_t newHeight,uint32_t delayTime){
    using namespace tracer_nsp;
    /*边界判断*/
    if(newHeight>armHeightMax){
        newHeight=armHeightMax;
    }
    /*运动量判断，由于梯形加速的参数问题，仅支持2cm，5cm的上升或者下降,TODO*/
    int8_t diff=ABS((int8_t)newHeight-(int8_t)armHeight);
    if(diff !=2 && diff != 5){
        //warning
        //do nothing and return
        return;
    }
    if(newHeight>armHeight){
        raiseArmCmd(up,newHeight-armHeight);
    }else if(newHeight<armHeight){
        raiseArmCmd(down,armHeight-newHeight);
    }else{
        //do nothing
    }
    armHeight=newHeight;

    /*机械臂的运动和车地盘的运动可以并行进行，延迟只是考虑到指令过快导致卡死*/
    HAL_Delay(delayTime);
}


status_t chassis_t::detectCode(uint8_t attemptTimes)const
{
    
    HAL_Delay(respberryTime_1);
    
#if (VISION_MODULE==1)    
    /*根据参数进行多次的条形码检测尝试*/
    for(uint8_t i=0;i<attemptTimes;i++){
        detectCurlingCmd();
        HAL_Delay(detectInterval);
    }
    /*由于摄像头图像的延迟，需要等待至少2s才能传回即使图像*/
    HAL_Delay(respberryTime_2);
    /*从tim6_callback中的全局变量flag取得是否检测到正确的冰壶*/
    if(flagDetectCode==1){
        flagDetectCode=0;
        return 1;
    }else{
        return 0;
    }
#else
    return 1;

#endif
}

void chassis_t::pushCurling()const
{
    /*将指定端口置为高压即可*/
    HAL_GPIO_WritePin(PUSH_A_GPIO_Port,PUSH_A_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PUSH_B_GPIO_Port,PUSH_B_Pin,GPIO_PIN_SET);//注意此处方向
    HAL_Delay(pushCurlingWaitTime);		
    HAL_GPIO_WritePin(PUSH_A_GPIO_Port,PUSH_A_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PUSH_B_GPIO_Port,PUSH_B_Pin,GPIO_PIN_RESET);
}

__DEBUG void chassis_t::buzz(uint8_t argReseved){

}

__DEBUG void chassis_t::adjustDirection(void){
    /*如果没有在前进或者没有在巡线上，则退出*/
    if (headingDir == dirNowhere){
        return;
    }
    if (onTrail == 0){
        move((direction_t)headingDir,moveSpeed);
        return;
    }

    /*取得前进方向的前后两个tracer的偏移数值*/
    direction_t opDir=oppositeDir(headingDir);  
    /*如果前方的tracer完美在线上则不偏移*/
    if(tracer[headingDir].exactOnPath==1 /*&&tracer[opDir].exactOnPath==1*/ ){
        move((direction_t)headingDir,moveSpeed);  
        return;
    }
    if(tracerDetect(headingDir,exactTrace)){
        move((direction_t)headingDir,moveSpeed);  
        return;
    }

    /*计算结果若为正，则向右偏转,大小表示大致的偏转角度*/
    float deflectVal=calcTrimIntensity(\
        tracer[headingDir].getDeflectVal(),tracer[opDir].getDeflectVal());
    if(deflectVal<0){
        trim(dirLeft,ABS(deflectVal));
    }else if(deflectVal>0){
        trim(dirRight,ABS(deflectVal));
    }else{
        move((direction_t)headingDir,moveSpeed);
    }
    
}
