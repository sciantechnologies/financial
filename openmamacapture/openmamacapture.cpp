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
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <framework/RecordingMamaSubscriptionCallback.h>
#include <set>
#include <framework/GzFileHandler.h>

using namespace Wombat;
using namespace std;

class DictionaryCallback : public MamaDictionaryCallback
{
public:
    DictionaryCallback (mamaBridge bridgeImpl)
    {
        mBridgeImpl = bridgeImpl;
    }

    virtual ~DictionaryCallback (void) {}

    void onTimeout (void)
    {
        cerr << "Listener timed out waiting for dictionary" << endl;
        Mama::stop (mBridgeImpl);
    }

    void onError (const char* errMsg)
    {
        cerr << "Error getting dictionary: " << errMsg << endl;
        Mama::stop (mBridgeImpl);
    }

    void onComplete (void)
    {
        cerr << "Dictionary retreived..." << endl;
        Mama::stop (mBridgeImpl);
    }
private:
    mamaBridge   mBridgeImpl;
};


void* t_rmsc = NULL;
MamaQueueGroup* mQueueGroup = NULL;
vector<MamaSubscription*> mVectorMamaSubscription;
mamaBridge   mBridgeImpl;
MamaTransport* transport;

void cleanup(int sig)
{
    printf("Exiting application...\n");
    // Stop Dispatching
    printf("Stopping dispatch...\n");
    mQueueGroup->stopDispatch();
    // Mama stop
    printf("Stopping Bridge...\n");
    Mama::stop(mBridgeImpl);
    // Destroy Subscriptions
    printf("Destroying Subscriptions...\n");
    vector<MamaSubscription*>::iterator vit;
    for (vit = mVectorMamaSubscription.begin(); vit != mVectorMamaSubscription.end(); vit++)
    {
    	(*vit)->destroy();
    }
    printf("Clearing subscription vector...\n");
    mVectorMamaSubscription.clear();
    // Destroy Event Queues and wait for them to empty
    printf("Destroying event queues...\n");
    mQueueGroup->destroyWait();
    printf("Flushing Gzip Buffer...\n");
    delete (RecordingMamaSubscriptionCallback*)t_rmsc;
    // Destroy Transports
    //printf("Destroying transport...\n");
    //delete transport;
    // Mama close
    printf("Closing MAMA...\n");
    //Mama::close();

    exit(1);
}

void usage()
{

}

int main(int argc, const char** argv)
{
    //MamaQueue* mDefaultQueue = NULL;

    const char* symbol = "";
    const char* source = "";
    const char* middleware = "";
    const char* transportname = "";
    const char* output_filename = "";
    const char* symbol_filename = "";
    GzFileHandler* gzfh = NULL;
    set<string> all_symbols;

    int i = 1;

    for (i = 1; i < argc;)
    {
        if (strcmp (argv[i], "-S") == 0)
        {
            source = argv[i + 1];
            i += 2;
        }
        else if (strcmp (argv[i], "-tport") == 0)
        {
            transportname = argv[i + 1];
            i += 2;
        }
        else if (strcmp (argv[i], "-f") == 0)
        {
            char sbuf[128];
            symbol_filename = argv[i + 1];
            i += 2;
            FILE *sfile = fopen(symbol_filename, "r");
            while(fgets(sbuf, 128, sfile) != NULL)
            {
                char* nl_find = strchr(sbuf, '\n');
                if(nl_find != NULL)
                    *nl_find = '\0';
                string str = sbuf;
                all_symbols.insert(sbuf);
            }
        }
        else if (strcmp (argv[i], "-s") == 0)
        {
            symbol = argv[i + 1];
            i += 2;
            all_symbols.insert(symbol);
        }
        else if (strcmp (argv[i], "-m") == 0)
        {
            middleware = argv[i + 1];
            i += 2;
        }
        else if (strcmp (argv[i], "-o") == 0)
        {
            output_filename = argv[i + 1];
            i += 2;
        }
    }

    gzfh = new GzFileHandler(output_filename, GZ_FILE_HANDLER_WRITE);
    t_rmsc = new RecordingMamaSubscriptionCallback(gzfh);
    int number_threads = 1;

    // Load all required bridges at this point
    mBridgeImpl = Mama::loadBridge (middleware);

    // Open up the MAMA api
    Mama::open ();

    Mama::enableLogging (MAMA_LOG_LEVEL_NORMAL, stderr);
    MamaQueue* mDefaultQueue = Mama::getDefaultEventQueue (mBridgeImpl);
    transport = new MamaTransport;
    transport->create(transportname, mBridgeImpl);
    set<string>::iterator it;
    MamaSource* mDictSource = new MamaSource ("WOMBAT", transport, "WOMBAT");

    DictionaryCallback cb (mBridgeImpl);
    MamaDictionary* mDictionary = new MamaDictionary;
    mDictionary->create (mDefaultQueue, &cb, mDictSource, 3, 10);
    Mama::start (mBridgeImpl);
    printf("Writing Dictionary...\n");
    mDictionary->writeToFile("/home/frank/wush/fquinn/openmamacapture/data.dict");
    mQueueGroup = new MamaQueueGroup (number_threads, mBridgeImpl);

    for(it = all_symbols.begin(); it != all_symbols.end(); it++)
    {
        printf("Setting up MAMA Subscription for %s\n", (*it).c_str());
        MamaSubscription* sub = new MamaSubscription;
        mVectorMamaSubscription.push_back(sub);
        sub->create (transport, mQueueGroup->getNextQueue (), (RecordingMamaSubscriptionCallback*)t_rmsc,
                 source, (*it).c_str(), NULL);
    }
    signal(SIGINT, cleanup);
    printf("Starting...\n");
    Mama::start(mBridgeImpl);
}
