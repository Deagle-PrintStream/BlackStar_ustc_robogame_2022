#ifndef __BSP_STEP_MOTOR_INIT_H
#define	__BSP_STEP_MOTOR_INIT_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "bsp_stepper_T_speed.h"
#include "stm32f1xx_hal_tim.h"

/*�궨��*/
/*******************************************************/
//�궨���Ӧ������Ľӿ� 1 ��2 ��3 ��4
#define CHANNEL_SW 1

#if(CHANNEL_SW == 1)
//Motor ���� 
#define MOTOR_DIR_PIN                  	GPIO_PIN_6  
#define MOTOR_DIR_GPIO_PORT            	GPIOE               
#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOE_CLK_ENABLE()

//Motor ʹ�� 
#define MOTOR_EN_PIN                  	GPIO_PIN_5
#define MOTOR_EN_GPIO_PORT            	GPIOE                       
#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOE_CLK_ENABLE()
	
//Motor ����
#define MOTOR_PUL_IRQn                  TIM8_CC_IRQn
#define MOTOR_PUL_IRQHandler            TIM8_CC_IRQHandler

#define MOTOR_PUL_TIM                   TIM8
#define MOTOR_PUL_CLK_ENABLE()  		    __HAL_RCC_TIM8_CLK_ENABLE()

#define MOTOR_PUL_PORT       			      GPIOC
#define MOTOR_PUL_PIN             		  GPIO_PIN_6
#define MOTOR_PUL_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOC_CLK_ENABLE()
#define MOTOR_PUL_CHANNEL_x             TIM_CHANNEL_1

#define MOTOR_TIM_IT_CCx                TIM_IT_CC1
#define MOTOR_TIM_FLAG_CCx              TIM_FLAG_CC1
#elif(CHANNEL_SW == 2)

//Motor ���� 
#define MOTOR_DIR_PIN                  	GPIO_PIN_15
#define MOTOR_DIR_GPIO_PORT            	GPIOD    
#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOD_CLK_ENABLE()

//Motor ʹ�� 
#define MOTOR_EN_PIN                  	GPIO_PIN_13
#define MOTOR_EN_GPIO_PORT            	GPIOC                      
#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOC_CLK_ENABLE()
	
//Motor ����
#define MOTOR_PUL_IRQn                  TIM8_CC_IRQn
#define MOTOR_PUL_IRQHandler            TIM8_CC_IRQHandler

#define MOTOR_PUL_TIM                   TIM8
#define MOTOR_PUL_CLK_ENABLE()  		    __HAL_RCC_TIM8_CLK_ENABLE()

#define MOTOR_PUL_PORT       			      GPIOC
#define MOTOR_PUL_PIN             	  	GPIO_PIN_7
#define MOTOR_PUL_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOC_CLK_ENABLE()
#define MOTOR_PUL_CHANNEL_x             TIM_CHANNEL_2

#define MOTOR_TIM_IT_CCx                TIM_IT_CC2
#define MOTOR_TIM_FLAG_CCx              TIM_FLAG_CC2
#elif(CHANNEL_SW == 3)

//Motor ���� 
#define MOTOR_DIR_PIN                  	GPIO_PIN_2
#define MOTOR_DIR_GPIO_PORT            	GPIOD          
#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOD_CLK_ENABLE()

//Motor ʹ�� 
#define MOTOR_EN_PIN                  	GPIO_PIN_12
#define MOTOR_EN_GPIO_PORT            	GPIOC                 
#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOC_CLK_ENABLE()
	
//Motor ����
#define MOTOR_PUL_IRQn                  TIM8_CC_IRQn
#define MOTOR_PUL_IRQHandler            TIM8_CC_IRQHandler

#define MOTOR_PUL_TIM                   TIM8
#define MOTOR_PUL_CLK_ENABLE()  		    __HAL_RCC_TIM8_CLK_ENABLE()

#define MOTOR_PUL_PORT       			      GPIOC
#define MOTOR_PUL_PIN             		  GPIO_PIN_8
#define MOTOR_PUL_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOC_CLK_ENABLE()
#define MOTOR_PUL_CHANNEL_x             TIM_CHANNEL_3

#define MOTOR_TIM_IT_CCx                TIM_IT_CC3
#define MOTOR_TIM_FLAG_CCx              TIM_FLAG_CC3
#elif(CHANNEL_SW == 4)

//Motor ���� 
#define MOTOR_DIR_PIN                  	GPIO_PIN_10
#define MOTOR_DIR_GPIO_PORT            	GPIOC
#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOC_CLK_ENABLE()

//Motor ʹ�� 
#define MOTOR_EN_PIN                  	GPIO_PIN_11
#define MOTOR_EN_GPIO_PORT            	GPIOC       
#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOA_CLK_ENABLE()
	
//Motor ����
#define MOTOR_PUL_IRQn                  TIM8_CC_IRQn
#define MOTOR_PUL_IRQHandler            TIM8_CC_IRQHandler

#define MOTOR_PUL_TIM                   TIM8
#define MOTOR_PUL_CLK_ENABLE()  		    __HAL_RCC_TIM8_CLK_ENABLE()

#define MOTOR_PUL_PORT       			      GPIOC
#define MOTOR_PUL_PIN                   GPIO_PIN_9
#define MOTOR_PUL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define MOTOR_PUL_CHANNEL_x             TIM_CHANNEL_4

#define MOTOR_TIM_IT_CCx                TIM_IT_CC4
#define MOTOR_TIM_FLAG_CCx              TIM_FLAG_CC4
#endif

/****************************************************************/

#define HIGH GPIO_PIN_SET	      //�ߵ�ƽ
#define LOW  GPIO_PIN_RESET	    //�͵�ƽ

#define ON  LOW	              //��
#define OFF HIGH	                //��

#define CW 	LOW		              //˳ʱ��
#define CCW HIGH      	        //��ʱ��

//����ʹ������
/* ���κ꣬��������������һ��ʹ�� */
#define MOTOR_EN(x)					HAL_GPIO_WritePin(MOTOR_EN_GPIO_PORT,MOTOR_EN_PIN,x)
#define MOTOR_PUL(x)				HAL_GPIO_WritePin(MOTOR_PUL_GPIO_PORT,MOTOR_PUL_PIN,x)
#define MOTOR_DIR(x)				HAL_GPIO_WritePin(MOTOR_DIR_GPIO_PORT,MOTOR_DIR_PIN,x)
static  __IO uint32_t set_speed  = 4000;         // 速度 单位为0.05rad/sec
// 加速度和减速度选取一般根据实际需要，值越大速度变化越快，加减速阶段比较抖动
// 所以加速度和减速度值一般是在实际应用中多尝试出来的结果
static __IO uint32_t accel_val = 500;         // 加速度 单位为0.0.5rad/sec^2
static __IO uint32_t decel_val = 500;         // 减速度 单位为0.05rad/sec^2
#define MOTOR_EN_TOGGLE     HAL_GPIO_TogglePin(MOTOR_EN_GPIO_PORT,MOTOR_EN_PIN)
#define MOTOR_PUL_TOGGLE    HAL_GPIO_TogglePin(MOTOR_PUL_PORT,MOTOR_PUL_PIN)


//����Ƚ�ģʽ��������Ϊ0xFFFF
#define TIM_PERIOD                   0xFFFF


//�����Լ���������
extern TIM_HandleTypeDef TIM_TimeBaseStructure;
void stepper_Init(void);
void stepper_move_T(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void TIM_PWMOUTPUT_Config(void);
#endif
