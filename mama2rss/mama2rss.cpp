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

#include <mongoose/mongoose.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <unistd.h>
#include <framework/GenericUtilities.h>
#include <framework/MamaCachingCallback.h>
#include <framework/Mama2RssEngine.h>

#define		MAX_NUM_MIDDLEWARES				5
#define		MAMA_2_RSS_APP_NAME				"mama2rss"

GenericMamaManager* mGenericMamaManager = NULL;
MamaRssTemplate* mMamaRssTemplate = NULL;
map<string, Mama2RssPropertyCache*> mMama2RssPropertyCache;
mg_context* mMgContext;
const char* APPLICATION_NAME = "";


static void* web_callback(enum mg_event event, struct mg_connection *conn,
		const struct mg_request_info *request_info)
{
	Mama2RssServerSettings* m2rss = (Mama2RssServerSettings*)request_info->user_data;
	Mama2RssEngine* mMama2RssEngine = m2rss->mMama2RssEngine;

	void* vp = NULL;
	char cp[1];
	cp[0] = '\0';
	vp = cp;

	if (event == MG_NEW_REQUEST)
	{
		string request_response = "";
		char* request_uri = request_info->uri;
		printf("Request received for %s\n", request_uri);
		time_t rawtime;
		struct tm * timeinfo;

		// Get browser-friendly time format
		char buffer[80];
		time(&rawtime);
		timeinfo = gmtime(&rawtime);
		strftime (buffer, 80, "%a, %d %b %Y %X GMT", timeinfo);

		// Get time 24 hours from now
		char buffer_24hrs[80];
		rawtime += 86400;
		timeinfo = gmtime(&rawtime);
		strftime (buffer_24hrs, 80, "%a, %d %b %Y %X GMT", timeinfo);
		// Echo requested URI back to the client

		mg_printf(conn, "HTTP/1.1 200 OK\r\n"
				"Last-Modified: %s\r\n"
				"Content-Type: text/xml\r\n"
				"Cache-Control: max-age=53\r\n"
				"Expires: %s\r\n"
				"Date: %s\r\n"
				"Connection: keep-alive\r\n"
				"Connection: Transfer-Encoding\r\n\r\n", buffer, buffer_24hrs, buffer);
		printf("[%s]\n", request_uri);

		vector<string> vs;
		size_t token_count = GenericUtilities::tokenizeString(request_uri, vs, "/");
		if(token_count == 5)
		{
			string middleware = vs[1];
			string transport = vs[2];
			string source = vs[3];
			string symbol_concat = vs[4];
			vector<string> symbol_list;
			size_t symbol_count = GenericUtilities::tokenizeString(symbol_concat.c_str(), symbol_list, "+");
			Mama2RssPropertyCache* m2rpc = NULL;
			for (size_t sc = 0; sc < symbol_count; sc++)
			{
				string symbol = symbol_list[sc];
				string smapkey = middleware;
				smapkey.append("/");
				smapkey.append(transport);
				smapkey.append("/");
				smapkey.append(source);
				smapkey.append("/");
				smapkey.append(symbol);
				map<string, Mama2RssPropertyCache*>::iterator m2rpcit;
				m2rpcit = mMama2RssPropertyCache.find(smapkey);
				if(m2rpcit == mMama2RssPropertyCache.end())
				{
					printf("Creating new property cache for %s\n", symbol.c_str());
					m2rpc = new Mama2RssPropertyCache;
					mMama2RssPropertyCache[smapkey] = m2rpc;
				}
				else
				{
					printf("Reusing existing property cache for %s\n", symbol.c_str());
					m2rpc = m2rpcit->second;
				}

				MamaSubscription* mama_sub = mGenericMamaManager->getExistingMamaSubscription(
						middleware.c_str(), transport.c_str(), source.c_str(), symbol.c_str());
				// If subscription doesn't exist, set one up
				if(mama_sub == NULL)
				{
					printf("Creating new subscription for %s\n", symbol.c_str());
					void* mscb = NULL;
					mscb = new MamaCachingCallback();
					m2rpc->MAMA_SOURCE_NAME = source;
					m2rpc->MAMA_SYMBOL_NAME = symbol;
					m2rpc->MAMA_TRANSPORT_NAME = transport;
					m2rpc->WEB_SERVER_NAME = m2rss->mUrlRoot;
					m2rpc->WEB_SERVER_PORT = m2rss->mPort;
					m2rpc->MAMA_MIDDLEWARE_NAME = middleware;
					m2rpc->URI = request_uri;

					mama_sub = mGenericMamaManager->getMamaSubscription(middleware.c_str(),
							transport.c_str(), (MamaSubscriptionCallback*)mscb, source.c_str(), symbol.c_str());
				}
				else
				{
					printf("Reusing existing subscription for %s\n", symbol.c_str());
				}
				// Extract the callback from this subscription
				MamaCachingCallback* mccb = (MamaCachingCallback*)mama_sub->getCallback();

				m2rpc->CURRENT_TIME = buffer;

				// Get a copy of the latest MAMA Message from this cache
				MamaMsg* mm = mccb->getMamaMsgCopy();
				string s = "";
				printf("CHECKING mm != NULL\n");
				if(sc == 0)
				{
					request_response = mMama2RssEngine->populateRssHeaderTemplate(mMamaRssTemplate, m2rpc);
				}
				if(mm != NULL)
				{
					printf("Populating RSS header\n");
					// Populate required XML with the formatted output

					printf("Populating RSS ITEM\n");
					request_response.append(mMama2RssEngine->populateRssItemTemplate(mMamaRssTemplate, mm, m2rpc));
					printf("Populating RSS Footer\n");
				}
			}
			request_response.append(mMama2RssEngine->populateRssFooterTemplate(mMamaRssTemplate, m2rpc));
			printf("Done populating RSS\n");
			mg_printf(conn, "%s", request_response.c_str());
		}

		return vp;  // Mark as processed
	}
	else
	{
		return NULL;
	}
}

