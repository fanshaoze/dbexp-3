#include "stdafx.h"
#include "select.h"
int linearselect(int addr, int val)
{
	Buffer buf;
	unsigned int *blkr = NULL, *blkw = NULL;
	int j = 0, i = 0;
	int waddr = 1200;
	FILE *fo;
	//先把磁盘上之前的select结果块们删掉
	for (i = 0; i < 48; i++)
	{
		if (dropBlockOnDisk(waddr + i * 100) == -1)
			break;
	}

	if ((fo = fopen("OUTwhenSelect.txt", "wb")) == NULL)
	{
		printf("打不开文件whenSelect.txt\n");
		return 0;
	}

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	//----------------------------------------------
	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块

	while (addr != 0)//读下一个块
	{
		fprintf(fo, "现在是第%d块\n", addr);
		for (i = 0; i < 7; i++)//遍历块中的元组
		{//处理读取块中的单个元组
			fprintf(fo, "A %d, B %d\n", *(blkr + 2 * i), *(blkr + 2 * i + 1));
			if (*(blkr + 2 * i) == val)
			{
				//把当前的查询到的复制到申请的缓存块中
				*(blkw + j) = *(blkr + 2 * i);
				*(blkw + j + 1) = *(blkr + 2 * i + 1);
				j = (j + 2) % 16;
				fprintf(fo, "^\n");

				if (j == 0)
				{
					*(blkw + 15) = waddr;
					waddr += 100;
					writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
					//freeBlockInBuffer((unsigned char *)blkw,&buf);
					//blkw = (unsigned int *)getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块
				}
			}
		}

		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		printf("SELECT next: %d\n", addr);
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
				return -1;
			}
		}
	}
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	//最后判断一下是否有往磁盘上写过块，若没有，则写一次。因为如果未写过，说明选择到的记录不到7条（不满一个blk）
	if (j != 0)
	{
		writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
	}
	fclose(fo);
	freeBuffer(&buf);
	return waddr;
}
int binaryselect(int addr, int val)
{
	return 0;
}
