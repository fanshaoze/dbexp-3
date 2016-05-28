#include "stdafx.h"

int NLJ()//采用块嵌套循环连接
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
		cout << "打不开文件whenJoin.txt" << endl;
		return 0;
	}

	//先把磁盘上之前的NLJ结果块们删掉
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

	blkw = (unsigned int *)getNewBlockInBuffer(&buf);//要一块缓冲区块，用来暂存要写回磁盘的块

	for (times_put_r_in_buf = 0; times_put_r_in_buf<3; times_put_r_in_buf++)
	{
		for (ri = 0; ri<6 && addr1 != 0; ri++)//把R读6块进缓冲区
		{
			fprintf(fo, "现在R是第%d块\n", addr1);
			if ((blkr[ri] = (unsigned int *)readBlockFromDisk(addr1, &buf)) == NULL)
			{
				perror("Reading Block Failed!\n");
				break;
			}
			addr1 = *(blkr[ri] + 15);
		}
		//对S的每个块，遍历缓冲区中的6个R块
		addr2 = Saddr;


		while (addr2 != 0)//读下一个块
		{
			if ((blks = (unsigned int *)readBlockFromDisk(addr2, &buf)) == NULL)
			{
				perror("NLJ,Reading Block Failed!\n");
				break;
			}

			fprintf(fo, "现在S是第%d块\n", addr2);
			for (ri = 0; ri<6; ri++)
			{
				if (blkr[ri] == NULL) break;
				for (i = 0; i<7; i++)//遍历块中的元组
				{//处理读取块中的单个元组
					for (j = 0; j<7; j++)
					{
						if (*(blks + 2 * j) == *(blkr[ri] + 2 * i))
						{
							*(blkw + 4 * w) = *(blkr[ri] + 2 * i);
							*(blkw + 4 * w + 1) = *(blkr[ri] + 2 * i + 1);
							*(blkw + 4 * w + 2) = *(blks + 2 * j);
							*(blkw + 4 * w + 3) = *(blks + 2 * j + 1);
							fprintf(fo, "哈A %d, B %d, C %d, D %d\n", *(blkr[ri] + 2 * i), *(blkr[ri] + 2 * i + 1), *(blks + 2 * j), *(blks + 2 * j + 1));
							w++;
							fprintf(fo, "^\n");
							if (w == 3)//每个块中可以装下四个记录abcd
							{
								w = 0;
								*(blkw + 15) = waddr + 1;
								writeBlockToDisk((unsigned char *)blkw, waddr, &buf);
								//每次write回将被写的块在缓冲区中标记为释放！！！
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

		//释放当前已经用完的blkr，并把其指针指到空
		for (ri = 0; ri<6; ri++)
		{
			if (blkr[ri] != NULL)
				freeBlockInBuffer((unsigned char *)blkr[ri], &buf);
			blkr[ri] = NULL;
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
	freeBlockInBuffer((unsigned char *)blkw, &buf);

	//把最后一块取回，修改其下一个块的地址为0，表示这一段结束了
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