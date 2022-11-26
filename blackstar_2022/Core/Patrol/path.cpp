
#include "path.h"
#include "chassis.h"

#include "motor.h"
#include "tracer.h"
#include "gyroscope.h"
#include "counter.h"

using namespace timeout_nsp;
using tracer_nsp::lostTrace;

/* Exported macro ------------------------------------------------------------*/

path_t path;

/* Private functions defines ------------------------------------------------------------*/

/**
 * @brief Get the Base Angle after launching from start line, unused as well since gyroscope couldn't help a bit
 */
inline void getBaseAngle(void){
	static const uint8_t countMax=200;
	float localBaseAngle=0;

	enableGyroscope();
	for(uint8_t count=0;count<countMax;count++){
		localBaseAngle+=getAngle();
		HAL_Delay(30);
	}
	disableGyroscope();

	baseAngle=localBaseAngle/countMax - (1.5);/*手动调整一点点*/
}

inline void adjustbyGyroscope(float oldAngle,direction_t headingDir,uint8_t speed,uint32_t timeout){
	enableGyroscope();
	float newAngle=getAngle();
	float trimIntensity=0;
	tracerCounter.setup(timeout);
	chassis.move(headingDir,speed);

	while(tracerCounter.ifTimeout()==0){
		newAngle=getAngle();
		trimIntensity=ABS(newAngle-oldAngle)*0.05;
		if(newAngle>oldAngle+0.3){
			chassis.publicTrim(dirRight,trimIntensity);
		}else if(newAngle<oldAngle-0.3){
			chassis.publicTrim(dirLeft,trimIntensity);
		}else{
			chassis.move(headingDir,speed);
		}
		HAL_Delay(100);
	}
	disableGyroscope();
}

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
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,decelerateTime);
	chassis.stop();
	/*离开出发点的正方形的框*/
	chassis.move(dirRight,speedHigh);
	waitforTracerDetect(dirFront,onTrace,leavingStartLineTime);
	HAL_Delay(passingForkTime);
	
}

/*到达上端的右转直角弯*/
void path_t::landonUpperTurning(void){
	
	//在前进过程中求出基准角度
	getBaseAngle();
	//等待tracer[right]脱线，说明快到直角弯了
	waitforTracerDetect(dirRight,lostTrace,upperTurningTime/2);
	chassis.move(dirRight,speedLow);

	waitforTracerDetect(dirBack,exactTrace,2500);
	chassis.stop();
}

/*穿越无巡线路径*/
void path_t::gothroughWasteLand(void){
	/*向后移动*/
	chassis.onTrail=1;
	chassis.move(dirBack,speedHigh);
	HAL_Delay(guideLineTime);
	/*开始冲刺*/
	chassis.onTrail=0;
	
	chassis.move(dirBack,superDash);

	/*手动要求车子向前进方向左侧偏转一点再调回*/
	HAL_Delay(11500);
	chassis.publicTrim(dirLeft,0.4);
	HAL_Delay(1000);
	chassis.publicTrim(dirRight,0.4);
	HAL_Delay(800);
	chassis.move(dirBack,superDash);

}

/*抵达基准线*/
void path_t::landonBaseline(void){
	//chassis.togglePauseTracer(tracer_nsp::on);
	waitforTracerDetect(dirBack,onTrace,quiteLongTime);

	chassis.move(dirBack,speedLow);
	waitforTracerDetect(dirRight,exactTrace,decelerateTime);
	chassis.stop();
	/*设置基线节点为node_0*/
	chassis.onTrail=1;
	flag_Baseline=1;
	baselineNode=lowerRightTurning;
	chassis.setGripperHeight(noCurlingHeight,0);
	chassis.move(dirLeft,speedHigh,1000);
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
		waitforTracerDetect(dirFront,exactTrace,upperTurningTime);
		chassis.stop();
		return;
	}

	/*中间需要跳过的节点*/
	for(uint8_t i=0;i<forkCount-1;i++){
		if(baselineNode==lowerRightTurning){
			waitforTracerDetect(dirFront,onTrace,upperTurningTime);
		
		}else{
			waitforTracerDetect(dirFront,onTrace,curlingIntervalTime);
		}
		HAL_Delay(passingForkTime);
	}
	/*到达指定的节点*/
	waitforTracerDetect(dirFront,exactTrace,decelerateTime);
		
	chassis.stop();
}

/*在十字交叉口上原地旋转180度*/
void path_t::turnBack(direction_t newDir){
	if(tracerDetect(dirRight,exactTrace)==0 && tracerDetect(dirLeft,exactTrace)==0 ){
		adjustOnCrossing();
	}
	chassis.rotate(newDir,rotateSpeedDefault);
	HAL_Delay(1000);
	waitforTracerDetect(dirFront,exactTrace,3500);

	HAL_Delay(1000);
	if( waitforTracerDetect(dirFront,exactTrace,3500)==0){
		chassis.stop();
		chassis.rotate(oppositeDir(newDir),speedLow);
		waitforTracerDetect(dirFront,exactTrace,1000);
	}
	chassis.stop();
	chassisHeadingDir=oppositeDir(chassisHeadingDir);
}

