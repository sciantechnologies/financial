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

#include <framework/GenericMamaManager.h>
#include <stdlib.h>

GenericMamaManager::GenericMamaManager ()
{
	mKeyDelimiter = "::";
}

GenericMamaManager::~GenericMamaManager()
{

}

void GenericMamaManager::create(const char** middlewares, size_t queues)
{
	printf("CREATING\n");
	// Load middleware bridges
	for (const char **mid_pointer = middlewares; *mid_pointer != NULL; mid_pointer++)
	{
		printf("Loading MAMA [%s] bridge\n", *mid_pointer);
		mamaBridge bridge = Mama::loadBridge (*mid_pointer);
		mMamaBridges[*mid_pointer] = bridge;
	}

	// Open up MAMA
	Mama::open ();

	// Set MAMA logging level
	Mama::enableLogging (MAMA_LOG_LEVEL_NORMAL, stderr);

	// Populate members with number of queues and default event queues
	for (const char **mid_pointer = middlewares; *mid_pointer != NULL; mid_pointer++)
	{
		printf("Loading MAMA [%s] Queue Group", *mid_pointer);
		mMamaQueueGroups[*mid_pointer] = new MamaQueueGroup (queues, mMamaBridges[*mid_pointer]);
	}
}

MamaQueue* GenericMamaManager::getDefaultEventQueue(const char* middleware)
{
	map<string, MamaQueue*>::iterator qit;
	map<string, mamaBridge>::iterator bit;

	// Return NULL if the middleware is not loaded
	if(!isBridgeLoaded(middleware))
		return NULL;

	qit = mDefaultMamaQueues.find(middleware);
	if(qit != mDefaultMamaQueues.end())
	{
		MamaQueue* mq = Mama::getDefaultEventQueue (mMamaBridges[middleware]);
		mDefaultMamaQueues[middleware] = mq;
		return mq;
	}
	else
		return qit->second;
}

bool GenericMamaManager::isBridgeLoaded(const char* middleware)
{
	map<string, mamaBridge>::iterator bit;
	bit = mMamaBridges.find(middleware);
	if(bit == mMamaBridges.end())
		return false;
	else
		return true;
}

MamaTransport* GenericMamaManager::getMamaTransport(const char* transport, const char* middleware)
{
	if(!isBridgeLoaded(middleware))
	{
		printf("Middleware %s does not seem to be loaded - transport %s cannot be loaded.\n",
				middleware, transport);
		return NULL;
	}
	// Build up the key for the map
	string key;
	key.append(middleware);
	key.append(mKeyDelimiter);
	key.append(transport);

	// Isolate the bridge
	mamaBridge bridge = mMamaBridges[middleware];

	// Find and return it if it already exists
	map<string, MamaTransport*>::iterator mtit;
	mtit = mMamaTransports.find(key);
	if(mtit != mMamaTransports.end())
	{
		printf("Transport %s seems to already exist - re-using...\n");
		return mtit->second;
	}
	// Create it if it does not
	else
	{
		printf("Transport %s does not yet exist - creating it...\n", transport);
		MamaTransport* t = new MamaTransport;
		t->create(transport, bridge);
		if(t == NULL)
			printf("Error creating transport %s\n", key.c_str());
		else
		{
			mMamaTransports[key] = t;
		}
		return t;
	}
}

MamaSubscription* GenericMamaManager::getMamaSubscription(const char* middleware,
		const char* transport, MamaSubscriptionCallback *callback,
		const char* source, const char* symbol)
{
	if(!isBridgeLoaded(middleware))
	{
		printf("Cannot get MAMA Subscription for transport %s as bridge %s is not loaded",
				transport, middleware);
		return NULL;
	}

	string key = getMamaSubscriptionKey(middleware, transport, source, symbol);

	map<string, MamaSubscription*>::iterator msit;
	msit = mMamaSubscriptions.find(key);
	if(msit == mMamaSubscriptions.end())
	{
		// Get the next available queue group for this bridge
		MamaQueue* mq = mMamaQueueGroups[middleware]->getNextQueue();
		MamaTransport* mt = getMamaTransport(transport, middleware);
		MamaSubscription* sub = new MamaSubscription();
		if(mt != NULL)
		{
			sub->create (mt, mq, callback, source, symbol, NULL);
			mMamaSubscriptions[key] = sub;
		}
		else
		{
			printf("Failed to create transport %s\n", transport);
		}
		return sub;
	}
	else
		return msit->second;
}

string GenericMamaManager::getMamaSubscriptionKey(const char* middleware,
		const char* transport, const char* source, const char* symbol)
{
	string key;
	key.append(middleware);
	key.append(mKeyDelimiter);
	key.append(transport);
	key.append(mKeyDelimiter);
	key.append(source);
	key.append(mKeyDelimiter);
	key.append(symbol);
	return key;
}

void GenericMamaManager::start()
{
	map<string, mamaBridge>::iterator bit;
	for(bit = mMamaBridges.begin(); bit != mMamaBridges.end(); bit++)
	{
		Mama::startBackground(bit->second, NULL);
	}
}

MamaSubscription* GenericMamaManager::getExistingMamaSubscription(const char* middleware,
		const char* transport, const char* source, const char* symbol)
{
	string key = getMamaSubscriptionKey(middleware, transport, source, symbol);
	map<string, MamaSubscription*>::iterator msit;
	msit = mMamaSubscriptions.find(key);
	if(msit == mMamaSubscriptions.end())
	{
		return NULL;
	}
	else
	{
		return msit->second;
	}
}
