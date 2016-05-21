#include "init_show.h"
#include <time.h>
int initdata()
{
	int i = 0;
	int j = 0;
	Buffer *buf = new  Buffer;
	unsigned char* blk;
	
	srand((unsigned int)time(0));
	if (initBuffer(520, 64, buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	for (j = 0; j < 16; j++)
	{
		blk = getNewBlockInBuffer(buf);
	}
}
void show()
{
	int i;
}