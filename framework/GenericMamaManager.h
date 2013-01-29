#include <map>
#include <string>
#include <mama/mamacpp.h>

#ifndef __H_GenericMamaManager
#define __H_GenericMamaManager

using namespace Wombat;
using namespace std;

class GenericMamaManager
{
public:
	GenericMamaManager ();
    ~GenericMamaManager();
    void create(const char** middlewares, size_t queues);
    bool isBridgeLoaded(const char* middleware);
    MamaTransport* getMamaTransport(const char* transport, const char* middleware);
    MamaSubscription* getMamaSubscription(const char* middleware,
    		const char* transport, MamaSubscriptionCallback *callback,
    		const char* source, const char* symbol);
    MamaQueue* getDefaultEventQueue(const char* middleware);
    MamaTransport* getTransport(const char* transport, const char* middleware);
    string getMamaSubscriptionKey(const char* middleware,
    		const char* transport, const char* source, const char* symbol);
    MamaSubscription* getExistingMamaSubscription(const char* middleware,
    		const char* transport, const char* source, const char* symbol);
    void start();
private:
    map<string, mamaBridge> mMamaBridges;
    map<string, MamaQueue*> mDefaultMamaQueues;
    map<string, MamaTransport*> mMamaTransports;
    map<string, MamaQueueGroup*> mMamaQueueGroups;
    map<string, MamaSubscription*> mMamaSubscriptions;
    const char* mKeyDelimiter;
};

#endif
