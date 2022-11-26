/**
 * @file pid.c
 * @author stubborncake
 * @brief PID算法相关函数
 * 
 * DO NOT TRY TO UNDERSTAND IT, FEEL IT.
 * 
 * @version 1.0
 * @date 2022-10-06
 * @see https://github.com/stubborncake/RobogameMoveCode
 * @copyright Copyright (c) 2022
 * @warning 由于多次utf8-gbk之间转码的问题，原先的注释全部变成了乱码，且无法转回。故谨慎修改！
 */
#include "pid.h"
#include "common.h"
#include "motor.h"

pid_type_def motor1_speed, motor1_position;
pid_type_def motor2_speed, motor2_position;
pid_type_def motor3_speed, motor3_position;
pid_type_def motor4_speed, motor4_position;

float Kp_position1, Ki_position1, Kd_position1;
float Kp_position2, Ki_position2, Kd_position2;
float Kp_position3, Ki_position3, Kd_position3;
float Kp_position4, Ki_position4, Kd_position4;
float Kp_speed1, Ki_speed1, Kd_speed1;
float Kp_speed2, Ki_speed2, Kd_speed2;
float Kp_speed3, Ki_speed3, Kd_speed3;
float Kp_speed4, Ki_speed4, Kd_speed4;

#define LimitMax(input, max) \
  {                          \
    if (input > max)         \
    {                        \
      input = max;           \
    }                        \
    else if (input < -max)   \
    {                        \
      input = -max;          \
    }                        \
  }

void PID_init(pid_type_def *pid, int mode, int max_out, int max_iout, float Kp, float Ki, float Kd)
{
  if (pid == NULL)
  {
    return;
  }
  pid->mode = mode;
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
  pid->max_out = max_out;
  pid->max_iout = max_iout;
  pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0;
  pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0;
}

int PID_calc(pid_type_def *pid, int ref, int set)
{
  if (pid == NULL)
  {
    return 0;
  }
  pid->error[2] = pid->error[1];
  pid->error[1] = pid->error[0];
  pid->set = set;
  pid->fdb = ref;
  pid->error[0] = set - ref;
  //位置PID
  if (pid->mode == 1)
  {
    pid->Pout = pid->Kp * pid->error[0];
    pid->Iout += pid->Ki * pid->error[0];
    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
    pid->Dout = pid->Kd * pid->Dbuf[0];
    LimitMax(pid->Iout, pid->max_iout);
    pid->out = pid->Pout + pid->Iout + pid->Dout;
    LimitMax(pid->out, pid->max_out);
  }
  //速度PID
  else if (pid->mode == 2)
  {
    pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
    pid->Iout = pid->Ki * pid->error[0];
    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
    pid->Dout = pid->Kd * pid->Dbuf[0];
    pid->out += pid->Pout + pid->Iout + pid->Dout;
    LimitMax(pid->out, pid->max_out);
  }
  return pid->out;
}

void PID_clear(pid_type_def *pid)
{
  if (pid == NULL)
  {
    return;
  }
  pid->error[0] = pid->error[1] = pid->error[2] = 0;
  pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0;
  pid->out = pid->Pout = pid->Iout = pid->Dout = 0;
  pid->fdb = pid->set = 0;
}

#define PID_SPEED_INIT(__order)                                                                                \
  do                                                                                                           \
  {                                                                                                            \
    PID_init(&motor##__order##_speed, 2, 3600, 2000, Kp_speed##__order, Ki_speed##__order, Kd_speed##__order); \
  } while (0)

#define PID_POSITION_INIT(__order)                                                                                         \
  do                                                                                                                       \
  {                                                                                                                        \
    PID_init(&motor##__order##_position, 1, 3600, 2000, Kp_position##__order, Ki_position##__order, Kd_position##__order); \
  } while (0)

void PID_Value_Init()
{
  //速度PID参数赋值
  Kp_speed1 = Kp_speed2 = Kp_speed3 = Kp_speed4 = 12;
  Ki_speed1 = Ki_speed2 = Ki_speed3 = Ki_speed4 = 0.9;
  Kd_speed1 = Kd_speed2 = Kd_speed3 = Kd_speed4 = 0;
  //位置PID参数赋值
  Kp_position1 = 20;
  Ki_position1 = 0;
  Kd_position1 = 0;
  Kp_position2 = 20;
  Ki_position2 = 0;
  Kd_position2 = 0;
  Kp_position3 = 20;
  Ki_position3 = 0;
  Kd_position3 = 0;
  Kp_position4 = 20;
  Ki_position4 = 0;
  Kd_position4 = 0;

  PID_SPEED_INIT(1);
  PID_SPEED_INIT(2);
  PID_SPEED_INIT(3);
  PID_SPEED_INIT(4);

  PID_POSITION_INIT(1);
  PID_POSITION_INIT(2);
  PID_POSITION_INIT(3);
  PID_POSITION_INIT(4);

  Read_Encoder(1);
  Read_Encoder(2);
  Read_Encoder(3);
  Read_Encoder(8);
}
