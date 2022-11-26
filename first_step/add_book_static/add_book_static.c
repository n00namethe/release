#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>

#define N 20
#define M 5


typedef struct 
{
	char a[N]; //name, second name
	int c; 	   //phone number
} book;

int main()
{
	book adr[M]={0};
	int i; //Счетчик
	int j; //switch
	char name[N];
	int phone, phs;
	
	int*p=&phone;
	
	strncpy(adr[0].a, "igor prikhodko", N);
	adr[0].c=74557;

	strncpy(adr[1].a, "ilya trishin", N);
	adr[1].c=23453;

	strncpy(adr[2].a, "efim glazkov", N);
	adr[2].c=73273;
	for (int i=0; i<3; ++i)
	{
		printf("%s %d\n", adr[i].a, adr[i].c);
	}

	while (1)
	{

		printf("1. Добавить абонента;\n");
		printf("2. Список абонентов;\n");
		printf("3. Найти абонента;\n");
		printf("4. Удалить абонента;\n");
		printf("5. Выход;\n");
		int scn;
		scn = scanf("%d%*c", &j);
			if (scn == 0)
			{
				printf("You died\n");
				return 0;
			}
		printf("\n");

		switch(j)
			{
			
				case 1: //добавление абонента
					{
						for (i=0; i<M; ++i)
							{
								if (adr[i].c==0)
								{
								printf("Имя Фамилия:\n");
								fgets(name, N-1, stdin);
								//while ((name = getchar()) != '\n' && name != EOF);
								fflush(stdin);
								name[strlen(name) - 1] = '\0';
								strncpy(adr[i].a, name, N);
								printf("Номер телефона:\n");
								scanf("%d%*c", p);
								adr[i].c = *p;
								printf("Добавлен абонент:\n%s% d\n", adr[i].a, adr[i].c);
								break;
								}
							}

							if (i==M)
								{
									printf("Мест нет\n");
									break;
								}
						
					}
					break;
					
				case 2: //список абонентов
					{
					
						for (i=0; i<M; ++i)
							{
								if (adr[i].c!=0)
								{
								printf("%s %d\n", adr[i].a, adr[i].c);
								}
							}
					}
					break;

				case 3: //поиск абонента
					{
					    printf("Номер телефона\n");
						scanf("%d%*c", &phs);

							for (i=0; i<M; ++i)
							{
								if (phs==adr[i].c)
								{
									printf("Имя фамилия: %s\nНомер телефона: %d\n", adr[i].a, adr[i].c);
								}
							}
						break;
					}

					break;

				case 4: //удаление абонента
					{
						int k;
						printf("Выберете номер для удаления:\n");

							for (i=0; i<M; ++i)
							{
								if (adr[i].c!=0)
								{
								printf("%d. %s %d\n", i+1, adr[i].a, adr[i].c);
								} 
							}

						scanf("%d%*c", &k);
						
						if ((k <= 0)||(k>M))

							{
							printf("try again\n");
							break;
							}
						--k;
						strncpy(adr[k].a, "0", N);
						adr[k].c=0;
					}

					break;

				case 5: //Выход
					{
						return 0;
					}

					break;

				default:
					{
						printf("Ошибка\n");
						break;
					}
			}
	}		
	return 0;
}