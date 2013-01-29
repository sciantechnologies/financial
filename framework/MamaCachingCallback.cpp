#include <framework/MamaCachingCallback.h>

MamaCachingCallback::MamaCachingCallback ()
{
	mCachedMamaMessage = new MamaMsg();
	mCachedMamaMessage->create();
}

MamaCachingCallback::~MamaCachingCallback ()
{
	delete mCachedMamaMessage;
}

void MamaCachingCallback::onError(MamaSubscription* subscription, const MamaStatus&  status, const char* subject)
{

}

void MamaCachingCallback::onMsg (MamaSubscription* subscription, MamaMsg& msg)
{
	//printf("Received Message [%d]\n", time(NULL));
	mCachedMamaMessage->applyMsg(msg);
}

void MamaCachingCallback::onQuality (MamaSubscription* subscription, mamaQuality quality, const char* symbol, short cause, const void* platformInfo)
{

}

void MamaCachingCallback::onCreate (MamaSubscription* subscription)
{

}

void MamaCachingCallback::onDestroy (MamaSubscription* subscription)
{

}

MamaMsg* MamaCachingCallback::getMamaMsgCopy()
{
	MamaMsg* mm = new MamaMsg();
	mm->create();
	mm->copy(*mCachedMamaMessage);
	return mm;
}
