#ifndef __H_GzFileHandler
#define __H_GzFileHandler

#include <zlib.h>

// Constants for Read / Write
#define		GZ_FILE_HANDLER_WRITE			1
#define		GZ_FILE_HANDLER_READ			2

// Default low water mark size of 128k
#define		GZ_FILE_HANDLER_LOW_WM			131072

// Default read ahead buffer chunk size
#define		GZ_FILE_HANDLER_CHUNK			524288

// Default gzip buffer size of 1MB
#define		GZ_FILE_HANDLER_BUFFER_SIZE		1048576

class GzFileHandler
{
public:
	GzFileHandler ();
	GzFileHandler (const char* filename, int mode);
    ~GzFileHandler ();
    bool create (const char* filename, int mode);
    bool write(char* buffer, size_t& buffer_size);
    char* readTo(const char* key, bool nowrap = false, unsigned long int offset = 0);
    char* shiftTo(unsigned long int offset);
private:
    bool initializeReadBuffer();
    void resetDefaults();
    void readBlock();
    gzFile mGzFile;
    char* findString(const char* key, char* start, unsigned long int& limit);
    unsigned long int mOutputBufferSize;
    unsigned long int mLowWaterMark;
    unsigned long int mChunkSize;
    // This is the remaining used buffer (measured from mSeekBufferPointer)
    unsigned long int mRemainingUsedBufferSize;
    char* mRootBufferPointer;
    char* mSeekBufferPointer;
    int mGzFileMode;
};
#endif
