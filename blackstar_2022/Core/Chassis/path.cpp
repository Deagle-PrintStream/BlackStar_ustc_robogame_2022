
#include "path.h"
#include "chassis.h"

#include "motor.h"
#include "tracer.h"
#include "patrol.h"/*移除*/

using namespace timeout_nsp;

/* Exported macro ------------------------------------------------------------*/

path_t path;

/* Private functions defines ------------------------------------------------------------*/



/*path_t construction &destruction func---------------------------------------------*/

path_t::path_t()
{
	baselineNode=lowerRightTurning;
	flag_Baseline=0;
	chassisHeadingDir=dirNowhere;
}

path_t::~path_t()
{
	baselineNode=lowerRightTurning;
	flag_Baseline=0;
	chassisHeadingDir=dirNowhere;
}

/*path_t private func---------------------------------------------*/

/*在起点等待一段时间*/
void path_t::waitInStartLine(void){
#ifdef __DEBUG
	HAL_Delay(startLineWaitingTime);
#endif
	chassis.onTrail=0;
	flag_Baseline=0;
	chassisHeadingDir=dirFront;
}

/*从起点的正方形区域内离开*/
void path_t::leaveStartLine(void){
	chassis.onTrail=1;
	chassis.togglePauseTracer(tracer_nsp::on);
	/*前进半步对准线*/
	chassis.move(dirFront,speedHigh);	 
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),decelerateTime);
	chassis.stop();
	/*离开出发点的正方形的框*/
	chassis.move(dirRight,speedHigh);
	WAIT_FOR(tracerDetect(dirFront,onTrace),leavingStartLineTime);
	HAL_Delay(passingForkTime);
	
}

/*到达上端的右转直角弯*/
void path_t::landonUpperTurning(void){

	//等待tracer[right]脱线，说明快到直角弯了
	WAIT_FOR(tracerDetect(dirRight,lostTrace),upperTurningTime);

	WAIT_FOR(tracerDetect(dirBack,exactTrace),decelerateTime);
	chassis.stop();
}

/*穿越无巡线路径*/
void path_t::gothroughWasteLand(void){
	/*向后移动*/
	chassis.move(dirBack,speedHigh);
	HAL_Delay(guideLineTime);
	/*开始冲刺*/
	chassis.onTrail=0;
	chassis.togglePauseTracer(tracer_nsp::off);
	chassis.move(dirBack,superDash,wasteLandTime);	
}

/*抵达基准线*/
void path_t::landonBaseline(void){
	chassis.togglePauseTracer(tracer_nsp::on);
	WAIT_FOR(tracerDetect(dirBack,onTrace),quiteLongTime);
	chassis.move(dirBack,speedLow);	/*减速是否是必要的呢？*/
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),decelerateTime);/*条件有点强*/
	chassis.stop();
	/*设置基线节点为node_0*/
	chassis.onTrail=1;
	flag_Baseline=1;
	baselineNode=lowerRightTurning;
	chassis.setGripperHeight(noCurlingHeight);
}

void path_t::goPassForks(direction_t newDir,uint8_t forkCount){
	/*越界矫正*/
	if(forkCount==0 || forkCount>baselineNodeCount){
		forkCount=1;
	}
	if(newDir!=dirRight && newDir!= dirLeft){
		return;
	}
	/*车子向指定方向前进,并挂起一段时间，确保当前节点不会影响判断*/
	chassis.move(newDir,speedHigh,passingForkTime);
	chassis.onTrail=1;

	/*若forkCount==1，则到达下一个路口就停止,*/
	if(forkCount==1){
		WAIT_FOR(tracerDetect(dirFront,exactTrace) || tracerDetect(dirBack,exactTrace),upperTurningTime);
		chassis.stop();
		return;
	}

	/*中间需要跳过的节点*/
	for(uint8_t i=0;i<forkCount-1;i++){
		if(baselineNode==lowerRightTurning){
			WAIT_FOR(tracerDetect(dirFront,onTrace) || tracerDetect(dirBack,onTrace),upperTurningTime);	
		}else{
			WAIT_FOR(tracerDetect(dirFront,onTrace) || tracerDetect(dirBack,onTrace),curlingIntervalTime);	
		}
		HAL_Delay(passingForkTime);
	}
	/*到达指定的节点*/
	WAIT_FOR(tracerDetect(dirFront,exactTrace) || tracerDetect(dirBack,exactTrace),decelerateTime);
	chassis.stop();
}

