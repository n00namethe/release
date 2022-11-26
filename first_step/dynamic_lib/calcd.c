#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

enum action
{
	SUM = 1,
	SUB,
	MUL,
	DIV,
	EXIT,
};

int check_scanf(int check, int variable)
{
	if ((check != 1) || (variable < 1) || (variable > 4))
	{
		printf("Вы ввели неправильное значние\n");
		exit(1);
	}
}

int scanf1 (int variable)
{
	int check;
	check = scanf("%d%*c", &variable);
	check_scanf(check, variable);
	
	return variable;
	
}

int check_scanf2(int check)
{
	if (check != 1)
	{
		printf("Вы ввели неправильное значние\n");
		exit(1);
	}
}

int scanf2 (int arg)
{
	int check;
	check = scanf("%d%*c", &arg);
	check_scanf2(check);
	return arg;
}


int main ()
{
	int result, arg1, arg2;
	int variable;
	void*exp_library;

	while(1)
	{
		printf("Выберете действие:\n");
		printf("1. Сложить\n2. Вычесть\n3. Умножить\n4. Делить\n");
		variable = scanf1(variable);
		printf("Введите 2 аргумента:\n");
		arg1 = scanf2(arg1);
		arg2 = scanf2(arg2);
		switch (variable)
		{	
			case SUM:
			{
				int (*act_sum)(int arg1, int arg2);
				exp_library = dlopen("./lib_sum.so",RTLD_LAZY);
				if (!exp_library)
				{
					fprintf(stderr, "dlopen() error: %s\n", dlerror());
					return 1;
					break;
				}
				act_sum = dlsym(exp_library, "sum");
				if (act_sum)
				{
					printf("%d\n", (*act_sum)(arg1, arg2));
				}
				else
				{
					printf("%s\n", dlerror());
					break;
				}
				dlclose(exp_library);
				break;
			}
	
			case SUB:
			{
				int (*act_sub)(int arg1, int arg2);
				exp_library = dlopen("./lib_sub.so",RTLD_LAZY);
				if (!exp_library)
				{
					fprintf(stderr, "dlopen() error: %s\n", dlerror());
					return 1;
					break;
				}
				act_sub = dlsym(exp_library, "sub");
				if (act_sub)
				{
					printf("%d\n", (*act_sub)(arg1, arg2));
				}
				else
				{
					printf("%s\n", dlerror());
					break;
				}
				dlclose(exp_library);
				break;
			}
			
			case MUL:
			{
				int (*act_mul)(int arg1, int arg2);
				exp_library = dlopen("./lib_mul.so",RTLD_LAZY);
				if (!exp_library)
				{
					fprintf(stderr, "dlopen() error: %s\n", dlerror());
					return 1;
					break;
				}
				act_mul = dlsym(exp_library, "mul");
				if (act_mul)
				{
					printf("%d\n", (*act_mul)(arg1, arg2));
				}
				else
				{
					printf("%s\n", dlerror());
					break;
				}
				dlclose(exp_library);
				break;
			}
		
			case DIV:
			{
				int (*act_div)(int arg1, int arg2);
				exp_library = dlopen("./lib_div.so",RTLD_LAZY);
				if (!exp_library)
				{
					fprintf(stderr, "dlopen() error: %s\n", dlerror());
					return 1;
					break;
				}
				act_div = dlsym(exp_library, "div");
				if (act_div)
				{
					printf("%d\n", (*act_div)(arg1, arg2));
				}
				else
				{
					printf("%s\n", dlerror());
					break;
				}
				dlclose(exp_library);
				break;
			}

			default:
			{
				return 0;
			}
		}
	}
	return 0;
}