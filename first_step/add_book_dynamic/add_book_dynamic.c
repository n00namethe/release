#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#define N 20

typedef struct 
{
	char namesurname[N];
	int  phonenum;
} book;

enum action
{
	ADD = 1,
	LIST,
	SEARCH,
	DELETE,
	EXIT,
};

int main()
{
	int M = 3;
	book*adr = NULL;
	adr = malloc(M*sizeof(book));
	if (NULL == adr)
		{
			printf("ошибка выделения памяти\n");
			return(1);
		}
	int i; //Счетчик
	int p, phs;
	
	int*phone = &p;
	strncpy(adr[0].namesurname, "igor prikhodko", N);
	adr[0].phonenum=74557;
		
	strncpy(adr[1].namesurname, "ilya trishin", N);
	adr[1].phonenum=23453;
		
	strncpy(adr[2].namesurname, "efim glazkov", N);
	adr[2].phonenum=73273;
	for (int i = 0; i < M; ++i)
	{
		printf("%s %d\n", adr[i].namesurname, adr[i].phonenum);
	}
	while (1)
	{
		printf("1. Добавить абонента;\n");
		printf("2. Список абонентов;\n");
		printf("3. Найти абонента;\n");
		printf("4. Удалить абонента;\n");
		printf("5. Выход;\n");
		int scn;
		int j; //switch
		scn = scanf("%d%*c", &j);
			if (scn != 1)
			{
				free(adr);
				printf("You died\n");
				return 0;
			}
		printf("\n");
		switch(j)
		{
				case ADD: 
					{
						
						char name[N] = {};
						++M;
						book*safe_add_memory = NULL;
						safe_add_memory = realloc(adr, sizeof(book)*M);
						if (NULL == safe_add_memory)
							{
								printf("ошибка выделения памяти");
								break;
							}
						else
							{
							adr = safe_add_memory;	
							}
						printf("Имя Фамилия:\n");
						fgets(name, N-1, stdin);
						name[strlen(name) - 1] = '\0';
						strncpy(adr[M-1].namesurname, name, N);
						printf("Номер телефона:\n");
						scn = scanf("%d%*c", phone);
						if (scn != 1)
							{
								free(adr);
								printf("You died\n");
								return 0;
							}
						adr[M-1].phonenum = *phone;
						printf("Добавлен абонент:\n%s% d\n", adr[M-1].namesurname, adr[M-1].phonenum);
						break;
						
					}
					break;
					
				case LIST: 
					{
						printf("Список абонентов:\n");					
						for (i = 0; i < M; ++i)
							{
								printf("%s %d\n", adr[i].namesurname, adr[i].phonenum);
							}
					}
					break;

				case SEARCH: 
					{
					    printf("Номер телефона\n");
						scn = scanf("%d%*c", &phs);
						if (scn != 1)
							{
								free(adr);
								printf("You died\n");
								return 0;
							}
						for (i = 0; i < M; ++i)
							{
								if (phs == adr[i].phonenum)
								{
									printf("Имя фамилия: %s\nНомер телефона: %d\n", adr[i].namesurname, adr[i].phonenum);
								}
							}		
					}

					break;

				case DELETE: 
					{
						printf("Выберете номер для удаления:\n");
						for (i = 0; i < M; ++i)
							{
								printf("%d. %s %d\n", i+1, adr[i].namesurname, adr[i].phonenum); 
							}
						int k;
						scn = scanf("%d%*c", &k);
						if (scn != 1)
							{
								free(adr);
								printf("You died\n");
								return 0;
							}
						if ((k <= 0) || (k > M))
							{
								printf("try again\n");
								break;
							}
						--k;
						memmove(&adr[k], &adr[k+1], sizeof(book)*(M-k));
						--M;
						book*safe_remove_memory = NULL;
						safe_remove_memory = realloc(adr, sizeof(book)*M);
						if (NULL == safe_remove_memory)
							{
								printf("ошибка выделения памяти\n");
								break;
							}
						else
							{
								adr = safe_remove_memory;
							}	
						printf("Абонент удален!\n");
					}
					break;

				case EXIT: 
					{
						free(adr);
						return 0;
					}

					break;

				default:
					{
						printf("Неизвестный пункт меню\n");
						break;
					}
		} 
	}
	free(adr);		
	return 0;
}