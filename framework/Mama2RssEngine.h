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
#include <framework/GenericMamaManager.h>

#define		MAX_MAMA_FIELD_VALUE_LENGTH		8192

using namespace std;
using namespace Wombat;

#ifndef __H_Mama2RssEngine
#define __H_Mama2RssEngine

class Mama2RssEngine;

struct MamaRssTemplate
{
	string full_text;
    string item_title;
    string item_description;
    string item_link;
    string item_guid;
    string item_pubDate;
    string global_title;
    string global_description;
    string global_link;
    string global_pubDate;
    string global_lastBuildDate;
};

struct Mama2RssPropertyCache
{
	string MAMA_SOURCE_NAME;
	string MAMA_SYMBOL_NAME;
	string MAMA_TRANSPORT_NAME;
	string MAMA_MIDDLEWARE_NAME;
	string CURRENT_TIME;
	string URI;
	string WEB_SERVER_NAME;
	string WEB_SERVER_PORT;
};

struct Mama2RssServerSettings
{
	string mPort;
	string mUrlRoot;
	MamaRssTemplate* mMamaRssTemplate;
	GenericMamaManager* mGenericMamaManager;
	Mama2RssEngine* mMama2RssEngine;
};

class Mama2RssEngine
{
public:
	Mama2RssEngine ();
    ~Mama2RssEngine ();
    MamaRssTemplate* readMamaRssTemplate(const char* xml_template);
    string populateRssItemTemplate(MamaRssTemplate* mrt, MamaMsg* mm, Mama2RssPropertyCache* m2rpc);
    string populateRssFooterTemplate(MamaRssTemplate* mrt, Mama2RssPropertyCache* m2rpc);
    string populateRssHeaderTemplate(MamaRssTemplate* mrt, Mama2RssPropertyCache* m2rpc);
    string expandVariables(string s, MamaMsg* mm, Mama2RssPropertyCache* m2rpc);
    string expandVariable(string s, MamaMsg* mm, Mama2RssPropertyCache* m2rpc);
    void setMamaDictionary(MamaDictionary* dict);
private:
    MamaDictionary* mMamaDictionary;
};
#endif
