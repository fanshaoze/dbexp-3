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
	1.���̹鲢������A��ֵΪ�ؼ���
	1.1 �Ը�����ֱ�鲢��д�ش���
	1.2
	2.����select�����飬Ūһ��int����ո�д�����blk��Aֵ����ʼ��Ϊ0����ÿ�������blkд֮ǰ�ж�һ�£����ظ���д
	3.��blkw����д����
	*/
	sortinit();
	res_of_mergesort = mergeAll();
	printf("�鲢������������%d��\n", res_of_mergesort);

	//����������ⲿ���򣬿�ʼ��ͶӰ
	addr = res_of_mergesort;
	//�ȰѴ�����֮ǰ��project�������ɾ��
	for (i = 0; i<16; i++)
	{
		if (dropBlockOnDisk(waddr + i) == -1)
			break;
	}
	/*
	if( (fo=fopen("OUTwhenProject.txt","w") )==NULL )
	{
	printf("�򲻿��ļ�whenProject.txt\n");
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

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//Ҫһ�黺�����飬�����ݴ�Ҫд�ش��̵Ŀ�
	while (addr != 0)//����һ����
	{
		for (i = 0; i<7; i++)//�������е�Ԫ��
		{//�����ȡ���еĵ���Ԫ��
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

	//�����������������Ҳ���������Ҫ�����һ�λ������������
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

	//�����һ��ȡ�أ��޸�����һ����ĵ�ַΪ0����ʾ��һ�ν�����
	if ((blkr = (unsigned int *)readBlockFromDisk(waddr - 1, &buf)) == NULL)
	{
		perror("project, Reading Block Failed!\n");
	}
	*(blkr + 15) = 0;
	writeBlockToDisk((unsigned char *)blkr, waddr - 1, &buf);

	freeBuffer(&buf);
	//deleteTempBlks();
	return 12000;
}