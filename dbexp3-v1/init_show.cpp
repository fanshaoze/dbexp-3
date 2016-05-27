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
		perror("Buffer Initialization Failed!\n");
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
		printf("THIS--%d\n", addr);
		fprintf(fp, "��%d��������:\n", addr);

		//�������һ��
		for (i = 0; i < 8; i++)
		{
			printf("��%d��Ԫ�飺%d\t%d\n", i + 1, *(blk + i * 2), *(blk + i * 2 + 1));
			fprintf(fp, "��%d��Ԫ�飺%d\t%d\n", i + 1, *(blk + i * 2), *(blk + i * 2 + 1));
		}
		/* Write the block to the hard disk */
		if (writeBlockToDisk((unsigned char *)blk, addr, buf) != 0)
		{
			perror("Writing Block Failed!\n");
			return -1;
		}
		printf("����д���˵�%d�Ŵ��̿�\n", addr);

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
		printf("�򲻿��ļ�out.txt\n");
		return 0;
	}

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}

	while (addr != 0)//����һ����
	{
		fprintf(fout, "�����ǵ�%d��\n", addr);
		for (i = 0; i < 7; i++)//�������е�Ԫ��
		{//�����ȡ���еĵ���Ԫ��
			fprintf(fout, "A %d, B %d\n", *(blkr + 2 * i), *(blkr + 2 * i + 1));
		}
		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		printf("SHOW next: %d\n", addr);
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
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
		printf("�򲻿��ļ�out.txt\n");
		return 0;
	}

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}
	while (addr != 0)//����һ����
	{
		fprintf(fout, "�����ǵ�%d��\n", addr);
		for (i = 0; i < 3; i++)//�������е�Ԫ��
		{//�����ȡ���еĵ���Ԫ��
			fprintf(fout, "A %d, B %d, C %d, D %d\n", *(blkr + 4 * i), *(blkr + 4 * i + 1), *(blkr + 4 * i + 2), *(blkr + 4 * i + 3));
		}

		addr = *(blkr + 15);
		freeBlockInBuffer((unsigned char *)blkr, &buf);
		printf("SHOWNLJ next: %d\n", addr);
		if (addr != 0)
		{
			if ((blkr = (unsigned int *)readBlockFromDisk(addr, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
				break;
			}
		}
	}
	fclose(fout);
	freeBuffer(&buf);
}