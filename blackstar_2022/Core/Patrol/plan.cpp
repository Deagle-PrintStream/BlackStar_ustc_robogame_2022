/**
 * @file plan.cpp
 * @author Swayee2022 (malygosa@mail.ustc.edu.cn)
 * @brief 不同方案的路径实现过程，是path_t的方法的线性调用
 * @version 1.4
 * @date 2022-10-06
 * @see https://github.com/Deagle-PrintStream/connectivity_merge/tree/project_struct_rebuild
 * @see debug.h
 * @copyright Copyright (c) 2022
 * @note 目前根据规则，备用方案1和2均告失败，视觉方案理论可行，但是中途的错误率很高，机械臂指令发送失败是致命错误
 * @warning 需要根据debug.h中的宏对应修改，否则代码块在编译阶段就被吞了，无返回
 */
#include "plan.h"
#include "path.h"

/* Private functions prototypes ---------------------------------------------*/

/*左右各发射两个冰壶*/
void patrol_ver1(void);
/*在左侧发射所有冰壶*/
void patrol_ver2(void);

/* Exported functions defines ---------------------------------------------*/

void testPatrol(void){

#if (LAUNCH_ALL_LEFT==0)	/*是否全部在一侧发射冰壶 */
	patrol_ver1(); 
#else
	patrol_ver2();
#endif

}

/* Private functions defines ---------------------------------------------*/

void patrol_ver1(void){

	path.gotoBaseline();

	status_t flag_correctCurling=0;

	//1st curling with detecting
	path.gotoBaselineNode(easyCurlingRight_1);
	flag_correctCurling=path.goDetectEasyCurling();
	if(flag_correctCurling==1){
		//nothing cause curling has been taken back
	}else{
		path.gotoBaselineNode(easyCurlingRight_2);
		path.goTakeEasyCurling();
	}
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight,TRIM_LAUNCH);

	//2nd curling with no need of detecting
	path.gotoBaselineNode(easyCurlingMide);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight,TRIM_LAUNCH);

	//3rd curling with detecting
	path.gotoBaselineNode(hardCurling_1);
	flag_correctCurling=path.goDetectHardCurling();
	if(flag_correctCurling==1){
		//先把正确的发射了，再检测第二个困难冰壶
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft,TRIM_LAUNCH);
		path.gotoBaselineNode(hardCurling_2);

		flag_correctCurling=path.goDetectHardCurling();
		if(flag_correctCurling==1){
			path.gotoBaselineNode(launchLeft);
			path.goLaunchCurling(launchLeft,TRIM_LAUNCH,LAST_SHOT);
		}else{
			path.gotoBaselineNode(hardCurling_3);
			path.goTakeHardCurling();
			path.gotoBaselineNode(launchLeft);
			path.goLaunchCurling(launchLeft,TRIM_LAUNCH,LAST_SHOT);
		}

	}else{
		//检测到了错误的困难冰壶，则剩下两个都是正确的
		/*取第一个困难冰壶并发射*/
		path.gotoBaselineNode(hardCurling_2);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft);
		/*取第二个困难冰壶并发射*/
		path.gotoBaselineNode(hardCurling_3);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft,1,LAST_SHOT);
	}

}

void patrol_ver2(void){
	/*前往基准线*/
	path.gotoBaseline();
	status_t flag_correctCurling=0;

	//1st curling with detecting
	path.gotoBaselineNode(easyCurlingRight_1);
	flag_correctCurling=path.goDetectEasyCurling();
	if(flag_correctCurling==1){
		//nothing cause curling has been taken back
	}else{
		path.gotoBaselineNode(easyCurlingRight_2);
		path.goTakeEasyCurling();
	}
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft,TRIM_LAUNCH);

	//2nd curling with no need of detecting
	path.gotoBaselineNode(easyCurlingMide);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft,TRIM_LAUNCH);

	//3rd curling with detecting
	path.gotoBaselineNode(hardCurling_3);
	flag_correctCurling=path.goDetectHardCurling();
	if(flag_correctCurling==1){
		//先把正确的发射了，再检测第二个困难冰壶
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft,TRIM_LAUNCH);
		path.gotoBaselineNode(hardCurling_2);

		flag_correctCurling=path.goDetectHardCurling();
		if(flag_correctCurling==1){
			path.gotoBaselineNode(launchLeft);
			path.goLaunchCurling(launchLeft,TRIM_LAUNCH,LAST_SHOT);
		}else{
			path.gotoBaselineNode(hardCurling_1);
			path.goTakeHardCurling();
			path.gotoBaselineNode(launchLeft);
			path.goLaunchCurling(launchLeft,TRIM_LAUNCH,LAST_SHOT);
		}

	}else{
		//检测到了错误的困难冰壶，则剩下两个都是正确的
		/*取第一个困难冰壶并发射*/
		path.gotoBaselineNode(hardCurling_2);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft);
		/*取第二个困难冰壶并发射*/
		path.gotoBaselineNode(hardCurling_1);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft,1,LAST_SHOT);
	}
}
