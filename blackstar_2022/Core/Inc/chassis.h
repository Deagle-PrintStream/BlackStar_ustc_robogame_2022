
#ifndef __CHASSIS_H
#define __CHASSIS_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported constants ------------------------------------------------------------*/
static const uint8_t speedMaxDefault = 200;
static const uint8_t armHeightDefault = 10;

/* Exported functions prototypes ---------------------------------------------*/

/* Class defines -----------------------------------------------------------*/

class chassis_t
{
private:
    /*速度上限，由于pid调参的局限性，行进速度不能过快*/
    const uint8_t speedMax;
    /*机械臂最高高度*/
    const uint8_t armHeightMax;
    /*当前预设移动速度*/
    uint8_t moveSpeed;
    /*当前预设旋转角速度*/
    uint8_t rotateSpeed;
    /*当前预估的机械臂高度*/
    uint8_t armHeight;

    /**
     * @brief 行进过程中微调方向         * 
     * @param newDir 微调的方向，左或者右
     * @param deflectVal 当前偏移程度，最大为0.4
     */
    void trim(direction_t newDir, float deflectVal = deflectValDefault);

public:
    /*当前行进方向*/
    uint8_t headingDir;
    /*是否在巡线上,为1则使能巡线模块并返回trim指令*/
    status_t onTrail;
    /*默认构造函数和解构函数*/
    chassis_t();
    ~chassis_t();
    /* @breif 向指定方向直线前进，如果在线上则开启巡线模块计算
        @param newDir: 移动方向
        @param targetSpeed: 目标移动速度，会逐渐加速到该速度
        @param timeout: 最大移动时间，非0则在该时间内将主进程挂起
    */
    void move(direction_t newDir = dirFront, uint8_t targetSpeed = speedHigh, uint32_t timeout = 0);
    /**
     * @brief 原地旋转，并初始化pid参数
     * @param newDir 旋转方向，左或者右
     * @param targetSpeed 旋转速度
     * @param timeout: 最大旋转时间，非0则在该时间内将主进程挂起
     */
    void rotate(direction_t newDir, uint8_t targetSpeed = rotateSpeedDefault, uint32_t timeout = 0);
    /**
     * @brief 定角度旋转
     * 
     * @param newDir 旋转方向，左或者右
     * @param angle 旋转角度
     */
    void rotatebyDegree(direction_t newDir,float angle);
    /*@brief 停止运动（移动或者旋转）并暂停巡线模块计算
        *@param delayTime: 停止运动后的挂起时间
        */
    void stop(uint32_t delayTime = timeoutDefault);
    /**
     * @brief 将机械臂抬升到指定的高度           
     * @note 在抬起过程中不能再次发送该指令，否则会冲突
     * @param newHeight 机械臂的目标高度
     * @param delayTime 发送指令后主程序的挂起时间
     */
    void setGripperHeight(uint8_t newHeight,uint32_t delayTime=armRaiseDelayTime);
    /**
     * @brief 检测条形码或者冰壶颜色
     * 
     * 进行若干次检测，若有一次正确则视为正确冰壶
     * @param attemptTimes 检测的次数，
     * @return status_t 是否是正确冰壶
     */
    status_t detectCode(uint8_t attemptTimes = 1) const;
    /**
     * @brief 利用气瓶高压推出冰壶
     * @return None
     */
    void pushCurling()const;

    /**
     * @brief 前进过程中调整方位，被callback调用
     * @note 如果不在线上，或者停止，或者旋转状态则直接退出；
     * @see calcTrimDIntensity()
     * @return None
     */
    void adjustDirection(void);
    /*关闭或者开启所有巡线模块的计算*/
    void togglePauseTracer(status_t newStatus)const;

    void publicTrim(direction_t newDir, float deflectVal = deflectValDefault);

};

/* Exported macro ------------------------------------------------------------*/

extern chassis_t chassis;

/* Private defines -----------------------------------------------------------*/

#define __CHASSIS

#ifdef __cplusplus
}
#endif

#endif /*__CHASSIS_H*/
