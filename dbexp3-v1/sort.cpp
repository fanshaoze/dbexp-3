#include "stdafx.h"
#include "sort.h"
void swap(unsigned int *x, unsigned int *y)
{
	long long int t;
	t = *x;
	*x = *y;
	*y = t;
}

void bubbleSort(int n, unsigned int* p)//冒泡排序
{
	int i, j;
	int x, y;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			//k=8*j;
			x = p[j * 2];
			y = p[(j + 1) * 2];
			printf("%d趟  x: %d, y: %d\n", i, x, y);
			if (x > y)
			{
				swap(&p[j * 2], &p[(j + 1) * 2]);
			}
		}
	}
}
int sortinit()//把R的各块分别排序，形成初始归并段
{
	Buffer buf;
	unsigned int *blk = NULL;
	int addr = Raddr;

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	if ((blk = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}

	//blkw = getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块

	while (addr != 0)//读下一个块
	{
		bubbleSort(7, blk);
		writeBlockToDisk((unsigned char *)blk, addr, &buf);
		addr = *(blk + 15);
		freeBlockInBuffer((unsigned char *)blk, &buf);
		printf("next: %d\n", addr);
		if (addr != 0)
		{
			if ((blk = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
				return -1;
			}
		}
	}
	freeBuffer(&buf);
}
int mergeSort(unsigned int addr1, int n1, unsigned int addr2, int n2, int base)
{
	//n1,n2分别表示两个归并段各自包含的blk数,base表示写到的块的基地址
	Buffer buf;
	unsigned int *blkr1 = NULL, *blkr2 = NULL, *blkw = NULL;
	int i1 = 0, i2 = 0, w = 0;
	unsigned int waddr = base + addr1;
	int x, y;

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	if ((blkr1 = (unsigned int *)readBlockFromDisk(addr1, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}
	x = *blkr1;
	//addr= *(blkr1+60);
	if ((blkr2 = (unsigned int *)readBlockFromDisk(addr2, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}
	y = *blkr2;

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块

	while (n1 > 0 || n2 > 0)
	{
		if (x < y)
		{
			*(blkw + w * 2) = *(blkr1 + i1 * 2);
			*(blkw + w * 2 + 1) = *(blkr1 + i1 * 2 + 1);
			i1++;
		}
		else
		{
			*(blkw + w * 2) = *(blkr2 + i2 * 2);
			*(blkw + w * 2 + 1) = *(blkr2 + i2 * 2 + 1);
			i2++;
		}
		w++;
		if (w == 7)
		{
			w = 0;
			*(blkw + 15) = waddr - 1;
			if (waddr == 8001) *(blkw + 15) = 0;
			writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
			printf("归并，写回到了第%d块\n", waddr);
			waddr--;
		}
		if (n1 > 0) x = *(blkr1 + i1 * 2);
		if (n2 > 0) y = *(blkr2 + i2 * 2);
		if (i1 == 7)
		{
			i1 = 0;
			addr1--;
			n1--;
			freeBlockInBuffer((unsigned char *)blkr1, &buf);
			if (addr1 != 0 && n1 > 0)
			{
				if ((blkr1 = (unsigned int *)readBlockFromDisk(addr1, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
				x = *blkr1;
			}
			else x = 2000;
		}
		if (i2 == 7)
		{
			i2 = 0;
			addr2--;
			n2--;
			freeBlockInBuffer((unsigned char *)blkr2, &buf);
			if (addr2 != 0 && n2 > 0)
			{
				if ((blkr2 = (unsigned int *)readBlockFromDisk(addr2, &buf)) == NULL)
				{
					perror("Reading Block Failed!\n");
					return -1;
				}
				y = *blkr2;
			}
			else y = 2000;
		}
	}
	freeBuffer(&buf);
}
int mergeAll()
{
	int i = 0;
	unsigned int addr1 = 2000 + Raddr, addr2 = 0, base = Raddr;
	//base+=2000;
	for (i = 0; i < 8; i++)
	{
		addr1 = base - 2 * i;
		//addr1=Raddr-2*i;
		addr2 = addr1 - 1;
		mergeSort(addr1, 1, addr2, 1, 2000);
	}
	base += 2000;
	for (i = 0; i < 4; i++)
	{
		addr1 = base - 4 * i;
		//addr1=Raddr-4*i;
		addr2 = addr1 - 2;
		mergeSort(addr1, 2, addr2, 2, 2000);
	}
	base += 2000;
	for (i = 0; i < 2; i++)
	{
		addr1 = base - 8 * i;
		//addr1=Raddr-8*i;
		addr2 = addr1 - 4;
		mergeSort(addr1, 4, addr2, 4, 2000);
	}
	base += 2000;
	addr1 = base;
	addr2 = addr1 - 8;
	mergeSort(addr1, 8, addr2, 8, 2000);

	return base + 2000;
}
int binarysort(int array[], int val)
{
	int i = 0;
	int j = sizeof(array)-1;
	while (j - i > 1)
	{
		if (array[i] > val && val > array[j])
		{
			if (val - array[i] < array[j] - val) j = (i + j) / 2 + 1;
			else i = (i + j) / 2 - 1;
		}
		else if (array[i] = val) return i;
		else if (array[j] = val) return j;
	}
	return i;
}