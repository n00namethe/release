#include <stdio.h>

int main()
{
	
	int a;
	a = 0x00CCAABB;
	unsigned char*p;
	p = (char*)&a;

	printf("first byte of a = %d\n", *p);
	printf("adr a = %p\n", &a);
	printf("a = %x\n", a);
	
	//Меняем второй байт на DD
	p=p+1;
	*p=0xDD;

	printf("*p = %x\n", *p);
	printf("a = %x\n", a);
	printf("second byte of a = %d\n", *p);

	return 0;
}