/*在十字交叉口上原地旋转180度*/
void path_t::turnBack(direction_t newDir){
	chassis.onTrail=1;
	chassis.stop();

	chassis.rotate(newDir,rotateSpeedDefault,rotateTime_180degree);

	WAIT_FOR(tracerDetect(dirFront,exactTrace),decelerateTime);
	HAL_Delay(150);//exactOnpath不够准确，经常提前停止，尝试手动delay
	chassis.stop();
	/*转过了，需要再转回去*/
	if(tracerDetect(dirFront,lostTrace)){
		chassis.rotate(oppositeDir(newDir),rotateSpeedDefault);
		WAIT_FOR(tracerDetect(dirFront,onTrace),decelerateTime);		
	}

	chassisHeadingDir=oppositeDir(chassisHeadingDir);
	adjustOnCrossing();
}

void path_t::putdownCurling(void){

#if (NEW_GRIPPER==1)	/*全新的机械爪，可以在下降后直接发射*/
	chassis.setGripperHeight(launchCurlingHeight);
	chassis.stop();
#else
	chassis.move(dirBack,speedLow,500);
	chassis.stop();
	chassis.setGripperHeight(launchCurlingHeight);
	chassis.move(dirBack,speedLow,1300);/*确保不会在放下机械臂时碰到冰壶*/
	chassis.stop();
	chassis.setGripperHeight(7);
	chassis.move(dirFront,speedLow,2000);/*确保发射时地盘顶住场地柱子*/
	chassis.stop();
#endif
}

status_t path_t::adjustOnCrossing(void){
	chassis.stop();
	chassis.onTrail=1;

	if(tracerDetect(dirFront,lostTrace)){
		//chassis.onTrail=0;
		chassis.move(dirRight,speedLow);
		WAIT_FOR(tracerDetect(dirFront,exactTrace),adjustCrossingTime);
		chassis.stop();
		chassis.move(dirLeft,speedLow);
		WAIT_FOR(tracerDetect(dirFront,exactTrace),adjustCrossingTime *2);
		chassis.stop();
		chassis.onTrail=1;		
	}

	if(tracerDetect(dirFront,exactTrace)){
		chassis.move(dirFront,speedHigh,adjustCrossingTime/2);
		WAIT_FOR(tracerDetect(dirBack,exactTrace),adjustCrossingTime);
		chassis.stop();
		chassis.move(dirBack,speedHigh,adjustCrossingTime/2);
		WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),adjustCrossingTime);
		chassis.stop();		
		return 1;
	}else{

		return 0;
	}
}

/*path_t public func---------------------------------------------*/

/*从出发点出发，经过第一个右转弯，无巡线的情况下到达基准线，停止*/
void path_t::gotoBaseline(void){

	waitInStartLine();
	leaveStartLine();
	landonUpperTurning();
	gothroughWasteLand();
	landonBaseline();

}

__DEBUG void path_t::testBaselineInit(void){
	HAL_Delay(startLineWaitingTime);
	chassis.onTrail=1;
	chassisHeadingDir=dirFront;
	baselineNode=lowerRightTurning;
	flag_Baseline=1;
	chassis.stop();
	chassis.setGripperHeight(noCurlingHeight);
}

/*  @brief 在基准线上移动到指定的baselineNode,默认此时车头要朝着场地前方
	@param newNode: 移动到的新的节点标号
*/
void path_t::gotoBaselineNode(uint8_t newNode){
	/*越界判断*/
	if(newNode>baselineNodeCount){
		return;
	}
	/*是否在基线上的判断*/
	if(flag_Baseline==0){
		return;
	}
	/*车头方向判断*/
	if(chassisHeadingDir==dirFront){
		/*计算需要途径的节点数量*/
		if(newNode>baselineNode){
			goPassForks(dirLeft,newNode-baselineNode);
		}else if(newNode<baselineNode){
			goPassForks(dirRight,baselineNode-newNode);		
		}
	}else if(chassisHeadingDir==dirBack){
		if(newNode>baselineNode){
			goPassForks(dirRight,newNode-baselineNode);
		}else if(newNode<baselineNode){
			goPassForks(dirLeft,baselineNode-newNode);		
		}
	}
	baselineNode=newNode;
}

