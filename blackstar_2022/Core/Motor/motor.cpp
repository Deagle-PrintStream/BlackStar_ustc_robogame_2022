/**
 * @file motor.c
 * @author stubborncake
 * @brief 电机运动相关函数
 * 
 * DO NOT TRY TO UNDERSTAND IT, FEEL IT.
 * 
 * @version 1.0
 * @date 2022-10-06
 * @see https://github.com/stubborncake/RobogameMoveCode
 * @copyright Copyright (c) 2022
 * @warning 由于多次utf8-gbk之间转码的问题，原先的注释全部变成了乱码，且无法转回。故谨慎修改！
 */
#include "motor.h"
#include "connectivity.h"
#include "pid.h"
#include "string.h"

int target1, target2, target3, target4;
int DoubleBegin, Again;

int Read_Encoder(int TIMX)
{
	int Encoder_TIM;
	switch (TIMX)
	{
	case 1:
		Encoder_TIM = (short)TIM1->CNT;
		TIM1->CNT = 0;
		break;
	case 2:
		Encoder_TIM = (short)TIM2->CNT;
		TIM2->CNT = 0;
		break;
	case 3:
		Encoder_TIM = (short)TIM3->CNT;
		TIM3->CNT = 0;
		break;
	case 8:
		Encoder_TIM = (short)TIM8->CNT;
		TIM8->CNT = 0;
		break;
	default:
		Encoder_TIM = 0;
	}
	return Encoder_TIM;
}

#define WRITE_PIN(a1, a2, a3, a4, a5, a6, a7, a8) \
	do                                            \
	{                                             \
		HAL_GPIO_WritePin(GPIOF, m11_Pin, a1);    \
		HAL_GPIO_WritePin(GPIOF, m12_Pin, a2);    \
		HAL_GPIO_WritePin(GPIOF, m21_Pin, a3);    \
		HAL_GPIO_WritePin(GPIOF, m22_Pin, a4);    \
		HAL_GPIO_WritePin(GPIOC, m31_Pin, a5);    \
		HAL_GPIO_WritePin(GPIOC, m32_Pin, a6);    \
		HAL_GPIO_WritePin(GPIOC, m41_Pin, a7);    \
		HAL_GPIO_WritePin(GPIOC, m42_Pin, a8);    \
	} while (0)

#define ON (GPIO_PIN_SET)
#define OFF (GPIO_PIN_RESET)

//��ǰ
void FrontMove(void)
{
	WRITE_PIN(ON, OFF, OFF, ON, ON, OFF, OFF, ON);
}
//���
void BackMove(void)
{
	WRITE_PIN(OFF, ON, ON, OFF, OFF, ON, ON, OFF);
}
//����
void LeftMove(void)
{
	WRITE_PIN(OFF, ON, OFF, ON, ON, OFF, ON, OFF);
}
//����
void RightMove(void)
{
	WRITE_PIN(ON, OFF, ON, OFF, OFF, ON, OFF, ON);
}
//���ֻ���
void SkipMove(void)
{
	WRITE_PIN(ON, ON, ON, ON, ON, ON, ON, ON);
}

//������ת
void LeftRotate(void)
{
	WRITE_PIN(OFF, ON, OFF, ON, OFF, ON, OFF, ON);
}

//������ת
void RightRotate(void)
{
	WRITE_PIN(ON, OFF, ON, OFF, ON, OFF, ON, OFF);
}
void StopMove(void)
{
	WRITE_PIN(OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF);
}

/*更新PWM输出*/
void updatePWM(void)
{
	static int pwm1 = 0, pwm2 = 0, pwm3 = 0, pwm4 = 0;
	static float enc1 = 0, enc2 = 0, enc3 = 0, enc4 = 0;
	static float testnumber[2];
	static int enc;
	if (DoubleBegin == 0)
	{
		enc1 = Read_Encoder(1);
		if (enc1 < 0)
			enc1 = -enc1;
		pwm1 = PID_calc(&motor1_speed, enc1, target1);
		if (pwm1 < 0)
			pwm1 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm1);

		enc2 = Read_Encoder(2);
		if (enc2 < 0)
			enc2 = -enc2;
		pwm2 = PID_calc(&motor2_speed, enc2, target2);
		if (pwm2 < 0)
			pwm2 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm2);

		enc3 = Read_Encoder(3);
		if (enc3 < 0)
			enc3 = -enc3;
		pwm3 = PID_calc(&motor3_speed, enc3, target3);
		if (pwm3 < 0)
			pwm3 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm3);

		enc4 = Read_Encoder(8);
		if (enc4 < 0)
			enc4 = -enc4;
		pwm4 = PID_calc(&motor4_speed, enc4, target4);
		if (pwm4 < 0)
			pwm4 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwm4);

		//������PID���ߵ���
		static uint8_t tempData[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0x7F};
		testnumber[0] = enc4;
		testnumber[1] = 100.0;
		memcpy(tempData, (uint8_t *)&testnumber, sizeof(testnumber));
		HAL_UART_Transmit(&huart3, (uint8_t *)tempData, 12, 0x000A);
	}
	else if (DoubleBegin == 1)
	{
		if (Again == 1)
		{
			enc1 = enc2 = enc3 = enc4 = 0;
			Again = 0;
		}
		enc = Read_Encoder(1);
		if (enc < 0)
			enc = -enc;
		enc1 = enc1 + enc;
		pwm1 = PID_calc(&motor1_position, enc1, target1); //λ�û�
		pwm1 = PID_calc(&motor1_speed, enc, pwm1);		  //�ٶȻ�
		if (pwm1 < 0)
			pwm1 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm1);

		enc = Read_Encoder(2);
		if (enc < 0)
			enc = -enc;
		enc2 = enc2 + enc;
		pwm2 = PID_calc(&motor2_position, enc2, target2); //λ�û�
		pwm2 = PID_calc(&motor2_speed, enc, pwm2);		  //�ٶȻ�
		if (pwm2 < 0)
			pwm2 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm2);

		enc = Read_Encoder(3);
		if (enc < 0)
			enc = -enc;
		enc3 = enc3 + enc;
		pwm3 = PID_calc(&motor3_position, enc3, target3); //λ�û�
		pwm3 = PID_calc(&motor3_speed, enc, pwm3);		  //�ٶȻ�
		if (pwm3 < 0)
			pwm3 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm3);

		enc = Read_Encoder(8);
		if (enc < 0)
			enc = -enc;
		enc4 = enc4 + enc;
		pwm4 = PID_calc(&motor4_position, enc4, target4); //λ�û�
		pwm4 = PID_calc(&motor4_speed, enc, pwm4);		  //�ٶȻ�
		if (pwm4 < 0)
			pwm4 = 0;
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwm4);
	}
}

