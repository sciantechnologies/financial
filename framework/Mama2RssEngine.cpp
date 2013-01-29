#include <framework/Mama2RssEngine.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>


Mama2RssEngine::Mama2RssEngine()
{

}

Mama2RssEngine::~Mama2RssEngine()
{

}

void Mama2RssEngine::setMamaDictionary(MamaDictionary* dict)
{
	mMamaDictionary = dict;
}

string Mama2RssEngine::expandVariable(string s, MamaMsg* mm, Mama2RssPropertyCache* m2rpc)
{
	printf("Expanding variable: %s = ", s.c_str());
	MamaFieldDescriptor* mfd = NULL;
	if(mm != NULL)
	{
		try
		{
			mfd = mMamaDictionary->getFieldByName(s.c_str());
		}
		catch(MamaStatus& ms)
		{
			printf("MAMA Exception: %s\n", ms.toString());
		}
		if(mfd != NULL && mm->tryField(mfd))
		{
			MamaMsgField* mmf = mm->getField(mfd);
			char buf[MAX_MAMA_FIELD_VALUE_LENGTH];
			mmf->getAsString(buf, MAX_MAMA_FIELD_VALUE_LENGTH);
			printf("%s\n", buf);
			return buf;
		}
	}
	// Check if it's one of our reserved values
	if(mm == NULL || mfd == NULL)
	{
		if(s == "MAMA_SOURCE_NAME")
		{
			printf("%s\n", m2rpc->MAMA_SOURCE_NAME.c_str());
			return m2rpc->MAMA_SOURCE_NAME;
		}
		else if(s == "MAMA_MIDDLEWARE_NAME")
		{
			printf("%s\n", m2rpc->MAMA_MIDDLEWARE_NAME.c_str());
			return m2rpc->MAMA_MIDDLEWARE_NAME;
		}
		else if(s == "MAMA_SYMBOL_NAME")
		{
			printf("%s\n", m2rpc->MAMA_SYMBOL_NAME.c_str());
			return m2rpc->MAMA_SYMBOL_NAME;
		}
		else if(s == "MAMA_TRANSPORT_NAME")
		{
			printf("%s\n", m2rpc->MAMA_TRANSPORT_NAME.c_str());
			return m2rpc->MAMA_TRANSPORT_NAME;
		}
		else if(s == "CURRENT_TIME")
		{
			printf("%s\n", m2rpc->CURRENT_TIME.c_str());
			return m2rpc->CURRENT_TIME;
		}
		else if(s == "URI")
		{
			printf("%s\n", m2rpc->URI.c_str());
			return m2rpc->URI;
		}
		else if(s == "WEB_SERVER_PORT")
		{
			printf("%s\n", m2rpc->WEB_SERVER_PORT.c_str());
			return m2rpc->WEB_SERVER_PORT;
		}
		else if(s == "WEB_SERVER_NAME")
		{
			printf("%s\n", m2rpc->WEB_SERVER_NAME.c_str());
			return m2rpc->WEB_SERVER_NAME;
		}
		else
		{
			printf("UNDEFINED\n");
		}
	}
	string r = "UNDEFINED";
	return r;
}

string Mama2RssEngine::expandVariables(string s, MamaMsg* mm, Mama2RssPropertyCache* m2rpc)
{
	printf("Expanding variables: %s = ", s.c_str());
	size_t start_pos = 0, end_pos = 0;
	// invar flags if currently parsing a variable
	bool invar = false;
	while(start_pos != string::npos && end_pos != string::npos)
	{
		if(!invar)
		{
			start_pos = s.find("$(", end_pos);
			invar = true;
		}
		else
		{
			end_pos = s.find(")", start_pos);
			// If we have a complete match
			if(end_pos != string::npos)
			{
				string var_name = s.substr(start_pos+2, (end_pos - start_pos - 2));
				string ev = expandVariable(var_name, mm, m2rpc);
				s.replace(start_pos, (end_pos - start_pos + 1), ev);
				end_pos = start_pos + ev.length();
			}
			invar = false;
		}
	}
	printf("[[%s]]\n", s.c_str());
	return s;
}

