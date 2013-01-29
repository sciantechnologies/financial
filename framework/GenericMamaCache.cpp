#include <framework/GenericMamaCache.h>
#include <framework/GenericMamaUtilities.h>

using namespace boost::interprocess;

named_mutex mMutex(open_or_create, "GMC");

GenericMamaCache::GenericMamaCache ()
{
	mMsg = new MamaMsg();
	mMsg->create();
	mMamaMsgIterator = new MamaMsgIterator(NULL);

}

GenericMamaCache::~GenericMamaCache()
{
	delete mMsg;
	delete mMamaMsgIterator;
}

void GenericMamaCache::applyUpdate(MamaMsg& msg)
{
	//printf("MSGL %s", msg.toString());
	//scoped_lock<named_mutex> lock(mMutex);
	//printf("Applying Message Update\n");
	try{
		mMsg->applyMsg(msg);
	}
	catch(MamaStatus& status)
	{
		printf("Encountered problem applying MAMA Msg Update - stepping out [%s]\n", status.toString());
	}

	//msg.applyMsg(*mMsg);
	/*
	msg.begin(*mMamaMsgIterator);

	printf("Entering while\n");
    while (*(*mMamaMsgIterator) != NULL)
    {
    	printf("Assigning Iterator\n");
    	MamaMsgField& mmf = *(*mMamaMsgIterator);
    	mama_fid_t mft = mmf.getFid();
    	//Update this field in the cache
    	printf("Adding or updating field\n");
    	GenericMamaUtilities::addOrUpdateField(&mmf, *mMsg);
        ++(*mMamaMsgIterator);
    }
    */
    //printf("Finished applying Message Update\n");
    //scoped_lock<named_mutex> unlock(mMutex);
}

MamaMsg* GenericMamaCache::getMsg()
{
	return mMsg;
}

void GenericMamaCache::clear()
{
	//scoped_lock<named_mutex> lock(mMutex);
	mMsg->clear();
	//scoped_lock<named_mutex> unlock(mutex);
}
