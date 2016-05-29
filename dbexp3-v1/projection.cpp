#include "stdafx.h"
#include "projection.h"

int project()
{
	int res_of_mergesort = 0;
	int val_former = 0;
	Buffer buf;
	unsigned int *blkr = NULL, *blkw = NULL;
	int i = 0, w = 0;
	int addr = 0;
	int waddr = 12000;
	FILE* fo = NULL;
	/*
	1.磁盘归并排序，以A的值为关键字
	1.1 对各个块分别归并并写回磁盘
	1.2
	2.类似select，读块，弄一个int保存刚刚写入输出blk的A值（初始化为0），每次往输出blk写之前判断一下，不重复则写
	3.当blkw满则写磁盘
	*/
	sortinit();
	res_of_mergesort = mergeAll();
	printf("归并排序结果保存在%d块\n", res_of_mergesort);

	//终于完成了外部排序，开始做投影
	addr = res_of_mergesort;
	//先把磁盘上之前的project结果块们删掉
	for (i = 0; i<16; i++)
	{
		if (dropBlockOnDisk(waddr + i) == -1)
			break;
	}
	/*
	if( (fo=fopen("OUTwhenProject.txt","w") )==NULL )
	{
	printf("打不开文件whenProject.txt\n");
	return 0;
	}
	*/
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("project, Reading Block Failed!\n");
		return -1;
	}

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块
	while (addr != 0)//读下一个块
	{
		for (i = 0; i<7; i++)//遍历块中的元组
		{//处理读取块中的单个元组
			if (val_former != *(blkr + 2 * i))
			{
				*(blkw + w) = *(blkr + 2 * i);
				val_former = *(blkw + w);
				w++;
				if (w == 14)
				{
					w = 0;
					*(blkw + 15) = waddr + 1;
					//waddr++;
					writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
					waddr++;

				}
			}
		}

		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		printf("PROJECT next: %d\n", addr);
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				perror("project, Reading Block Failed!\n");
				break;
			}
		}
	}

	//最后，如果缓冲区不空且不满，则需要把最后一次缓冲区内容输出
	if (w>0)
	{
		while (w<15)
		{
			*(blkw + w) = 0;
			w++;
		}
		writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
		waddr++;
	}

	//把最后一块取回，修改其下一个块的地址为0，表示这一段结束了
	if ((blkr = (unsigned int *)readBlockFromDisk(waddr - 1, &buf)) == NULL)
	{
		perror("project, Reading Block Failed!\n");
	}
	*(blkr + 15) = 0;
	writeBlockToDisk((unsigned char *)blkr, waddr - 1, &buf);

	freeBuffer(&buf);
	deleteTempBlks();
	return 12000;
}