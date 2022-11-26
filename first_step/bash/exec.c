#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])

{
	if (argc > 3)
	{
		printf("Превышено число аргументов");
		exit(1);
	}
	printf("argv[1] = %s\n", argv[1]);
	printf("argv[2] = %s\n", argv[2]);
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	a /= b;
	printf("return: %d\n", a);
	return 1;
}