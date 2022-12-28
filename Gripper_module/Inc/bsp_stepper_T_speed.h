#ifndef __BSP_STEPPER_T_SPEED_H
#define	__BSP_STEPPER_T_SPEED_H

#include "stm32f1xx.h"
#include "bsp_stepper_init.h"
#include "math.h"

/*���μӼ�����ر���*/
typedef struct 
{
	/*��ǰ���״̬*/
	uint8_t  run_state ; 
	/*��ת����*/
	uint8_t  dir ;    
	/*������*/
	int  step_delay;  
	/*����λ��*/
	int  decel_start; 
	/*���ٲ���*/
	int  decel_val;   
	/*��С���*/
	int  min_delay;   
	/*���ٲ���*/
	int  accel_count; 
}speedRampData;

/*/ϵͳ״̬*/
struct GLOBAL_FLAGS 
{
	//�����������������ʱ��ֵΪ1
	unsigned char running:1;
	//�����ڽ��յ�����ʱ��ֵΪ1
	unsigned char cmd:1;
	//���������������ʱ,ֵΪ1
	unsigned char out_ena:1;
};

#define ACCEL_R(x)	ceil(x)//����ȡ��
#define DECEL_R(x)	floor(x)//����ȡ��

#define FALSE             0
#define TRUE              1


/*����ٶȾ����е��ĸ�״̬*/
#define STOP              0 // ֹͣ״̬
#define ACCEL             1 // ����״̬
#define DECEL             2 // ����״̬
#define RUN               3 // ����״̬

/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/(TIM_PRESCALER+1)
//���� �߼���ʱ���� Ƶ��Ϊ168MHz,������ʱ��Ϊ84MHz
//168/(5+1)=28Mhz
//������Ҫ��Ƶ�ʿ����Լ�����
#define TIM_PRESCALER      1 
#define T1_FREQ           (SystemCoreClock/(TIM_PRESCALER+1)) // Ƶ��ftֵ


/*�����Ȧ����*/
#define STEP_ANGLE				1.8									//��������Ĳ���� ��λ����
#define FSPR              (360.0/1.8)         //���������һȦ����������

#define MICRO_STEP        32          				//ϸ����ϸ���� 
#define SPR               (FSPR*MICRO_STEP)   //ϸ�ֺ�һȦ����������

/*��ѧ����,���ڼ򻯼���*/

#define ALPHA             ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10           ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148       ((float)((T1_FREQ*0.676)/10)) // 0.69Ϊ�������ֵ(������̣��ĵ�����д)
#define A_SQ              ((float)(2*100000*ALPHA)) 
#define A_x200            ((float)(200*ALPHA))

extern void speed_decision(void);

#endif
