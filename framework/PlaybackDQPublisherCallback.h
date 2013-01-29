#include <mama/mama.h>
#include <mama/mamacpp.h>
#include <mama/MamaDQPublisherManagerCallback.h>
#include <mama/MamaDQPublisherManager.h>
#include <mama/MamaDQPublisher.h>
#include <mama/MamaQueueEventCallback.h>
#include <map>
#include <framework/GenericMamaCache.h>

using namespace Wombat;
using namespace std;

#ifndef __H_PlaybackDQPublisherCallback
#define __H_PlaybackDQPublisherCallback

class PlaybackDQPublisherCallback : public MamaDQPublisherManagerCallback, MamaBasicSubscriptionCallback
{
public:
	PlaybackDQPublisherCallback () {}
    ~PlaybackDQPublisherCallback() {}
    void onCreate(MamaBasicSubscription* mbs);
    void onError(MamaBasicSubscription* mbs, const Wombat::MamaStatus& status, const char* err);
    void onMsg(MamaBasicSubscription* mbs, MamaMsg& msg);

    void onCreate (MamaDQPublisherManager*  publisher);

    void onNewRequest (
        MamaDQPublisherManager*  publisherManager,
        const char*              symbol,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg);

    void onRequest (
        MamaDQPublisherManager*  publisherManager,
        const MamaPublishTopic&  publishTopicInfo,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg);

    void onRefresh (
        MamaDQPublisherManager*  publisherManager,
        const MamaPublishTopic&  publishTopicInfo,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg);

    void onError (
        MamaDQPublisherManager*  publisher,
        const MamaStatus&        status,
        const char*              errortxt,
        MamaMsg*                 msg);
    bool addMamaMsg(MamaMsg* msg, char* symbol);
    MamaDQPublisher* getDQPublisher(char* symbol);
    void setDictionaryResponder(MamaTransport* transport, MamaDictionary* dict);
private:
    GenericMamaCache*	mGenericMamaCache;
    map<string, GenericMamaCache*> mCacheMap;
    map<GenericMamaCache*, MamaDQPublisher*> mDQPubMap;
    map<GenericMamaCache*, MamaDQPublisher*>::iterator mDQPubIterator;
    MamaPublisher* mDictPublisher;
    MamaMsg* mDictMessage;
};

#endif
