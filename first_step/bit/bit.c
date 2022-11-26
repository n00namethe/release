#include <stdio.h>

int main()
{
	//заменить CC на DD

	int a = 0xCCAABB;
	int x;
	x = a|0xdd0000;

	printf("%x\n%x\n", a, x);
	printf("\n");

	//еще вариант

	int d = 0xDD0000;
	x = a<<16;
	x = a>>8;
	x = a|d;

	printf("0x%x\n", x);
	printf("\n");

	//Вывести в цикле по байтам число
	int q = 0x00CCAABB; 
	unsigned char w;

	for (int i=0; i<=24; i+=8)
	{
		w=((q>>i)&0xFF);

		printf("0x%x\n", w);

	}
return 0;
}