/*向前移动，取回一个低难度的冰壶，并退回基准线*/
void path_t::goTakeEasyCurling(void){
	/*前进到冰壶面前*/
	//chassis.move(dirFront,speedLow,easyCurlingTakeTime);/*定距离前进去取壶，待定的参数*/
	chassis.move(dirFront,speedHigh,easyCurlingTakeTime);/*高速前进，修改了参数*/
	
	chassis.stop();
	chassis.setGripperHeight(holdEasyCurlingHeight);
    /*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),easyCurlingTakeTime);
	chassis.stop();

}
/*向前移动，取回一个高难度的冰壶，并退回基准线,TODO确定前进的时间*/
void path_t::goTakeHardCurling(void){
	if(chassisHeadingDir==dirFront){
		turnBack(dirRight);
	}
	/*前进到冰壶面前*/
	chassis.setGripperHeight(takeHardCurlingHeight);/*在接近冰壶前需要先将铲子抬高到底座高度5cm*/
	chassis.move(dirFront,speedHigh,hardCurlingTakeTime/2);/*高速取壶*/
	
	chassis.stop();
	chassis.setGripperHeight(holdHardCurlingHeight);

    /*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),hardCurlingTakeTime);
	chassis.stop();
	chassis.setGripperHeight(holdEasyCurlingHeight);
	if(chassisHeadingDir==dirBack){
		turnBack(dirLeft);
	}
}

void trimThenLaunch(direction_t newDir){
	chassis.onTrail=0;
#ifdef NEW_GRIPPER
	//trimDirection(newDir); // 该函数利用陀螺仪实现定角度偏转
	chassis.setGripperHeight(launchCurlingHeight);
	chassis.pushCurling();
	//trimDirectionback(oppositeDir(newDir)); // 该函数利用陀螺仪实现定角度偏转回来	
	chassis.onTrail=1;
#else

	PushCurlingTrimBack(newDir);
	chassis.setGripperHeight(launchCurlingHeight);
	chassis.pushCurling();
	PushCurlingTrimFront(newDir);

#endif
	chassis.onTrail=1;
}


/*向前移动，发射冰壶，并退回基准线*/
void path_t::goLaunchCurling(uint8_t launchNode,status_t flag_trimLaunch,status_t flag_lastShot){
	/*根据车头方向判断是否需要掉头*/
	if(chassisHeadingDir==dirBack){
		turnBack(dirRight);
	}
	/*准备发射，确保顶到发射边沿的柱子*/
	//chassis.move(dirFront,speedLow,easyCurlingTakeTime);
	chassis.move(dirFront,speedHigh,easyCurlingTakeTime);/*高速前进发射冰壶*/
	chassis.stop();
	/*是否需要偏转发射*/
	if(flag_trimLaunch==0){
		putdownCurling();
		chassis.pushCurling();
	}else{
		if(launchNode==launchRight){
			trimThenLaunch(dirLeft);
		}else{
			trimThenLaunch(dirRight);
		}
	}
    /*退回基准线，如果为最后一次则停止运动TODO*/
	if(flag_lastShot==1){
#ifdef __DEBUG
		chassis.stop();
		chassis.setGripperHeight(noCurlingHeight);
#else
		return;
#endif
	}
    chassis.move(dirBack,speedHigh);
    WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace) ,easyCurlingTakeTime);
	chassis.stop();
	chassis.setGripperHeight(noCurlingHeight);

}

status_t path_t::goDetectEasyCurling(void){
	status_t flag=0;
	/*前进到冰壶面前*/
	chassis.move(dirFront,speedHigh,easyCurlingTakeTime);/*定距离前进去取壶，待定的参数*/
	
	chassis.stop();
	flag=chassis.detectCode(1);
	if(flag==1){
		chassis.setGripperHeight(holdEasyCurlingHeight);
	}else{
		//do nothing
	}
	/*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace),easyCurlingTakeTime);
	chassis.stop();
	return flag;
}

status_t path_t::goDetectHardCurling(void){
	status_t flag=0;
	/*前进到冰壶面前*/
	if(chassisHeadingDir==dirFront){
		turnBack(dirRight);
	}
	chassis.setGripperHeight(takeHardCurlingHeight);
	chassis.move(dirFront,speedLow,hardCurlingTakeTime);/*定距离前进去取壶，待定的参数*/
	chassis.stop();
	flag=chassis.detectCode(1);
	if(flag==1){
		chassis.setGripperHeight(holdHardCurlingHeight);
	}else{
		//do nothing
	}
	/*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	WAIT_FOR(tracerDetect(dirLeft,exactTrace) || tracerDetect(dirRight,exactTrace) ,hardCurlingTakeTime);
	chassis.stop();
	if(flag==0){
		chassis.setGripperHeight(takeHardCurlingHeight);
	}else{
		chassis.setGripperHeight(holdEasyCurlingHeight);
	}
	/*取到壶了则掉头准备去发射，否则不用掉头，直接平移*/
	if(flag==1){
		turnBack(dirLeft);
	}else{
		//do nothing
	}
	return flag;
}

status_t path_t::detectFatalError(void){
	if(flag_Baseline==0){
		return 0;
	}
	uint8_t tracerDir=chassis.headingDir;
	if(tracerDir!=dirNowhere && tracer[tracerDir].onPath==0){
		return FATAL_ERROR;
	}else{
		return 0;
	}
}

status_t path_t::adjustBacktoTrace(void){
	chassis.stop();

	return 1;
}


/* Private functions defines ---------------------------------------------*/
