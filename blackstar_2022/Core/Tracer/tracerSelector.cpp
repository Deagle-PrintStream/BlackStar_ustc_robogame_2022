
#include "tracerSelector.h"
#include "tracer.h"
#include "chassis.h"

/* Exported macro ------------------------------------------------------------*/

selector_t tracerSelector;

/* Exported functions defines ---------------------------------------------*/

/* Private functions defines ---------------------------------------------*/

inline void setSelectorGPIO(GPIO_PinState outA,GPIO_PinState outB){
	HAL_GPIO_WritePin(SELECTOR0_GPIO_Port,SELECTOR0_Pin,outA);
	HAL_GPIO_WritePin(SELECTOR1_GPIO_Port,SELECTOR1_Pin,outB);

}
/* Class functions defines ---------------------------------------------*/

/*Selector_t functions defines---------------------------------------------*/

selector_t::selector_t(){
	currDir=(uint8_t)dirFront;
}

selector_t::~selector_t(){

}

void selector_t::updateOutput(void)const{
	#define ON GPIO_PIN_SET
	#define OFF GPIO_PIN_RESET

	void setSelectorGPIO(GPIO_PinState outA,GPIO_PinState outB);

	/*NOTE:与理论返回逻辑值完全不同，可能是接线问题，
		最初的设计依次为fornt,right,back,left*/
	/*历史遗留问题之一*/
	switch ((direction_t)currDir){
	case dirBack:
		setSelectorGPIO(OFF,OFF);
		break;
	case dirLeft:
		setSelectorGPIO(ON,OFF);
		break;
	case dirRight:
		setSelectorGPIO(OFF,ON);
		break;
	case dirFront:
		setSelectorGPIO(ON,ON);
		break;
	default:
		break;
	}

	#undef ON
	#undef OFF
}

void selector_t::setCurrDir(direction_t newDir){
	currDir=(uint8_t)newDir;
	updateOutput();
}

direction_t selector_t::update(void){
	/*理应使用 getrightDir()，由于历史遗留原因，硬件配置不方便改动，故保留不动*/
	if(currDir<dirBack){
		currDir++;
	}else{
		currDir=dirFront;
	}
	updateOutput();
	return (direction_t)currDir;
}
