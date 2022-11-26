#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "struct_and_define.h"
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

#define EXIT_CHAR 'e'
#define NUMBER_OF_ARGUMENTS 2

struct sigevent sigev;

typedef struct _client_context_t
{
    client_to_server_msg_t *client2server_msg;
    sem_t *sem_service;
    user_id_t client_id;
    char client_pid_queue_for_chat[CLIENT_QUEUE_SIZE];
    mqd_t chat_queue;
} client_context_t;
client_context_t client_ctx = {0};

int id_shm_serv;

int get_semaphore_value()
{
    int sval;
    sem_getvalue(client_ctx.sem_service, &sval);
    return sval;
}

void semaphore_post(const char *asker_func_name)
{
    printf("%s делает sem_post\n", asker_func_name);
    printf("значение семафора до %s = %d\n", asker_func_name, get_semaphore_value());
    sem_post(client_ctx.sem_service);
}

void semaphore_wait(const char *asker_func_name)
{
    printf("%s делает sem_wait\n", asker_func_name);
    printf("значение семафора до %s = %d\n", asker_func_name, get_semaphore_value());
    sem_wait(client_ctx.sem_service);
}

void wait_shmem_free()
{
    while(1)
    {
        semaphore_wait(__FUNCTION__);
        if (client_ctx.client2server_msg->action != C2S_ACTION_INVALID_TYPE)
        {
            printf("память не высвобождена, иди своей дорогой\n");
            semaphore_post(__FUNCTION__);
            sleep(1);
            continue;
        }
        break;
    }
}

void here_are_am(const char *asker_func_name)
{
    printf("я зашел в %s\n", asker_func_name);
}

void check_agrc(int argc)
{
    here_are_am(__FUNCTION__);
    if (argc != NUMBER_OF_ARGUMENTS)
    {
        printf("write just your nickname\ntry again\n");
        exit(EXIT_FAILURE);
    }
}

void global_client_name_set(const char *client_name)
{
    if (NULL == client_name)
    {
        printf("client name = NULL, try again\n");
        exit(EXIT_FAILURE);
    }
    memmove(client_ctx.client_id.client_name, client_name, sizeof(client_ctx.client_id.client_name));
}

void global_client_pid_set(pid_t client_pid)
{
    here_are_am(__FUNCTION__);
    if (client_pid == 0)
    {
        printf("невалидный пид клиента = %d\n", client_pid);
        exit(EXIT_FAILURE);
    }
    client_ctx.client_id.client_pid = client_pid;
}

void wait_connect()
{
    here_are_am(__FUNCTION__);
    id_shm_serv = -1;
    while (id_shm_serv <= 0)
    {
        printf("я в цикле, жду подключения к серверу\n");
        id_shm_serv = shm_open(SERVICE_SHM, O_RDWR, S_IRUSR | S_IWUSR);
        if (id_shm_serv == -1)
        {
            perror("shm_open not success\n");
            printf("next try 3..2..1..\nerrno = %d\n", errno);
            sleep(3);
        }
    }
    if (ftruncate(id_shm_serv, sizeof(client_to_server_msg_t)) == -1)
    {
        printf("ошибка ftruncate id_shm_serv: %d\n", errno);
        close(id_shm_serv);
        exit(EXIT_FAILURE);
    }
    client_ctx.client2server_msg = mmap(NULL, sizeof(client_to_server_msg_t), PROT_WRITE, MAP_SHARED, id_shm_serv, 0);
    if (client_ctx.client2server_msg == MAP_FAILED)
    {
        printf("ошибка mmap: %d\n", errno);
        close(id_shm_serv);
        exit(EXIT_FAILURE);
    }
    client_ctx.sem_service = sem_open(SEM, O_RDWR, S_IRWXU, 0);
    if(client_ctx.sem_service == SEM_FAILED)
    {
        perror("sem1 open not success\n");
        close(id_shm_serv);
        exit(EXIT_FAILURE);
    }
    printf("Доступ к общей памяти получен успешно, shm descriptor: %d\n", id_shm_serv);
}

