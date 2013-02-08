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
