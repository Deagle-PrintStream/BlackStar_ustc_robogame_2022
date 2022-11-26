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

/* Exported functions defines ---------------------------------------------*/

__DEBUG void testPath_ver3(void){

#if (WHOLE_PROCEDURE==1)
	path.gotoBaseline();
#else
	path.testBaselineInit();
#endif

#if (BACKUP_PLAN==0)
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
	path.goLaunchCurling(launchRight);

	//2nd curling with no need of detecting
	path.gotoBaselineNode(easyCurlingMide);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);

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
		path.goLaunchCurling(launchLeft,STRAIGHT_LAUNCH,LAST_SHOT);
	}

#endif
}


__DEBUG void testPath_ver4(void){
	/*前往基准线*/
	//path.gotoBaseline();
	path.testBaselineInit();
#if(BACKUP_PLAN==1)
	/*取第一个简单冰壶并发射*/
	path.gotoBaselineNode(easyCurlingRight_1);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);
	/*取第二个简单冰壶并发射*/
	path.gotoBaselineNode(easyCurlingMide);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);
	/*取第一个困难冰壶并发射*/
	path.gotoBaselineNode(hardCurling_1);
	path.goTakeHardCurling();
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft);
	/*取第二个困难冰壶并发射*/
	path.gotoBaselineNode(hardCurling_2);
	path.goTakeHardCurling();
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft,LAST_SHOT);

#endif
}

__DEBUG void testPath_ver5(void){

	/*前往基准线*/
	//path.gotoBaseline();
	path.testBaselineInit();
	/*取第一个简单冰壶并发射*/
#if (BACKUP_PLAN==1)

/*取第一个困难冰壶并发射*/
#if(EASY_CURLING_NODE_1==1)
	path.gotoBaselineNode(easyCurlingRight_1);
#else
	path.gotoBaselineNode(easyCurlingRight_1);
	HAL_Delay(fakeDetectingTime);
	path.gotoBaselineNode(easyCurlingRight_2);
#endif
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);

	/*取第二个简单冰壶并发射*/
	path.gotoBaselineNode(easyCurlingMide);
	path.goTakeEasyCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);

/*取第一个困难冰壶并发射*/
#if(HARD_CURLING_NODE_1==3)

	path.gotoBaselineNode(hardCurling_1);
	path.goTakeHardCurling();
	path.gotoBaselineNode(launchRight);
	path.goLaunchCurling(launchRight);

	#if(HARD_CURLING_NODE_2==4)
		path.gotoBaselineNode(hardCurling_2);

	#elif(HARD_CURLING_NODE_2==5)
		path.gotoBaselineNode(hardCurling_2);
		path.turnBack(dirRight);
		HAL_Delay(fakeDetectingTime);
		path.turnBack(dirLeft);
		path.gotoBaselineNode(hardCurling_3);

	#endif

#elif (HARD_CURLING_NODE_1==4)
	path.gotoBaselineNode(hardCurling_1);
	path.turnBack(dirRight);
	HAL_Delay(fakeDetectingTime);
	path.turnBack(dirLeft);
	path.gotoBaselineNode(hardCurling_2);
	path.goTakeHardCurling();
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft);

	path.gotoBaselineNode(hardCurling_3);

#endif
	/*取第二个困难冰壶并发射*/
	path.goTakeHardCurling();
	path.gotoBaselineNode(launchLeft);
	path.goLaunchCurling(launchLeft,LAST_SHOT);

#endif

}

__DEBUG void testPath_ver6(void){

	/*前往基准线*/
	//path.gotoBaseline();
	path.testBaselineInit();
	/*取第一个简单冰壶并发射*/
#if (BACKUP_PLAN==2)

	/*取第一个困难冰壶并发射*/
	if(bp_easyCurlingNode==1){
		path.gotoBaselineNode(easyCurlingRight_1);
	}else{
		path.gotoBaselineNode(easyCurlingRight_1);
		HAL_Delay(fakeDetectingTime);
		path.gotoBaselineNode(easyCurlingRight_2);
	}
		path.goTakeEasyCurling();
		path.gotoBaselineNode(launchRight);
		path.goLaunchCurling(launchRight);

		/*取第二个简单冰壶并发射*/
		path.gotoBaselineNode(easyCurlingMide);
		path.goTakeEasyCurling();
		path.gotoBaselineNode(launchRight);
		path.goLaunchCurling(launchRight);

	/*取第一个困难冰壶并发射*/
	if(bp_hardCurlingNode_1==3){

		path.gotoBaselineNode(hardCurling_1);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchRight);
		path.goLaunchCurling(launchRight);

		if(bp_hardCurlingNode_2==4){
			path.gotoBaselineNode(hardCurling_2);

		}else if(bp_hardCurlingNode_2==5){
			path.gotoBaselineNode(hardCurling_2);
			path.turnBack(dirRight);
			HAL_Delay(fakeDetectingTime);
			path.turnBack(dirLeft);
			path.gotoBaselineNode(hardCurling_3);
		}

	}else if(bp_hardCurlingNode_1==4){
		path.gotoBaselineNode(hardCurling_1);
		path.turnBack(dirRight);
		HAL_Delay(fakeDetectingTime);
		path.turnBack(dirLeft);
		path.gotoBaselineNode(hardCurling_2);
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft);

		path.gotoBaselineNode(hardCurling_3);

		}
		/*取第二个困难冰壶并发射*/
		path.goTakeHardCurling();
		path.gotoBaselineNode(launchLeft);
		path.goLaunchCurling(launchLeft,LAST_SHOT);

#endif

}
