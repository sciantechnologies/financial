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
