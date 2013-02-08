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

#include <framework/PlaybackDQPublisherCallback.h>
#include <framework/GenericMamaCache.h>
#include <stdio.h>
#include <mama/version.h>

void PlaybackDQPublisherCallback::onCreate (MamaDQPublisherManager*  publisher)
{
	printf("Creating new publisher callback\n");
	mGenericMamaCache = NULL;
    mCacheMap.clear();
    mDQPubMap.clear();
    mDictPublisher = NULL;
    mDictMessage = NULL;
}

// The basic callbacks for dictionary requests
void PlaybackDQPublisherCallback::onCreate(MamaBasicSubscription* mbs)
{

}

void PlaybackDQPublisherCallback::onError(MamaBasicSubscription* mbs, const Wombat::MamaStatus& status, const char* err)
{

}

void PlaybackDQPublisherCallback::onMsg(MamaBasicSubscription* mbs, MamaMsg& msg)
{
	if(mDictPublisher != NULL)
	{
		mDictMessage->updateU8 (MamaFieldMsgType.mName, MamaFieldMsgType.mFid, MAMA_MSG_TYPE_INITIAL);
		printf("Processing Basic request for %s...\n", mbs->getTopic());
		mDictPublisher->sendReplyToInbox(msg, mDictMessage);
	}
}

void PlaybackDQPublisherCallback::setDictionaryResponder(MamaTransport* transport, MamaDictionary* dict)
{
	mDictPublisher = new MamaPublisher();
	mDictMessage = dict->getDictionaryMessage();
    mDictPublisher->create (transport, "WOMBAT.DATA_DICT", NULL, NULL);
}

void PlaybackDQPublisherCallback::onNewRequest (
        MamaDQPublisherManager*  publisherManager,
        const char*              symbol,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg)
{
	printf("Processing new request for %s...\n", symbol);
	map<string, GenericMamaCache*>::iterator mCacheIterator;
	string s = symbol;
	mCacheIterator = mCacheMap.find(s);
	if(mCacheIterator != mCacheMap.end())
	{
		MamaDQPublisher *mPub = publisherManager->createPublisher (symbol, mCacheIterator->second);
		mDQPubMap[mCacheIterator->second] = mPub;
		//mPub->setSenderId(0);
		//mPub->setStatus(MAMA_MSG_STATUS_OK);
		//mPub->setSeqNum(0);
		MamaMsg* reply_msg = mCacheIterator->second->getMsg();
		mamaMsg cmm = reply_msg->getUnderlyingMsg ();
		mama_log (MAMA_LOG_LEVEL_FINEST, "Underlying(): "
		                   "Sent message from C. %s", mamaMsg_toString(cmm));

		//printf("Processing new request for %s [%s]...\n", symbol, reply_msg->toString());
		reply_msg->updateU8 (NULL, MamaFieldMsgType.mFid, MAMA_MSG_TYPE_INITIAL);
		// There seems to be a bug in the avis library where sending a message inside
		// this callback won't work, so not publishing this full initial image.... deltas
		// will still get published
		//mPub->sendReply(msg, reply_msg);
		//printf("Attempting to send message: %s\n", reply_msg->toString());
		mPub->send(reply_msg);
	}
	else
	{
		printf("Can't satisfy request for %s as it has not yet been cached\n", symbol);
	}
}

void PlaybackDQPublisherCallback::onRequest (
        MamaDQPublisherManager*  publisherManager,
        const MamaPublishTopic&  publishTopicInfo,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg)
{
	GenericMamaCache* gmc = (GenericMamaCache*)publishTopicInfo.mCache;
	MamaDQPublisher* mdqp = publishTopicInfo.mPub;
	const char* symbol = publishTopicInfo.mSymbol;

	MamaMsg* reply_msg = gmc->getMsg();

	printf("Processing additional request for %s [%s]\n", symbol, reply_msg->toString());
	reply_msg->updateU8 (NULL, MamaFieldMsgType.mFid, MAMA_MSG_TYPE_INITIAL);
	mdqp->send(reply_msg);
}

void PlaybackDQPublisherCallback::onRefresh (
        MamaDQPublisherManager*  publisherManager,
        const MamaPublishTopic&  publishTopicInfo,
        short                    subType,
        short                    msgType,
        MamaMsg&                 msg)
{
	printf("Processing refresh\n");
}

void PlaybackDQPublisherCallback::onError (
        MamaDQPublisherManager*  publisher,
        const MamaStatus&        status,
        const char*              errortxt,
        MamaMsg*                 msg)
{
	printf("Processing error\n");
}

bool PlaybackDQPublisherCallback::addMamaMsg(MamaMsg* msg, char* symbol)
{
	map<string, GenericMamaCache*>::iterator mCacheIterator;
	string s = symbol;
	bool r = true;
	mGenericMamaCache = NULL;
	mCacheIterator = mCacheMap.find(s);
	// If this isn't yet in the cache, add it...
	if(mCacheIterator == mCacheMap.end())
	{
		mGenericMamaCache = new GenericMamaCache();
		mCacheMap[s] = mGenericMamaCache;
		//printf("Adding new entry to cache map for %s {%s}\n", s.c_str(), msg->toString());
	}
	else
	{
		mGenericMamaCache = mCacheIterator->second;
	}
	if(mGenericMamaCache != NULL)
	{
		// If there is no interest in this symbol, return false
		mDQPubIterator = mDQPubMap.find(mGenericMamaCache);
		if(mDQPubIterator == mDQPubMap.end())
		{
			r = false;
		}
	}

	mGenericMamaCache->applyUpdate(*msg);
	return r;
}

MamaDQPublisher* PlaybackDQPublisherCallback::getDQPublisher(char* symbol)
{
	string s = symbol;
	mDQPubIterator = mDQPubMap.find(mGenericMamaCache);
	if(mDQPubIterator != mDQPubMap.end())
	{
		return mDQPubIterator->second;
	}
	else
	{
		return NULL;
	}
}
