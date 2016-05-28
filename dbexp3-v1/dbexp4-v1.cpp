// dbexp3-v1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*
typedef struct tagBuffer
{
	unsigned long numIO; // Number of IO's
	size_t bufSize; // Buffer size//
	size_t blkSize; // Block size //
	size_t numAllBlk;// Number of blocks that can be kept in the buffer //
	size_t numFreeBlk;// Number of available blocks in the buffer//
	unsigned char *data;// Starting address of the buffer //
} Buffer;

// Initialize a buffer with the specified buffer size and block size.
// If the initialization fails, the return value is NULL;
// otherwise the pointer to the buffer.
//
Buffer *initBuffer(size_t bufSize, size_t blkSize, Buffer *buf);

// Free the memory used by a buffer. //
void freeBuffer(Buffer *buf);

// Apply for a new block from a buffer.
// If no free blocks are available in the buffer,
// then the return value is NULL; otherwise the pointer to the block.
//
unsigned char *getNewBlockInBuffer(Buffer *buf);

// Set a block in a buffer to be available. //
void freeBlockInBuffer(unsigned char *blk, Buffer *buf);

// Drop a block on the disk //
int dropBlockOnDisk(unsigned int addr);

// Read a block from the hard disk to the buffer by the address of the block.
unsigned char *readBlockFromDisk(unsigned int addr, Buffer *buf);

// Read a block in the buffer to the hard disk by the address of the block.
int writeBlockToDisk(unsigned char *blkPtr, unsigned int addr, Buffer *buf);
*/

int main(int argc, char **argv)
{
	Buffer buf; /* A buffer */
	unsigned char *blk; /* A pointer to a block */
	int i = 0;

	/* Initialize the buffer */
	if (!initBuffer(20, 8, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	/* Get a new block in the buffer */
	blk = getNewBlockInBuffer(&buf);

	/* Fill data into the block */
	for (i = 0; i < 8; i++)
		*(blk + i) = 26 + i;

	/* Write the block to the hard disk */
	if (writeBlockToDisk(blk, 31415926, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}

	/* Read the block from the hard disk */
	if ((blk = readBlockFromDisk(31415926, &buf)) == NULL)
	{
		cout << *(blk);
		perror("Reading Block Failed!\n");
		return -1;
	}

	/* Process the data in the block */
	for (i = 0; i < 8; i++)
		cout << *(blk + i) << endl;

	cout << endl;
	cout << "# of IO's is " << buf.numIO << endl; /* Check the number of IO's */
	return 0;
}
