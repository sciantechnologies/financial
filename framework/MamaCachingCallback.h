#include <mama/mamacpp.h>
#include <map>

using namespace std;
using namespace Wombat;

#ifndef __H_MamaCachingCallback
#define __H_MamaCachingCallback

class MamaCachingCallback : public MamaSubscriptionCallback
{
public:
	MamaCachingCallback ();
    virtual ~MamaCachingCallback ();
    virtual void onError(MamaSubscription* subscription, const MamaStatus&  status, const char* subject);
    virtual void onMsg (MamaSubscription* subscription, MamaMsg& msg);
    virtual void onQuality (MamaSubscription* subscription, mamaQuality quality, const char* symbol, short cause, const void* platformInfo);
    virtual void onCreate (MamaSubscription* subscription);
    virtual void onDestroy (MamaSubscription* subscription);
    // returns a copy of the latest MAMA Message... up to caller to destroy
    MamaMsg* getMamaMsgCopy();
private:
    MamaMsg* mCachedMamaMessage;
};
#endif
