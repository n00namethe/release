#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parents(pid_t *pid)
{
	printf("Я родитель par \t%d, породил ребенка: \t%d\n", getpid(), *pid);
}
int child()
{
	printf("Я ребенок \t%d, мой родитель: \t\t%d\n", getpid(), getppid());
}

int main()
{
   	pid_t par, ch_1 = 1, ch_2 = 2, ch_3 = 3, ch_4 = 4, ch_5 = 5;
   	par = getpid();
	printf("Я самый главный процесс, мой PID: %d меня запустил PPID: %d\n", par, getppid());
	printf("par = %d, ch_1 = %d, ch_2 = %d, ch_3 = %d, ch_4 = %d, ch_5 = %d\n", par, ch_1, ch_2, ch_3, ch_4, ch_5);

   	ch_1 = fork();
   	if (ch_1 == -1)
   	{
   		perror("error with ch_1");
   		exit(1);
   	}
   	if (ch_1 == 0)
   	{
   		child();
   		ch_3 = fork();
   		if (ch_3 == -1)
   		{
   			perror("error with ch_3");
   			exit(1);
   		}
   		if (ch_3 == 0)
   		{
   			child();
   		}
   		else 
   		{
   			parents(&ch_3);
   			ch_4 = fork();
   			if (ch_4 == -1)
   			{
   				perror("error with ch_4");
   				exit(1);
   			}
   			if (ch_4 == 0)
   			{
   				child();
   			}
   			else
   			{
   				parents(&ch_4);
   			}
   		}
   	}
   	else 
   	{
   		parents(&ch_1);
   		ch_2 = fork();
		if (ch_2 == -1)
	   	{
	   		perror("error with ch_2");
	   		exit(1);
	   	}   		
   		if (ch_2 == 0)
   		{
   			child();
   			ch_5 = fork();
   			if (ch_5 == -1)
  		 	{
   				perror("error with ch_5");
	  		 	exit(1);
 		  	}
  		 	if (ch_5 == 0)
  		 	{
   				child();
 		  	}
  		 	else
  		 	{
  		 		parents(&ch_5);
 		  	}	
   		}
   		else
   		{
   			parents(&ch_2);
   		}
   	}
   	if (ch_1 != 0)
   	{
   		wait(NULL);
   	}
   	wait(&ch_5);
  return 0;
}