void SetFourPWM(int a, int b, int c, int d)
{
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, a);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, b);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, c);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, d);
}
void SetTarget(int a, int b, int c, int d)
{
	target1 = a;
	target2 = b;
	target3 = c;
	target4 = d;
}

void motorMove(direction_t newdir, uint16_t newspeed)
{
	if (newdir == dirFront)
	{
		FrontMove();
	}
	if (newdir == dirBack)
	{
		BackMove();
	}
	if (newdir == dirLeft)
	{
		LeftMove();
	}
	if (newdir == dirRight)
	{
		RightMove();		
	}
	SetTarget(newspeed, newspeed, newspeed, newspeed);
}

/*edited by Swayee on 2022.10.17
	TODO: 优化上下限的设置和换算算法，线性关系在小量时是精确的
*/

inline int calcSlowerSpeed(uint16_t speed, float deflectVal){
	/*线性变换*/
	static const float lowBound=0.6;
	return MAX((1-deflectVal),lowBound)*speed;
}

inline int calcFasterSpeed(uint16_t speed, float deflectVal){
	/*线性变换*/
	static const float upperBound=1.4;
	return MIN((1+deflectVal),upperBound)*speed;
}

void motorTrim(direction_t newdir1,direction_t newdir2, uint16_t newspeed, float deflectVal)
{

	int slowerSpeed=calcSlowerSpeed(newspeed,deflectVal);
	int fasterSpeed=calcFasterSpeed(newspeed,deflectVal);
    if(newdir1==dirFront)
	{
		FrontMove();	
		if (newdir2 == dirLeft) SetTarget(slowerSpeed, slowerSpeed, fasterSpeed, fasterSpeed);			
		if (newdir2 == dirRight) SetTarget(fasterSpeed, fasterSpeed, slowerSpeed, slowerSpeed);			
	}
	if(newdir1==dirBack)
	{
		BackMove();	
		if (newdir2 == dirLeft)  SetTarget(fasterSpeed, fasterSpeed, slowerSpeed, slowerSpeed);		
		if (newdir2 == dirRight)  SetTarget(slowerSpeed, slowerSpeed, fasterSpeed, fasterSpeed);		
	}
    if(newdir1==dirRight)
	{
		RightMove();	
		if (newdir2 == dirLeft) SetTarget(slowerSpeed, fasterSpeed , slowerSpeed, fasterSpeed);			
		if (newdir2 == dirRight) SetTarget(fasterSpeed, slowerSpeed, fasterSpeed, slowerSpeed);			
	}
    if(newdir1==dirLeft)
	{
		LeftMove();	
		if (newdir2 == dirLeft) SetTarget(fasterSpeed, slowerSpeed, fasterSpeed, slowerSpeed);				
		if (newdir2 == dirRight) SetTarget(slowerSpeed, fasterSpeed , slowerSpeed, fasterSpeed);		
	}
}

void motorRotate(direction_t newdir, uint16_t newspeed)
{
/*edit by swayee 10.13, replace so-call rotate180degree()*/
	PID_Value_Init();
	motorStop();
	HAL_Delay(500);
	if (newdir == dirLeft)
	{
		LeftRotate();
	}
	if (newdir == dirRight)
	{
		RightRotate();
	}
	SetFourPWM(0,0,0,0);
	SetTarget(newspeed, newspeed, newspeed, newspeed);
	/*6700ms 180degree rotate*/

}

void motorStop(void)
{
	SetFourPWM(0, 0, 0, 0);
	SetTarget(0, 0, 0, 0);
	StopMove();
}

void PushCurlingTrimBack(direction_t newdir)
{
	PID_Value_Init();
	motorStop();
	HAL_Delay(500);
	if(newdir==dirLeft) WRITE_PIN(OFF, ON, OFF, OFF, OFF, ON, OFF, OFF);				
	if(newdir==dirRight) WRITE_PIN(OFF, OFF, ON, OFF, OFF, OFF, ON, OFF);
	SetFourPWM(0,0,0,0);
	SetTarget(100,100,100,100);
	if(newdir==dirLeft) HAL_Delay(300);
	else HAL_Delay(400);
	//还需测试
	motorStop();
}
void PushCurlingTrimFront(direction_t newdir)
{
	PID_Value_Init();
	motorStop();
	HAL_Delay(500);
	if(newdir==dirLeft) WRITE_PIN(ON, OFF, OFF, OFF, ON, OFF, OFF, OFF);				
	if(newdir==dirRight) WRITE_PIN(OFF, OFF, OFF, ON, OFF, OFF, OFF, ON);
	SetFourPWM(0,0,0,0);
	SetTarget(100,100,100,100);
	if(newdir==dirLeft) HAL_Delay(350);
	else HAL_Delay(450);
	//还需测试
	motorStop();
}		
