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


/**
 * @file 
 * @author Frank Quinn
 * @version
 *
 */

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
