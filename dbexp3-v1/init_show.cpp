#include "init_show.h"
#include "stdafx.h"
#include <time.h>

int initdata()
{
	int i = 0;
	int j = 0;
	int addr = 0;
	Buffer *buf = new  Buffer;
	unsigned int* blk;

	FILE *fp = NULL;
	fp = fopen("init.txt", "wb");

	srand((unsigned int)time(0));
	if (initBuffer(520, 64, buf))
	{
		cout<<"Buffer Initialization Failed!"<<endl;
		return -1;
	}
	for (j = 0; j < 16; j++)//init r
	{
		blk = (unsigned int *)getNewBlockInBuffer(buf);
		/* Fill data into the block */
		for (i = 0; i < 7; i++)//init r
		{
			*(blk + 2 * i) = rand() % 40 + 1;//A
			cout << *(blk + 2 * i) << endl;
			*(blk + 2 * i + 1) = rand() % 1000 + 1;//B
			cout << *(blk + 2 * i + 1) << endl;
		}
		*(blk + 15) = addr;
		addr++;
		cout << "THIS--" << addr << endl;
		fprintf(fp, "第%d个块这是:\n", addr);

		//遍历输出一下
		for (i = 0; i < 8; i++)
		{
			cout << "第" << i + 1 << "个元组：" << *(blk + i * 2) << "\t" << *(blk + i * 2 + 1) << endl;
			//printf("第%d个元组：%d\t%d\n", i + 1, *(blk + i * 2), *(blk + i * 2 + 1));
			fprintf(fp, "第%d个元组：%d\t%d\n", i + 1, *(blk + i * 2), *(blk + i * 2 + 1));
		}
		/* Write the block to the hard disk */
		if (writeBlockToDisk((unsigned char *)blk, addr, buf) != 0)
		{
			cout << "Writing Block Failed!" << endl;
			//perror("Writing Block Failed!\n");
			return -1;
		}
		cout << "现在写到了第" << addr << "号磁盘快" << endl;
		//printf("现在写到了第%d号磁盘快\n", addr);
		freeBlockInBuffer((unsigned char *)blk, buf);
	}
}
int showBlks(int addr)
{
	Buffer buf;
	unsigned int *blkr = NULL;
	int i = 0;
	FILE *fout;

	if ((fout = fopen("OUT.txt", "wb")) == NULL)
	{
		cout << "打不开文件out.txt" << endl;
		return 0;
	}

	if (!initBuffer(520, 64, &buf))
	{
		cout << "Buffer Initialization Failed!" << endl;
		return -1;
	}

	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		cout << "Reading Block Failed!" << endl;
		return -1;
	}

	while (addr != 0)//读下一个块
	{
		fprintf(fout, "现在是第%d块\n", addr);
		for (i = 0; i < 7; i++)//遍历块中的元组
		{//处理读取块中的单个元组
			fprintf(fout, "A %d, B %d\n", *(blkr + 2 * i), *(blkr + 2 * i + 1));
		}
		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		cout << "SHOW next: " << addr << endl;
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				cout << "Reading Block Failed!" << endl;
				break;
			}
		}
	}
	fclose(fout);
	freeBuffer(&buf);
}

int showBlksOfNLJ(int addr)
{
	Buffer buf;
	unsigned int *blkr = NULL;
	int i = 0;
	FILE *fout;

	if ((fout = fopen("OUT.txt", "wb")) == NULL)
	{
		cout << "打不开文件out.txt" << endl;
		return 0;
	}

	if (!initBuffer(520, 64, &buf))
	{
		cout << "Buffer Initialization Failed!" << endl;
		return -1;
	}

	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		cout << "Reading Block Failed!" << endl;
		return -1;
	}
	while (addr != 0)//读下一个块
	{
		fprintf(fout, "现在是第%d块\n", addr);
		for (i = 0; i < 3; i++)//遍历块中的元组
		{//处理读取块中的单个元组
			fprintf(fout, "A %d, B %d, C %d, D %d\n", *(blkr + 4 * i), *(blkr + 4 * i + 1), *(blkr + 4 * i + 2), *(blkr + 4 * i + 3));
		}

		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		cout << "SHOWNLJ next:" << addr << endl;
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				cout << "Reading Block Failed!" << endl;
				break;
			}
		}
	}
	fclose(fout);
	freeBuffer(&buf);
}