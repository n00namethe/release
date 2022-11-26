#include <stdio.h>
#define M 3

int main()
{
	int c[M][M]={};
	int x=0;

	printf("Размер массива = %zd\n", sizeof(c)/sizeof(int));

	for (int i=0; i<M; ++i)
	{
		for (int j=0; j<M; ++j)
		{
			c[i][j]=++x;
			printf("%d", c[i][j]);
		}
		printf("\n");
	}
	
		
return 0;

}


	
