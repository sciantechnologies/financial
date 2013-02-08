/**
 * Copyright (c) 2013 Scian Technologies
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:7
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 * 
 */

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