status_t path_t::adjustOnCrossing(void){
	chassis.stop();
	chassis.onTrail=1;

	goPassForks(dirRight,1);
	goPassForks(dirLeft,1);

	chassis.stop();
	return 1;
}

/*path_t public func---------------------------------------------*/

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
	chassis.setGripperHeight(noCurlingHeight,0);
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

	static const uint32_t hangupTime=1000;
	/*前进到冰壶面前*/
	chassis.move(dirBack,speedLow,500);
	chassis.move(dirFront,speedHigh,1400);/*高速前进，修改了参数*/
	chassis.move(dirFront,speedLow,1000);
	
	chassis.stop();
	chassis.setGripperHeight(holdEasyCurlingHeight);
    /*退回到基准线*/
	chassis.move(dirBack,speedHigh,hangupTime);
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,easyCurlingTakeTime);
	chassis.stop();

}
/*向前移动，取回一个高难度的冰壶，并退回基准线,TODO确定前进的时间*/
void path_t::goTakeHardCurling(void){
	if(chassisHeadingDir==dirFront){
		turnBack(dirRight);
	}
	/*前进到冰壶面前*/
	chassis.setGripperHeight(takeHardCurlingHeight,0);/*在接近冰壶前需要先将铲子抬高到底座高度5cm*/
	chassis.move(dirFront,speedHigh,2200);/*高速取壶*/
	chassis.move(dirFront,speedLow,800);
	
	chassis.stop();
	chassis.setGripperHeight(holdHardCurlingHeight);

    /*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,4000);
	chassis.stop();
	chassis.setGripperHeight(holdEasyCurlingHeight,0);
	if(chassisHeadingDir==dirBack){
		turnBack(dirRight);
	}
}

inline void trimThenLaunch(direction_t newDir,float angle){
  chassis.onTrail=0;
  chassis.stop();

#if (NEW_GRIPPER==1)
	/*偏转的角度，定值*/
	chassis.rotatebyDegree(newDir,angle);
	chassis.setGripperHeight(launchCurlingHeight,2000);

	chassis.pushCurling();	

	chassis.rotatebyDegree(oppositeDir(newDir),angle);
	chassis.stop();

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
	chassis.move(dirFront,speedHigh,1400);/*高速前进发射冰壶*/
	chassis.move(dirFront,speedLow,1600);
	chassis.stop();
	/*偏转发射--------------------------------------------------------------------------------------------------------------*/
#if(RED_OR_YELLOW==0)
	static const float leftLaunchAngle=0.5;
	static const float rightLaunchAngle=3.5;
#else
	static const float leftLaunchAngle=1;
	static const float rightLaunchAngle=6;
#endif

	if(launchNode==launchRight){
		trimThenLaunch(dirLeft,leftLaunchAngle);/*在右向左发射*/
	}else{
		trimThenLaunch(dirRight,rightLaunchAngle); /*在左向右发射*/
	}
    /*退回基准线，如果为最后一次则停止运动*/
	if(flag_lastShot==1){
		return;
	}
	chassis.move(dirBack,speedHigh);
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,3000);
	chassis.stop();

}

status_t path_t::goDetectEasyCurling(void){
	status_t flag=0;
	static const uint32_t hangupTime=1000;
	/*前进到冰壶面前*/
	chassis.move(dirFront,speedHigh,1300);
	chassis.move(dirFront,speedLow,1400);
	
	chassis.stop();
	flag=chassis.detectCode(3);
	if(flag==1){
		chassis.setGripperHeight(holdEasyCurlingHeight);
	}
	/*退回到基准线*/
	chassis.move(dirBack,speedHigh,hangupTime/2);
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,3000);
	chassis.stop();
	return flag;
}

status_t path_t::goDetectHardCurling(void){
	status_t flag=0;
	/*前进到冰壶面前*/
	if(chassisHeadingDir==dirFront){
		turnBack(dirRight);
	}
	chassis.setGripperHeight(takeHardCurlingHeight,0);
	chassis.move(dirFront,speedHigh,2200);
	chassis.stop();
	flag=chassis.detectCode(3);
	if(flag==1){
		chassis.setGripperHeight(holdHardCurlingHeight);
	}
	/*退回到基准线*/
	chassis.move(dirBack,speedHigh);
	waitforDuelTracerDetect(either,dirLeft,dirRight,exactTrace,4000);
	chassis.stop();
	if(flag==0){
		chassis.setGripperHeight(takeHardCurlingHeight,0);
	}else{
		chassis.setGripperHeight(holdEasyCurlingHeight,0);
	}
	/*取到壶了则掉头准备去发射，否则不用掉头，直接平移*/
	if(flag==1){
		turnBack(dirLeft);
	}
	return flag;
}

status_t path_t::detectFatalError(void){
	if(flag_Baseline==0){
		return 0;
	}
	uint8_t tracerDir=chassis.headingDir;
	if(tracerDir!=dirNowhere && tracerDetect(tracerDir,lostTrace)==1){
		flag_fatalError=1;
	}else{
		flag_fatalError=0;
	}
	return flag_fatalError;
}

status_t path_t::adjustBacktoTrace(void){
	chassis.stop();

	return 1;
}
