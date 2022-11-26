
#ifndef __PATH_H
#define __PATH_H
/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "tracerSelector.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Class defines -----------------------------------------------------------*/

class path_t
{
private:
    /*当前在基准线上的节点*/
    uint8_t baselineNode;
    /*车子是否在基准线上*/
    status_t flag_Baseline;
    /*车子关于场地的车头朝向*/
    uint8_t chassisHeadingDir;
    /*在起点等待一段时间*/
    void waitInStartLine(void);
    /*从起点的正方形区域内离开*/
    void leaveStartLine(void);
    /*到达上端的右转直角弯*/
    void landonUpperTurning(void);
    /*穿越无巡线路径*/
    void gothroughWasteLand(void);
    /*抵达基准线*/
    void landonBaseline(void);

    /*在到达一个十字节点后矫正位置，使得四个tracer都在线上*/
    status_t adjustOnCrossing(void);


/*  @brief 穿越若干个岔路口,没有使用节点判断
    @param newDir: 在基准线上的移动方向
    @param forkCount: 经过的岔路口数量，为1则意味着到下一个岔路口停止
    @retval None
    @note 岔路口的定义为任意左右左右fork,turning类的平凡节点
*/
    void goPassForks(direction_t newDir,uint8_t forkCount=1);

public:

    path_t();
    ~path_t();
    /*在十字交叉口上原地旋转180度, public for debug procedure*/
    void turnBack(direction_t newDir=dirRight);

    /*从出发点出发，经过第一个右转弯，无巡线的情况下到达基准线，停止*/
    void gotoBaseline(void);
    /*调试阶段的启动函数，从基准线开始测试*/
    __DEBUG void testBaselineInit(void);
    /*  @brief 在基准线上移动到指定的baselineNode,默认此时车头要朝着场地前方
        @param newNode: 移动到的新的节点标号
    */
    void gotoBaselineNode(uint8_t newNode);

    /*向前移动，取回一个低难度的冰壶，并退回基准线*/
    void goTakeEasyCurling(void);
    /*  @brief 前进并检测是否是正确的简单冰壶;
            如是则取回，若不是则直接退回基准线.
        @retval status_t 是否是正确冰壶*/
    status_t goDetectEasyCurling(void);


    /*向前移动，取回一个高难度的冰壶，并退回基准线,TODO确定前进的时间*/
    void goTakeHardCurling(void);
    /*  @brief 前进并检测是否是正确的困难冰壶;
            如是则取回，若不是则直接退回基准线.
        @retval status_t 是否是正确冰壶*/
    status_t goDetectHardCurling(void);

    /*向前移动，发射冰壶，并退回基准线*/
    void goLaunchCurling(uint8_t launchNode=launchRight,status_t flag_trimLaunch=1,status_t flag_lastShot=0);
    /*检测是否脱线了*/
    status_t detectFatalError(void);
    /*在脱线之后尝试矫正回到线上*/
    status_t adjustBacktoTrace(void);

};

/* Exported macro ------------------------------------------------------------*/

/*关于路径的最高层次的类*/
extern path_t path;

/* Private defines ------------------------------------------------------------*/

#define __PATH

/*是最后一击，则尽快结束，取消最后的返回基准线的运动*/
#define LAST_SHOT (1)
/*直线发射*/
#define STRAIGHT_LAUNCH (0)
/*偏转一定角度发射*/
#define TRIM_LAUNCH (1)
/*在基准线附近移动时脱线的标志*/
#define LOST_TRACE (1)


#ifdef __cplusplus
}
#endif

#endif	/*__PATH_H*/
