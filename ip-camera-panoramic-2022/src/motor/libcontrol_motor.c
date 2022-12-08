#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include "motor_daemon.h"
#include "motor_daemon_api.h"

// #define DEBUG
#ifdef DEBUG
#define PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
#define PRINT_DEBUG(...)
#endif


typedef struct _control_motor_context_t
{
    mqd_t pic2motor_queue;
    mqd_t motor2pic_queue;
} control_motor_context_t;
control_motor_context_t cnt_ctx = {0};

int event_number = 0;

int send_motor2pic_request(pic2motor_t *to_send)
{
    PRINT_DEBUG(__FUNCTION__);
    event_number++;
    to_send->number_of_comand_p2m = event_number;
    if (mq_send(cnt_ctx.pic2motor_queue, (char *)to_send, sizeof(pic2motor_t), PRIORITY_OF_QUEUE) == -1)
    {
        printf("mq_send pic2motor_queue not success, errno = %d\n", errno);
        return -1;
    }
    PRINT_DEBUG("Я отправил сообщение № %d с командой %d\nКоличество шагов = %d\nРазмер отправленного сообщения: %ld\n", \
                to_send->number_of_comand_p2m, to_send->action_p2m, to_send->make_steps, sizeof(pic2motor_t));
    return 0;
}

void connect_queue_pic2motor()
{
    PRINT_DEBUG(__FUNCTION__);
    while (cnt_ctx.pic2motor_queue <= 0)
    {
        PRINT_DEBUG("я в цикле, жду подключения к серверу\n");
        cnt_ctx.pic2motor_queue = mq_open(PIC2MOTOR_QUEUE, O_WRONLY);
        if (cnt_ctx.pic2motor_queue == -1)
        {
            printf("mq_open pic2motor not success\nnext try 3..2..1..\nerrno = %d\n", errno);
            sleep(3);
        }
    }
    PRINT_DEBUG("Подкдючение к очереди pic2motor произошло успешно, queue descriptor: %d\n", (int)cnt_ctx.pic2motor_queue);
}

void connect_queue_motor2pic()
{
    PRINT_DEBUG(__FUNCTION__);
    while (cnt_ctx.motor2pic_queue <= 0)
    {
        PRINT_DEBUG("я в цикле, жду подключения к серверу\n");
        cnt_ctx.motor2pic_queue = mq_open(MOTOR2PIC_QUEUE, O_RDONLY);
        if (cnt_ctx.motor2pic_queue == -1)
        {
            printf("mq_open motor2pic not success\nnext try 3..2..1..\nerrno = %d\n", errno);
            sleep(3);
        }
    }
    PRINT_DEBUG("Подкдючение к очереди motor2pic произошло успешно, queue descriptor: %d\n", (int)cnt_ctx.motor2pic_queue);
}

void receive_motor2pic_reply(motor2pic_t *to_receive)
{
    PRINT_DEBUG(__FUNCTION__);
    while (1)
    {
        int size_receive = mq_receive(cnt_ctx.motor2pic_queue, (char *)to_receive, sizeof(motor2pic_t), NULL);
        if (size_receive != sizeof(motor2pic_t))
        {
            printf("размер полученного сообщения не соответсвует ожидаемому. errno = %d\n", errno);
            printf("повтор попытки\n");
            sleep(3);
            continue;
        }
        PRINT_DEBUG("я принял сообщение №%d от действия %d\nРазмер полученного сообщения: %d\n", \
                    to_receive->number_of_comand_m2p, to_receive->action_m2p, size_receive);
        printf("Текущее положение мотора: %d\n", to_receive->motor_status);
        break;
    }
}

void goodbye_motor()
{
    PRINT_DEBUG(__FUNCTION__);
    pic2motor_t goodbye = {0};
    goodbye.action_p2m = CAM2MOTOR_ACTION_EXIT;
    send_motor2pic_request(&goodbye);
}

int calibration()
{
    PRINT_DEBUG(__FUNCTION__);
    pic2motor_t calibration_p2m = {0};
    calibration_p2m.action_p2m = CAM2MOTOR_ACTION_CALIBRATION;
    calibration_p2m.make_steps = LIMIT_STEP;
    send_motor2pic_request(&calibration_p2m);

    motor2pic_t calibration_m2p = {0};
    receive_motor2pic_reply(&calibration_m2p);
    if (calibration_p2m.action_p2m != calibration_m2p.action_m2p)
    {
        printf("Мотор ответил не на то событие\nНомер отправленного: %d\nНомер полученного: %d\n", \
               calibration_p2m.action_p2m, calibration_m2p.action_m2p);
        return CAM2MOTOR_ACTION_INVALID_TYPE;
    }
    if (calibration_p2m.number_of_comand_p2m != calibration_m2p.number_of_comand_m2p)
    {
        printf("Мотор ответил не на то сообщение\nНомер отправленного: %d\nНомер полученного: %d\n", \
               calibration_p2m.number_of_comand_p2m, calibration_m2p.number_of_comand_m2p);
        return CAM2MOTOR_ACTION_INVALID_TYPE;
    }
    if (calibration_m2p.action_m2p == CAM2MOTOR_ACTION_INVALID_TYPE)
    {
        printf("Какие-то проблемы с калибровкой\n");
        return CAM2MOTOR_ACTION_INVALID_TYPE;
    }
    return 0;
}

int step(int number_of_steps)
{
    pic2motor_t step_p2m = {0};
    step_p2m.action_p2m = CAM2MOTOR_ACTION_STEP;
    step_p2m.make_steps = number_of_steps;
    send_motor2pic_request(&step_p2m);

    motor2pic_t step_m2p = {0};
    receive_motor2pic_reply(&step_m2p);
    if (step_m2p.action_m2p == CAM2MOTOR_ACTION_END_OF_ENUM)
    {
        printf("Мотор дошел до конца\nНомер отправленного: %d\nНомер полученного: %d\n", \
               step_p2m.action_p2m, step_m2p.action_m2p);
        return CAM2MOTOR_ACTION_END_OF_ENUM;
    }
    if (step_p2m.number_of_comand_p2m != step_m2p.number_of_comand_m2p)
    {
        printf("Мотор ответил не на то сообщение\nНомер отправленного: %d\nНомер полученного: %d\n", \
               step_p2m.number_of_comand_p2m, step_m2p.number_of_comand_m2p);
        return CAM2MOTOR_ACTION_INVALID_TYPE;
    }
    if (step_m2p.action_m2p == CAM2MOTOR_ACTION_INVALID_TYPE)
    {
        printf("Какие-то проблемы с шагом\n");
        return CAM2MOTOR_ACTION_INVALID_TYPE;
    }
    return 0;
}

void init_contol_motor()
{
    connect_queue_pic2motor();
    connect_queue_motor2pic();
}