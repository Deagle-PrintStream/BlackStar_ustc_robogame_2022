
#ifndef __GRIPPER_H
#define __GRIPPER_H

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*constant defines--------------------------------------------------------------*/

static const uint8_t queueSizeMax=20;

/*Class defines----------------------------------------------------------------*/

/*本打算引用"queue.h",但是包含后代码量剧增，所以自行写了queue的简单实现,没有启用*/
class messageQueue_t{
private:
    message_t stack[queueSizeMax];
    uint8_t head;
    uint8_t tail;
    
    /*循环队列取得当前指针的下一个位置*/
    uint8_t nextPos(uint8_t ptr)const;

public:
    messageQueue_t();
    ~messageQueue_t();

    status_t push(message_t& newMsg);
    message_t pop(void);
    status_t isEmpty(void)const;
    status_t isFull(void)const;
    void clearAll(void);

};


class commandQueue_t{
private:
    const uint32_t intervalTime;
    messageQueue_t cmdQueue;

    /*发送队列中指令*/
    status_t sendCommand(void);
    /*清空当前队列*/
    void clearAll(void);

public:
    commandQueue_t();
    ~commandQueue_t();

    /*将新的指令推入队列中*/
    status_t newCommand(message_t newMsg);
    /*检查是否可以发送下一个指令*/
    status_t check(void);
};

/*Macro export*/

extern commandQueue_t commandQueue;

__DEBUG extern HAL_StatusTypeDef 
sendCommadbyRaw(command_t newCmd, uint8_t arg1, uint8_t arg2);


#ifdef __cplusplus
}
#endif

#endif
