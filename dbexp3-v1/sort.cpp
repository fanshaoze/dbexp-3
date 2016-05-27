#include "stdafx.h"
#include "sort.h"
void swap(unsigned int *x, unsigned int *y)
{
	long long int t;
	t = *x;
	*x = *y;
	*y = t;
}

void bubbleSort(int n, unsigned int* p)//√∞≈›≈≈–Ú
{
	int i, j;
	int x, y;
	for (i = 0; i<n - 1; i++)
	{
		for (j = 0; j<n - i - 1; j++)
		{
			//k=8*j;
			x = p[j * 2];
			y = p[(j + 1) * 2];
			printf("%dÃÀ  x: %d, y: %d\n", i, x, y);
			if (x>y)
			{
				swap(&p[j * 2], &p[(j + 1) * 2]);
			}
		}
	}
}