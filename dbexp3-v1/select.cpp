#include "stdafx.h"
#include "select.h"
int linearselect(int addr, int val)
{
	Buffer buf;
	unsigned int *blkr = NULL, *blkw = NULL;
	int j = 0, i = 0;
	int waddr = 1200;
	FILE *fo;
	//�ȰѴ�����֮ǰ��select�������ɾ��
	for (i = 0; i < 48; i++)
	{
		if (dropBlockOnDisk(waddr + i * 100) == -1)
			break;
	}

	if ((fo = fopen("OUTwhenSelect.txt", "wb")) == NULL)
	{
		printf("�򲻿��ļ�whenSelect.txt\n");
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

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//Ҫһ�黺�����飬�����ݴ�Ҫд�ش��̵Ŀ�

	while (addr != 0)//����һ����
	{
		fprintf(fo, "�����ǵ�%d��\n", addr);
		for (i = 0; i < 7; i++)//�������е�Ԫ��
		{//�����ȡ���еĵ���Ԫ��
			fprintf(fo, "A %d, B %d\n", *(blkr + 2 * i), *(blkr + 2 * i + 1));
			if (*(blkr + 2 * i) == val)
			{
				//�ѵ�ǰ�Ĳ�ѯ���ĸ��Ƶ�����Ļ������
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
					//blkw = (unsigned int *)getNewBlockInBuffer(&buf);//Ҫһ�黺�����飬�����ݴ�Ҫд�ش��̵Ŀ�
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
	//����ж�һ���Ƿ�����������д���飬��û�У���дһ�Ρ���Ϊ���δд����˵��ѡ�񵽵ļ�¼����7��������һ��blk��
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
