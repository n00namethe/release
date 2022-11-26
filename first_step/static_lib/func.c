#include <stdio.h>
#include "my_func.h"

enum action
{
	SUM = 1,
	SUB,
	MUL,
	DIV,
};

int check_scanf (int check_sc, int check_variable)
{
	if ((check_sc != 1) || (check_variable < 1) || (check_variable > 4))
	{
		printf("Вы ввели неправильное значние\n");
		return 0;
	}
	
}

int check_scanf1 (int check_sc1, int check_variable1)
{
	if ((check_sc1 != 1) || (check_variable1 > 10000))
	{
		printf("Вы ввели неправильное значние\n");
		return 0;
	}
	
}

int main ()
{
	int arg1, arg2, result, check, check1;
	int variable;

	while(1)
	{
		printf("Выберете действие:\n");
		printf("1. Сложить\n2. Вычесть\n3. Умножить\n4. Делить\n");
		check = scanf("%d%*c", &variable);
		check_scanf(check, variable);
		printf("Введите 2 аргумента:\n");
		check1 = scanf("%d%*c", &arg1);
		check_scanf1(check1, arg1);
		check1 = scanf("%d%*c", &arg2);
		check_scanf1(check1, arg2);
		switch (variable)
		{	
			case SUM:
			{
				result = my_func(variable, arg1, arg2);
				printf("%d\n", result);
				break;
			}
			case SUB:
			{
				result = my_func(variable, arg1, arg2);
				printf("%d\n", result);
				break;
			}
			case MUL:
			{
				result = my_func(variable, arg1, arg2);
				printf("%d\n", result);
				break;
			}
			case DIV:
			{
				result = my_func(variable, arg1, arg2);
				printf("%d\n", result);
				break;
			}
			default:
			{
				return 0;
			}
		}
		return 0;	
	}
	return 0;
}