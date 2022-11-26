#include <stdio.h>
#define M 7

int main()
{
	int C[M]={1,2,3,4,5,6,7};
	int B[M]={};

	for (int j=0; j<M; ++j)
	{
		C[M]=j;
		printf("%d", C[j]);
	}
	printf("\n");

	for (int i=0; i<M; ++i)
	{
		B[i]=C[M-i-1];
		printf("%d", B[i]);
	}
	printf("\n");

	return 0;
}