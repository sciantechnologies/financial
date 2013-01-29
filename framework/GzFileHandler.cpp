#include "GzFileHandler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


GzFileHandler::GzFileHandler ()
{
	this->resetDefaults();
}

GzFileHandler::GzFileHandler (const char* filename, int mode)
{
	this->resetDefaults();
    this->create(filename, mode);
}

GzFileHandler::~GzFileHandler (void)
{
	gzflush(mGzFile, Z_FINISH);
	gzclose(mGzFile);
	free(mRootBufferPointer);
}

void GzFileHandler::resetDefaults()
{
	mGzFile = NULL;
	mOutputBufferSize = GZ_FILE_HANDLER_BUFFER_SIZE;
	mLowWaterMark = GZ_FILE_HANDLER_LOW_WM;
	mChunkSize = GZ_FILE_HANDLER_CHUNK;
	mRemainingUsedBufferSize = 0;
	mRootBufferPointer = NULL;
	mSeekBufferPointer = NULL;
}

bool GzFileHandler::create (const char* filename, int mode)
{
	mGzFileMode = mode;
	switch(mGzFileMode)
	{
	if(mGzFileMode == 0)
		mGzFileMode = GZ_FILE_HANDLER_READ;

	case GZ_FILE_HANDLER_WRITE:
		mGzFile = gzopen (filename, "w");
		break;
	case GZ_FILE_HANDLER_READ:
		mGzFile = gzopen (filename, "r");
		break;
	default:
		break;
	}
	if(mGzFile != NULL)
		return false;
	else
		return true;
}

bool GzFileHandler::initializeReadBuffer()
{
	// Free up some memory
	mRootBufferPointer = (char*)malloc(mOutputBufferSize);
	mSeekBufferPointer = mRootBufferPointer;

	// If the memory allocation fails, we return false
	if(mRootBufferPointer == NULL)
		return false;
	else
		return true;
}

void GzFileHandler::readBlock()
{
	// Initialize the buffer if it isn't already initialized
	if(mRootBufferPointer == NULL)
	{
		if(this->initializeReadBuffer() == false)
			return;
	}

	if(mRemainingUsedBufferSize < mLowWaterMark)
	{
		//printf("We are %d miles from home and %d < %d...\n", mSeekBufferPointer - mRootBufferPointer,
		//		mRemainingUsedBufferSize, mLowWaterMark);
		// If this is the first run or we're already at the root node, no realignment is necessary
		if(mSeekBufferPointer != mRootBufferPointer)
		{
			//printf("Realigning Memory...\n");
			// Realign the block to the original root pointer
			memcpy(mRootBufferPointer, mSeekBufferPointer, mRemainingUsedBufferSize);
		}
		size_t read_in = gzread(mGzFile, mRootBufferPointer+mRemainingUsedBufferSize, mChunkSize);
		mSeekBufferPointer = mRootBufferPointer;
		//printf("Size read in: %d...\n", read_in);
		// Buffer will now be a little bigger
		mRemainingUsedBufferSize += read_in;
	}
}

char* GzFileHandler::readTo(const char* key, bool nowrap, unsigned long int offset)
{
	if(nowrap == false)
		this->readBlock();

	if(offset > 0)
		this->shiftTo(offset);
	char* match = this->findString(key, mSeekBufferPointer, mRemainingUsedBufferSize);
	if(match == NULL)
		return NULL;

	mRemainingUsedBufferSize -= match - mSeekBufferPointer;
	mSeekBufferPointer = match;
	return mSeekBufferPointer;
}

char* GzFileHandler::findString(const char* key, char* start, unsigned long int& limit)
{
	unsigned long int i = 1;
	char* r = start;
	char* sub = NULL;
	while (sub == NULL && i < limit)
	{
		sub = strstr(r, key);
		if(sub != NULL)
			r = sub;
		else
			r++;
		i++;
	}
	if(i == limit)
		return NULL;
	else
		return r;
}

char* GzFileHandler::shiftTo(unsigned long int offset)
{

	if(offset > mRemainingUsedBufferSize)
	{
		mSeekBufferPointer += mRemainingUsedBufferSize;
		mRemainingUsedBufferSize = 0;
		return NULL;
	}
	else
	{
		mSeekBufferPointer += offset;
		mRemainingUsedBufferSize -= offset;
	}
	return mSeekBufferPointer;
}

bool GzFileHandler::write(char* buffer, size_t& buffer_size)
{
	if(mGzFileMode & GZ_FILE_HANDLER_WRITE == 0)
		return false;
	if(gzwrite(mGzFile, buffer, buffer_size) <= 0)
		return false;
	else
		return true;
}
