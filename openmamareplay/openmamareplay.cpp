#include <mama/mamacpp.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <framework/GzFileHandler.h>
#include <framework/MamaRecordedMessage.h>
#include <framework/PlaybackDQPublisherCallback.h>
#include <set>
#include <unistd.h>

using namespace Wombat;
using namespace std;

const char* APPLICATION_NAME = "openmamareplay";

void cleanup(int sig)
{
    printf("Exiting application...\n");
    Mama::close();
    exit(1);
}

void usage()
{
	printf("Usage: %s [OPTION]... \n\
Replays custom MAMA payload agnostic messaging format.\n\
\n\
 -S\t[source] \tMAMA source name to publish under\n\
 -tport\t[transport]\tMAMA transport name to publish onto\n\
 -f\t[file]\t\tInput payload agnostic messaging format file (.gz)\n\
 -m\t[middleware]\tMiddleware on which to publish\n\
 -DF\t[file]\t\tData dictionary file (ascii format)\n\
 -rewind\t[count]\t\t[Optional] Number of times to rewind (-1 for infinite)\n\
 -rate\t[count]\t\t[Optional] Number of messages per second (-1 for no rate)\n", APPLICATION_NAME);
}

void playBackFile(const char* filename, PlaybackDQPublisherCallback* managerCallback, long int rate)
{
	GzFileHandler* gzfh = new GzFileHandler(filename, GZ_FILE_HANDLER_READ);

	printf("Playing back file from the beginning...\n");
    char* next_msg = NULL;
    char header[400];
    MamaMsg* mm = NULL;
    char* symbol;
    MamaDQPublisher* mdqp = NULL;
    uint64_t incrementor = 1;
    MamaRecordedMessage* mrm = new MamaRecordedMessage();
    long int delay = 0;
    if(rate > 0)
    {
    	delay = 1000000l/(long int)rate;
    }

    do
    {
    	if(incrementor % 10000 == 0)
    		printf("Processing message number %u\n", incrementor);

    	next_msg = gzfh->readTo("^M^S^");
    	// second parameter forces wrap not to happen
    	char* msg_end = gzfh->readTo("^M^E^", true);
    	if(msg_end == NULL || next_msg == NULL)
    		break;
    	// Include the actual end
    	msg_end += 5;
    	// This will be the size of the header
    	size_t offset = msg_end-next_msg;
    	//printf("[%x - %x] = size of %d\n", next_msg, msg_end, offset);
    	memcpy(header, next_msg, offset);
    	if(offset > 399)
    	{
    		printf("ERROR: offset is greater than buffer size - exiting...");
    		continue;
    	}

    	//printf("Header Found: %s\n", header);
    	mrm->populate(next_msg, offset);
    	mm = mrm->getMamaMsg(symbol);

    	// Will return true if this is an active (subscribed to) symbol
    	if(managerCallback->addMamaMsg(mm, symbol))
    	{
    		//printf("addMamaMsg for new symbol [%s] returned true\n", symbol);
    		mdqp = managerCallback->getDQPublisher(symbol);
    		mdqp->send(mm);
    	}
    	if(delay > 0)
    	{
    		usleep(delay);
    	}

    	mm->clear();
    	incrementor++;
    }
    while(next_msg != NULL);
    delete gzfh;
}

int main(int argc, const char** argv)
{
    const char* source = "";
    const char* middleware = "";
    const char* transportname = "";
    const char* playback_filename = "";
    const char* dict_filename = "";
    const char* rewind_count = "";
    const char* rate_count = "";

    int i = 1;
    int rewind = 1;
    long int rate = 0;

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
            playback_filename = argv[i + 1];
            i += 2;
        }
        else if (strcmp (argv[i], "-m") == 0)
        {
            middleware = argv[i + 1];
            i += 2;
        }
        else if (strcmp (argv[i], "-rewind") == 0)
		{
        	rewind_count = argv[i + 1];
        	rewind = atoi(rewind_count);
			i += 2;
		}
        else if (strcmp (argv[i], "-rate") == 0)
		{
        	rate_count = argv[i + 1];
        	rate = atol(rate_count);
			i += 2;
		}
        else if (strcmp (argv[i], "-DF") == 0)
		{
			dict_filename = argv[i + 1];
			i += 2;
		}
    }

    if(strlen(source) == 0 || strlen(middleware) == 0 || strlen(transportname) == 0
    	|| strlen(playback_filename) == 0 || strlen(dict_filename) == 0)
    {
    	usage();
    	return (1);
    }


    // Load the bridge
    mamaBridge mBridgeImpl = Mama::loadBridge (middleware);
    Mama::enableLogging (MAMA_LOG_LEVEL_NORMAL, stderr);
    Mama::open();
    // Create the transport
    MamaTransport* transport = new MamaTransport;
    transport->create(transportname, mBridgeImpl);
    // Create the queue group
    MamaQueueGroup* mQueueGroup = new MamaQueueGroup (1, mBridgeImpl);
    MamaDictionary* mDictionary = new MamaDictionary();
    mDictionary->populateFromFile(dict_filename);
    // Create the subscription manager
    PlaybackDQPublisherCallback* managerCallback =  new PlaybackDQPublisherCallback();
    MamaDQPublisherManager* mPubManager = new MamaDQPublisherManager();
    mPubManager->create(transport, Mama::getDefaultEventQueue(mBridgeImpl),
                             managerCallback, source);
    managerCallback->setDictionaryResponder(transport, mDictionary);
    MamaBasicSubscription  subscription;
    subscription.createBasic(transport, Mama::getDefaultEventQueue(mBridgeImpl), (MamaBasicSubscriptionCallback*)managerCallback, "_MDDD.WOMBAT.DATA_DICT");

    Mama::startBackground(mBridgeImpl, NULL);

    for (i = 0; (rewind < 0 || i < rewind); i++)
    {
    	playBackFile(playback_filename, managerCallback, rate);
    	if((i+1) < rewind)
    	{
    		printf("End of playback: Rewinding...\n");
    	}
    	else
    		printf("End of playback - no further rewinds necessary... Bye!!\n");
    }



}
