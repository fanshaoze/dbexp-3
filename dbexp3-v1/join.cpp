#include "stdafx.h"

int NLJ()//���ÿ�Ƕ��ѭ������
{
	Buffer buf;
	unsigned int *blks = NULL, *blkw = NULL;
	unsigned int *blkr[6] = { 0 };
	int w = 0, i = 0, j = 0, times_put_r_in_buf = 0, ri = 0;
	int waddr = 18000;
	int addr1 = Raddr, addr2 = Saddr;
	FILE *fo;
	if ((fo = fopen("OUTwhenJoin.txt", "w")) == NULL)
	{
		cout << "�򲻿��ļ�whenJoin.txt" << endl;
		return 0;
	}

	//�ȰѴ�����֮ǰ��NLJ�������ɾ��
	for (i = 0; 1; i++)
	{
		if (dropBlockOnDisk(waddr + i) == -1)
			break;
	}

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//Ҫһ�黺�����飬�����ݴ�Ҫд�ش��̵Ŀ�

	for (times_put_r_in_buf = 0; times_put_r_in_buf<3; times_put_r_in_buf++)
	{
		for (ri = 0; ri<6 && addr1 != 0; ri++)//��R��6���������
		{
			fprintf(fo, "����R�ǵ�%d��\n", addr1);
			if ((blkr[ri] = (unsigned int *)readBlockFromDisk(addr1, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
				break;
			}
			addr1 = *(blkr[ri] + 15);
		}
		//��S��ÿ���飬�����������е�6��R��
		addr2 = Saddr;


		while (addr2 != 0)//����һ����
		{
			if ((blks = (unsigned int *)readBlockFromDisk(addr2, &buf)) == NULL)
			{
				perror("NLJ,Reading Block Failed!\n");
				break;
			}

			fprintf(fo, "����S�ǵ�%d��\n", addr2);
			for (ri = 0; ri<6; ri++)
			{
				if (blkr[ri] == NULL) break;
				for (i = 0; i<7; i++)//�������е�Ԫ��
				{//�����ȡ���еĵ���Ԫ��
					for (j = 0; j<7; j++)
					{
						if (*(blks + 2 * j) == *(blkr[ri] + 2 * i))
						{
							*(blkw + 4 * w) = *(blkr[ri] + 2 * i);
							*(blkw + 4 * w + 1) = *(blkr[ri] + 2 * i + 1);
							*(blkw + 4 * w + 2) = *(blks + 2 * j);
							*(blkw + 4 * w + 3) = *(blks + 2 * j + 1);
							fprintf(fo, "��A %d, B %d, C %d, D %d\n", *(blkr[ri] + 2 * i), *(blkr[ri] + 2 * i + 1), *(blks + 2 * j), *(blks + 2 * j + 1));
							w++;
							fprintf(fo, "^\n");
							if (w == 3)//ÿ�����п���װ���ĸ���¼abcd
							{
								w = 0;
								*(blkw + 15) = waddr + 1;
								writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
								//ÿ��write�ؽ���д�Ŀ��ڻ������б��Ϊ�ͷţ�����
								blkw = (unsigned int *)getNewBlockInBuffer(&buf);
								waddr++;
							}
						}
					}
				}
			}

			addr2 = *(blks + 15);
			freeBlockInBuffer((unsigned char *)blks, &buf);
			//blks=NULL;
			printf("NLJ next: %d\n", addr2);
		}

		//�ͷŵ�ǰ�Ѿ������blkr��������ָ��ָ����
		for (ri = 0; ri<6; ri++)
		{
			if (blkr[ri] != NULL)
				freeBlockInBuffer((unsigned char *)blkr[ri], &buf);
			blkr[ri] = NULL;
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
	freeBlockInBuffer((unsigned char *)blkw, &buf);

	//�����һ��ȡ�أ��޸�����һ����ĵ�ַΪ0����ʾ��һ�ν�����
	if ((blkw = (unsigned int *)readBlockFromDisk(waddr - 1, &buf)) == NULL)
	{
		perror("NLJ, Reading Block Failed!\n");
	}
	*(blkw + 15) = 0;
	writeBlockToDisk((unsigned char *)blkw, waddr - 1, &buf);

	fclose(fo);
	freeBuffer(&buf);
	return 18000;
}
void deleteTempBlks()
{
	int i = 0;
	for (i = 1; i <= 16; i++)
	{
		dropBlockOnDisk(2000 + i);
		dropBlockOnDisk(4000 + i);
		dropBlockOnDisk(6000 + i);
		dropBlockOnDisk(8000 + i);
	}
}