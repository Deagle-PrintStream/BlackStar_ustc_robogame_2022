#ifndef PID_H
#define PID_H

/* Exported types ------------------------------------------------------------*/

//PIDģʽ
enum PID_MODE
{
  PID_POSITION = 0,
  PID_DELTA
};
 //PIDָ�붨��
typedef struct
{
  int mode;//1Ϊλ��PID��2Ϊ����PID
  //PID ������
  float Kp;
  float Ki;
  float Kd;
 
  int max_out;  //������
  int max_iout; //���������
 
  int set;
  int fdb;
 
  int out;
  int Pout;
	int Iout;
  int Dout;
  int Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�
  int error[3]; //����� 0���� 1��һ�� 2���ϴ�
 
} pid_type_def;
 
/* Exported macro ------------------------------------------------------------*/

extern pid_type_def motor1_speed;
extern pid_type_def motor1_position;
extern pid_type_def motor2_speed;
extern pid_type_def motor2_position;
extern pid_type_def motor3_speed;
extern pid_type_def motor3_position;
extern pid_type_def motor4_speed;
extern pid_type_def motor4_position;

extern float Kp_position1,Ki_position1,Kd_position1;
extern float Kp_position2,Ki_position2,Kd_position2;
extern float Kp_position3,Ki_position3,Kd_position3;
extern float Kp_position4,Ki_position4,Kd_position4;
extern float Kp_speed1,Ki_speed1,Kd_speed1;
extern float Kp_speed2,Ki_speed2,Kd_speed2;
extern float Kp_speed3,Ki_speed3,Kd_speed3;
extern float Kp_speed4,Ki_speed4,Kd_speed4;

/* Exported functions prototypes ---------------------------------------------*/

extern void PID_init(pid_type_def *pid, int mode,  int max_out, int max_iout,float Kp,float Ki,float Kd);//PID��ʼ��
extern int PID_calc(pid_type_def *pid, int ref, int set);//PID����
extern void PID_clear(pid_type_def *pid);//PID���
extern void PID_Value_Init(void);

#endif
