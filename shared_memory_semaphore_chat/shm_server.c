#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include "struct_and_define.h"

#define WAIT_MESSAGE_TIME 5
#define NUMBER_OF_USERS 10

typedef struct _client_t
{
    pid_t pid;
    char nickname[NICKNAME_SIZE];
    char client_queue[CLIENT_QUEUE_SIZE];
    mqd_t chat_queue;
} client_t;
client_t users_db[NUMBER_OF_USERS] = {0};

typedef struct _server_context_t
{
    client_to_server_msg_t *client2server;
    sem_t *sem_service;
    int id_shm_serv;
} server_context_t;
server_context_t server_ctx = {0};

int semaphore_value()
{
    int sval;
    sem_getvalue(server_ctx.sem_service, &sval);
    return sval;
}

void semaphore_post(const char *asker_func_name)
{
    printf("%s делает sem_post\n", asker_func_name);
    printf("значение семафора до %s = %d\n", asker_func_name, semaphore_value());
    sem_post(server_ctx.sem_service);
}

void semaphore_wait(const char *asker_func_name)
{
    printf("%s делает sem_wait\n", asker_func_name);
    printf("значение семафора до %s = %d\n", asker_func_name, semaphore_value());
    sem_wait(server_ctx.sem_service);
}

void my_printf(const char *asker_func_name)
{
    printf("я зашел в %s\n", asker_func_name);
}

void memory_clear()
{
    memset(server_ctx.client2server, 0, sizeof(client_to_server_msg_t));
    server_ctx.client2server->action = C2S_ACTION_INVALID_TYPE;
}

void create_shm_service()
{
    my_printf(__FUNCTION__);
    server_ctx.id_shm_serv = shm_open(SERVICE_SHM, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (server_ctx.id_shm_serv == -1)
    {
        printf("ошибка открытия shm: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (ftruncate(server_ctx.id_shm_serv, sizeof(client_to_server_msg_t)) == -1)
    {
        printf("ошибка ftruncate server_ctx.id_shm_serv: %d\n", errno);
        close(server_ctx.id_shm_serv);
        exit(EXIT_FAILURE);
    }
    server_ctx.client2server = mmap(NULL, sizeof(client_to_server_msg_t), PROT_READ | PROT_WRITE, MAP_SHARED, server_ctx.id_shm_serv, 0);
    if (server_ctx.client2server == MAP_FAILED)
    {
        printf("ошибка mmap: %d\n", errno);
        close(server_ctx.id_shm_serv);
        exit(EXIT_FAILURE);
    }
    memory_clear();
    server_ctx.sem_service = sem_open(SEM, O_RDWR | O_CREAT, S_IRWXU, 0);
    if (server_ctx.sem_service == SEM_FAILED)
    {
        perror("sem1 open not success\n");
        close(server_ctx.id_shm_serv);
        exit(EXIT_FAILURE);
    }
}

int user_connect()
{
    my_printf(__FUNCTION__);
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        if (i == NUMBER_OF_USERS - 1)
        {
            printf("Свободных мест нет, количество пользователей = %d\n", NUMBER_OF_USERS);
            memory_clear();
            break;
        }
        else if (users_db[i].pid == 0)
        {
            users_db[i].pid = server_ctx.client2server->client_id.client_pid;
            strncpy(users_db[i].nickname, server_ctx.client2server->client_id.client_name, sizeof(server_ctx.client2server->client_id.client_name));
            printf("user[%d].pid = %d\n", i, users_db[i].pid);
            printf("user[%d].nickname = %s\n", i, users_db[i].nickname);
            sprintf(users_db[i].client_queue, "/User_queue_%d", users_db[i].pid);

            printf("users_db[%d].client_queue = %s\n", i, users_db[i].client_queue);

            users_db[i].chat_queue = mq_open(users_db[i].client_queue, O_CREAT | O_NONBLOCK | O_RDWR, S_IRUSR | S_IWUSR, &attributes_from_server_client);
            if (users_db[i].chat_queue == -1)
            {
                printf("error = %d\n", errno);
                return 1;
            }
            printf("chat_queue[%d] создан, FD = %d\n", i, (int)users_db[i].chat_queue);
            memory_clear();
            break;
        }
    }
    return 0;
}

int user_disconnect()
{
    my_printf(__FUNCTION__);
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        if (users_db[i].pid == server_ctx.client2server->client_id.client_pid)
        {
            users_db[i].pid = 0;
            strncpy(users_db[i].nickname, "0", sizeof(users_db[i].nickname));
            if (mq_close((mqd_t)users_db[i].chat_queue) == -1)
            {
                printf("Не получилось закрыть очередь chat_queue[%d]. Errno = %d\n", i, errno);
            }
            if (mq_unlink(users_db[i].client_queue) == -1)
            {
                printf("Не получилось unlink очередь chat_queue[%d]. Errno = %d\n", i, errno);
            }
            memory_clear();
            break;
        }
    }
    int count_activ_user = 0;
    printf("список пользователей после удаления:\n");
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        if (users_db[i].pid != 0)
        {
            printf("%d: %d %s\n", i, users_db[i].pid, users_db[i].nickname);
            count_activ_user++;
        }
    }
    printf("Activ user count = %d\n", count_activ_user);
    if (count_activ_user == 0)
    {
        return 1;
    }
    memory_clear();
    return 0;
}