void usage()
{
	printf("\nUsage: mama2rss [OPTION]... \n\
RSS web server / portal for transcoding MAMA data.\n\
\n\
 -m [middleware] \tMiddlewares to load (may be passed multiple times)\n\
 -queues [count]\tNumber of MAMA queues to use per bridge\n\
 -p [port]\t\tPort on which to listen for web requests\n\
 -t [XML file]\t\tPath to XML template file used for RSS feed population\n\
 -DF [Dict file] \tPath to ascii data dictionary file\n\
 -h [Host Name] \tServer name for RSS clients (e.g. IP Address or FQDN)\n\
\n\
URLs are in the form of http://server:port/middleware/transport/source/symbol\n\n");
}

int main(int argc, const char** argv)
{
	Mama2RssEngine* mMama2RssEngine = new Mama2RssEngine();
	MamaDictionary* mMamaDictionary = NULL;
	APPLICATION_NAME = argv[0];
	const char* middlewares[MAX_NUM_MIDDLEWARES];
	int mw_offset = 0;
	unsigned int queues = 1;
	const char* port = "80";
	const char* template_file = "";
	const char* dict_filename = "";
	char urlroot[1024];
	urlroot[1023] = '\0';
	gethostname(urlroot, 1023);

	Mama2RssServerSettings* m2rss = new Mama2RssServerSettings;

    for (int i = 1; i < argc;)
    {
        if (strcmp (argv[i], "-m") == 0)
        {
            middlewares[mw_offset] = argv[i + 1];
            mw_offset++;
            i += 2;
        }
        else if (strcmp (argv[i], "-queues") == 0)
		{
        	const char* qs = argv[i + 1];
			queues = atoi(qs);
			i += 2;
		}
        else if (strcmp (argv[i], "-p") == 0)
		{
        	port = argv[i + 1];
			i += 2;
		}
        else if (strcmp (argv[i], "-t") == 0)
		{
        	template_file = argv[i + 1];
        	mMamaRssTemplate = mMama2RssEngine->readMamaRssTemplate(template_file);
			i += 2;
		}
        else if (strcmp (argv[i], "-DF") == 0)
		{
        	dict_filename = argv[i + 1];
        	mMamaDictionary = new MamaDictionary();
			mMamaDictionary->populateFromFile(dict_filename);
			i += 2;
		}
        else if (strcmp (argv[i], "-h") == 0)
		{
			strcpy(urlroot, argv[i + 1]);
			i += 2;
		}
    }

    if(mw_offset == 0)
    {
    	usage();
    	return (1);
    }
    m2rss->mPort = port;
    m2rss->mUrlRoot = urlroot;

    middlewares[mw_offset] = NULL;

	mGenericMamaManager = new GenericMamaManager();
	mGenericMamaManager->create(middlewares, queues);
	mGenericMamaManager->start();
	m2rss->mGenericMamaManager = mGenericMamaManager;
	m2rss->mMama2RssEngine = mMama2RssEngine;
	mMama2RssEngine->setMamaDictionary(mMamaDictionary);

	// Placeholder for the upcoming program
	const char *options[] = {"listening_ports", port, NULL};

	mMgContext = mg_start(&web_callback, (void*)m2rss, options);
	getchar();  // Wait until user hits "enter"
	mg_stop(mMgContext);

	return 0;
}
