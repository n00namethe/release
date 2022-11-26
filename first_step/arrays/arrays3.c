#include <stdio.h>

#define N 5

int main()
{
	int i, j, x;
	i=j=0;
	int mas[N][N] = {};
	for (i=0; i<N; ++i)
	{
		for (j=0; j<N; ++j)
		{
			if (i+j+1<N)
			{
				mas[i][j]=1;
			}
			else if (i+j>=N)
			{
				mas[i][j]=0;
			}
			printf("%d", mas[i][j]);
		}
		printf("\n");
	}


return 0;

}