int connect_info()
{
    here_are_am(__FUNCTION__);
    printf("semaphore value = %d\n", get_semaphore_value());
    wait_shmem_free();
    client_ctx.client2server_msg->action = C2S_ACTION_CONNECT;
    client_ctx.client2server_msg->client_id.client_pid = client_ctx.client_id.client_pid;
    sprintf(client_ctx.client2server_msg->client_id.client_name, "%s", client_ctx.client_id.client_name);
    semaphore_post(__FUNCTION__);
    printf("мой PID = %d\nникнейм = %s\nномер действия = %d\n", \
           client_ctx.client_id.client_pid, client_ctx.client_id.client_name, client_ctx.client2server_msg->action);
    return 0;
}

void create_new_chat_descriptor()
{
    here_are_am(__FUNCTION__);
    sprintf(client_ctx.client_pid_queue_for_chat, "/User_queue_%d", client_ctx.client_id.client_pid);
    printf("user_queue = %s\n", client_ctx.client_pid_queue_for_chat);
    client_ctx.chat_queue = -1;
    while (client_ctx.chat_queue <= 0)
    {
        printf("я в цикле, жду подключения к чату\n");
        client_ctx.chat_queue = mq_open(client_ctx.client_pid_queue_for_chat, O_RDONLY);
        if (client_ctx.chat_queue == -1)
        {
            printf("error = %d\n", errno);
            printf("next try 3..2..1..\nerrno = %d\n", errno);
            sleep(3);
        }
    }
    printf("Подкдючение к очереди произошло успешно, queue descriptor: %d\n", (int)client_ctx.chat_queue);
}

void sig_receive_message()
{
    server_to_client_msg_t struct_to_receive = {0};
    mq_notify(client_ctx.chat_queue, &sigev);
    mq_receive(client_ctx.chat_queue, (char *)&struct_to_receive, sizeof(struct_to_receive), NULL);
    if (struct_to_receive.client_id.client_pid != client_ctx.client_id.client_pid)
    {
        printf("\n%s: %s", struct_to_receive.client_id.client_name, struct_to_receive.server_to_client_msg);
    }   
}

void disconnect()
{
    here_are_am(__FUNCTION__);
    while (1)
    {
        wait_shmem_free();
        client_ctx.client2server_msg->client_id.client_pid = client_ctx.client_id.client_pid;
        client_ctx.client2server_msg->action = C2S_ACTION_DISCONNECT;
        semaphore_post(__FUNCTION__);
        break;
    }
}

void send_message_to_chat()
{
    here_are_am(__FUNCTION__);
    printf("if you want exit press %c\n", EXIT_CHAR);
    do
    {
        wait_shmem_free();
        printf("Your message:\n");
        char message = 'e';
        fgets(client_ctx.client2server_msg->client_to_server_msg, MESSAGE_SIZE, stdin);
        if (!strncmp(client_ctx.client2server_msg->client_to_server_msg, &message, strlen(&message)))
        {
            printf("пользователь решил выйти\n");
            semaphore_post(__FUNCTION__);
            break;
        }
        client_ctx.client2server_msg->action = C2S_ACTION_MESSAGE;
        client_ctx.client2server_msg->client_id.client_pid = client_ctx.client_id.client_pid;
        strncpy(client_ctx.client2server_msg->client_id.client_name, client_ctx.client_id.client_name, sizeof(client_ctx.client_id.client_name));
        semaphore_post(__FUNCTION__);
    } while(1);
}

void chat()
{
    here_are_am(__FUNCTION__);
    printf("Я зашел в chat\n");
    signal(SIGUSR1, sig_receive_message);
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    mq_notify(client_ctx.chat_queue, &sigev);
    send_message_to_chat();
    disconnect();
}

int main(int argc, char *argv[])
{
    check_agrc(argc);
    global_client_name_set(argv[1]);
    global_client_pid_set(getpid());
    wait_connect();
    connect_info();
    create_new_chat_descriptor();
    chat();
    return 0;
}