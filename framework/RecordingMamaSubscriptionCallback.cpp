#include <cstdio>
#include <ctime>
#include "RecordingMamaSubscriptionCallback.h"
#include <framework/MamaRecordedMessageCodec.h>
//#include <sys/time.h>


void RecordingMamaSubscriptionCallback::onError(MamaSubscription* subscription, const MamaStatus&  status, const char* subject)
{
    printf("Received onError at %d\n", time(NULL));
}

void RecordingMamaSubscriptionCallback::onMsg(MamaSubscription* subscription, MamaMsg& msg)
{
    timespec rtime;
    const char* sym = subscription->getSymbol();
    size_t buf_len = 0;
    const void* msg_buf;

    msg.getByteBuffer (msg_buf, buf_len);
    clock_gettime(0, &rtime);
    uint32_t msg_bytes_written = mMessageCodecEngine->encodeMsg(mCharMsgBuffer, &msg);
    size_t header_bytes_written = sprintf(mCharHeaderBuffer, "^M^S^%02d|%ld.%09d|%09d|%s^M^E^", 1, rtime.tv_sec, rtime.tv_nsec, msg_bytes_written, sym);
    mGzFileHandler->write(mCharHeaderBuffer, header_bytes_written);
    mGzFileHandler->write(mCharMsgBuffer, msg_bytes_written);
	if(mMsgCount % 1000 == 0)
		printf("Update for %s received at %ld.%09d\n", sym, rtime.tv_sec, rtime.tv_nsec);
    mMsgCount++;
}

void RecordingMamaSubscriptionCallback::onQuality(MamaSubscription* subscription, mamaQuality quality, const char* symbol, short cause, const void* platformInfo)
{
    printf("Received onQuality at %d\n", time(NULL));
}

void RecordingMamaSubscriptionCallback::onCreate(MamaSubscription* subscription)
{
}

RecordingMamaSubscriptionCallback::RecordingMamaSubscriptionCallback(GzFileHandler* gzfh)
{
    mGzFileHandler = gzfh;
    mCharMsgBuffer = (char*)malloc(1048576);
    mCharHeaderBuffer = (char*)malloc(4048);
    mMessageCodecEngine = new MamaRecordedMessageCodec();
    mMsgCount = 0;
}

RecordingMamaSubscriptionCallback::~RecordingMamaSubscriptionCallback()
{
	printf("Destroying RMS\n");
	delete mGzFileHandler;
    free(mCharMsgBuffer);
    free(mCharHeaderBuffer);
    delete mMessageCodecEngine;
}

void RecordingMamaSubscriptionCallback::onDestroy(MamaSubscription* ms)
{

}

