#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MY_PRINTF(...) printf("[%d]", getpid());\
(printf(__VA_ARGS__))

int cancel(int arg1, int arg2)
{
	int stat = -1;
	MY_PRINTF("ребенок. Делить на 0 нельзя. Будет передано: %d\n", stat);
	return stat;
}

int complit(char const *arg1, char const *arg2)
{
	if (execl("exec", "hello", arg1, arg2, NULL) == -1)
	{
		perror("exec error!");
	}
}

int main(int argc, char const *argv[3])
{
	if (argc > 3)
	{
		printf("Превышено число аргументов");
		return 0;
	}
	int status;
	pid_t par, ch;
	par = getpid();
	ch = fork();
	if (ch == -1)
	{
		perror("error with ch");
   		exit(EXIT_FAILURE);
	}
	if (ch == 0)
	{
		MY_PRINTF(":Child PID \n");
		if ((atoi(argv[1]) == 0) || (atoi(argv[2]) == 0))
		{
			goto exit;
			int stat = cancel(atoi(argv[1]), atoi(argv[2]));
			printf("return error: %d\n", stat);
			return stat;
		}
		else
		{
			complit(argv[1], argv[2]);
		}
	}
	else
	{
		wait(&status);

		if (WIFEXITED(status) != 0)
		{
			MY_PRINTF("родитель. ребенок завершился и вернул статус: %u\n", WEXITSTATUS(status));
		}
		else
		{
			MY_PRINTF("родитель. ребенок завершился с ошибкой и вернул статус: %u\n", WEXITSTATUS(status));
		}
	}
	return 0;
}