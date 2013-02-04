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
