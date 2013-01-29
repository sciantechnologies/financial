#ifndef __H_RecordingMamaSubscriptionCallback
#define __H_RecordingMamaSubscriptionCallback

#include <mama/mamacpp.h>
#include <cstdio>
#include <framework/GzFileHandler.h>
#include <framework/MamaRecordedMessageCodec.h>

using namespace Wombat;

class RecordingMamaSubscriptionCallback : public MamaSubscriptionCallback
{
public:
    RecordingMamaSubscriptionCallback (GzFileHandler* gzfh);
    virtual ~RecordingMamaSubscriptionCallback (void);
    virtual void onError(MamaSubscription* subscription, const MamaStatus&  status, const char* subject);
    virtual void onMsg (MamaSubscription* subscription, MamaMsg& msg);
    virtual void onQuality (MamaSubscription* subscription, mamaQuality quality, const char* symbol, short cause, const void* platformInfo);
    virtual void onCreate (MamaSubscription* subscription);
    virtual void onDestroy (MamaSubscription* subscription);
    FILE* mRecFile;
private:
    GzFileHandler* mGzFileHandler;
    char* mCharMsgBuffer;
    char* mCharHeaderBuffer;
    MamaRecordedMessageCodec* mMessageCodecEngine;
    size_t mMsgCount;
};

#endif