void send_message_to_client()
{
    my_printf(__FUNCTION__);
    server_to_client_msg_t message_send;
    strncpy(message_send.server_to_client_msg, server_ctx.client2server->client_to_server_msg, sizeof(server_ctx.client2server->client_to_server_msg));
    strncpy(message_send.client_id.client_name, server_ctx.client2server->client_id.client_name, sizeof(server_ctx.client2server->client_id.client_name));
    message_send.client_id.client_pid = server_ctx.client2server->client_id.client_pid;
    for(int i = 0; i < NUMBER_OF_USERS - 1; i++)
    {
        if (users_db[i].pid != 0)
        {
            if (mq_send(users_db[i].chat_queue, (char *)&message_send, sizeof(message_send), PRIORITY_OF_QUEUE) == -1)
            {
                printf("отправить сообщение пользователю users_db[%d].client_queue не удалось. Errno = %d\n", i, errno);
                break;
            }
        }
    }
    memory_clear();
}

int receive_service_struct()
{
    my_printf(__FUNCTION__);
    while (1)
    {
        semaphore_post(__FUNCTION__);
        /*printf("мой PID = %d\nникнейм = %s\nномер действия = %d\n",
               server_ctx.client2server->client_id.client_pid, server_ctx.client2server->client_id.client_name, server_ctx.client2server->action);*/
        switch (server_ctx.client2server->action)
        {
            case C2S_ACTION_INVALID_TYPE:
            {
                sleep(2);
                semaphore_wait(__FUNCTION__);
                continue;
            }
            case C2S_ACTION_CONNECT:
            {
                printf("case C2S_ACTION_CONNECT\n");
                semaphore_wait(__FUNCTION__);
                if (user_connect() == 1)
                {
                    return 0;
                }
                break;
            }
            case C2S_ACTION_DISCONNECT:
            {
                printf("case C2S_ACTION_DISCONNECT\n");
                semaphore_wait(__FUNCTION__);
                if (user_disconnect() == 1)
                {
                    return 0;
                }
                break;
            }
            case C2S_ACTION_MESSAGE:
            { 
                printf("case C2S_ACTION_MESSAGE\n");
                semaphore_wait(__FUNCTION__);
                send_message_to_client();
                break;
            }
            default:
            {
                printf("Получен неопознанный тип события. %d\n", server_ctx.client2server->action);
                semaphore_wait(__FUNCTION__);
                memory_clear();
                semaphore_post(__FUNCTION__);
                break;
            }
        }
    }
    return 0;
}

int main()
{
    create_shm_service();
    receive_service_struct();
    if (close(server_ctx.id_shm_serv) == -1)
    {
        perror("close shm_serv not success\n");
        return -1;
    }
    if (shm_unlink(SERVICE_SHM) == -1)
    {
        perror("shm_unlink SERVICE_SHM not success\n");
        return -1;
    }
    munmap(server_ctx.client2server, sizeof(client_to_server_msg_t));
    sem_close(server_ctx.sem_service);
    sem_unlink(SEM);
    shm_unlink(SERVICE_SHM);

    return 0;
}