string Mama2RssEngine::populateRssHeaderTemplate(MamaRssTemplate* mrt, Mama2RssPropertyCache* m2rpc)
{
	string r = "";
	r.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	r.append("<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	r.append("    <channel>\n");
	r.append("        <atom:link href=\"");
	r.append(expandVariables(mrt->global_link, NULL, m2rpc));
	r.append("\" rel=\"self\" type=\"application/rss+xml\" />\n");
	r.append("        <title>");
	r.append(expandVariables(mrt->global_title, NULL, m2rpc));
	r.append("</title>  \n        <link>");
	r.append(expandVariables(mrt->global_link, NULL, m2rpc));
	//r.append("http://fq.scian.com:8080/wmw/subscribe/TEST/MSFT.ARCA");
	r.append("</link>  \n        <description>");
	r.append(expandVariables(mrt->global_description, NULL, m2rpc));
	r.append("</description>  \n        <lastBuildDate>");
	r.append(expandVariables(mrt->global_lastBuildDate, NULL, m2rpc));
	r.append("</lastBuildDate>  \n        <language>en-gb</language>  \n");
	return r;
}

string Mama2RssEngine::populateRssFooterTemplate(MamaRssTemplate* mrt, Mama2RssPropertyCache* m2rpc)
{
	string r = "    </channel>  \n</rss>\n";
	return r;
}

string Mama2RssEngine::populateRssItemTemplate(MamaRssTemplate* mrt, MamaMsg* mm, Mama2RssPropertyCache* m2rpc)
{
	string r = "";
	r.append("        <item>\n          <title>");
	r.append(expandVariables(mrt->item_title, mm, m2rpc));
	r.append("</title>  \n          <description>");
	r.append(expandVariables(mrt->item_description, mm, m2rpc));
	r.append("</description>  \n          <link>");
	r.append(expandVariables(mrt->item_link, mm, m2rpc));
	r.append("</link>  \n          <guid isPermaLink=\"false\">");
	r.append(expandVariables(mrt->item_guid, mm, m2rpc));
	r.append("</guid>  \n          <pubDate>");
	r.append(expandVariables("$(CURRENT_TIME)", mm, m2rpc));
	r.append("</pubDate>  \n        </item>\n");
	return r;
}

MamaRssTemplate* Mama2RssEngine::readMamaRssTemplate(const char* xml_template)
{
    // populate tree structure pt
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(xml_template, pt);
    MamaRssTemplate* mrt = new MamaRssTemplate;
    size_t s = 1;
    long template_size = 0;
    // Get the file size of the template for slurping to memory (should only be small)
    FILE * xmlTemplateFileHandle;
    xmlTemplateFileHandle = fopen(xml_template, "rb");
    fseek (xmlTemplateFileHandle , 0 , SEEK_END);
    template_size = ftell (xmlTemplateFileHandle);
    rewind (xmlTemplateFileHandle);
    char* buffer = (char*) malloc (sizeof(char)*template_size);
    fread (buffer, 1, template_size, xmlTemplateFileHandle);
    mrt->full_text = buffer;

    BOOST_FOREACH( ptree::value_type const& v, pt.get_child("rss.channel") )
    {
    	try{
			if(v.first == "item")
			{
				mrt->item_title = v.second.get<std::string>("title");
				mrt->item_description = v.second.get<std::string>("description");
				mrt->item_link = v.second.get<std::string>("link");
				mrt->item_guid = v.second.get<std::string>("guid");
				mrt->item_pubDate = v.second.get<std::string>("pubDate");
			}
			else if(v.first == "title")
			{
				mrt->global_title = v.second.data();
			}
			else if(v.first == "description")
			{
				mrt->global_description = v.second.data();
			}
			else if(v.first == "link")
			{
				mrt->global_link = v.second.data();
			}
			else if(v.first == "lastBuildDate")
			{
				mrt->global_lastBuildDate = v.second.data();
			}
			else if(v.first == "pubDate")
			{
				mrt->global_pubDate = v.second.data();
			}
			s++;
    	}
    	catch(...)
    	{
    		printf("Error parsing rss/channel/%s, offset [%d]\n", v.first.c_str(), s);
    	}
    }
    return mrt;
}

