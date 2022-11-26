#include <stdio.h>

int main()
{
	/*исправить через **p2 значение указателя *p,
	чтобы он указывал на B*/

	char a = 'A';
	char b = 'B';
	unsigned char*p;
	unsigned char**p2;

	p=&a;
	p2=&p;
	printf("adr p1 before = %p\n", &p);
	printf("p2 before = %c\n", **p2);

	**p2=**p2+1;

	printf("adr a = %p\n", &a);
	printf("adr b = %p\n", &b);
	printf("p2 after = %c\n", **p2);
	printf("adr p1 after = %p\n", &p);


return 0;
}