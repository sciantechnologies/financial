#include <framework/GenericUtilities.h>
#include <stdlib.h>
#include <string.h>

size_t GenericUtilities::tokenizeString(const char* input, vector<string>& output, const char* delim)
{
	if(delim == NULL)
		delim = " ";
	char* p_it = const_cast<char*>(input);
	char* p_st = const_cast<char*>(input);
	char buf[256];
	size_t s = strlen(delim);
	size_t buflen = 0;
	size_t matches = 0;
	char* p_end = p_it + strlen(input);
	while(*p_it != '\0')
	{
		// If this matches the end of a
		if(strncmp(p_it, delim, s) == 0)
		{
			p_end = p_it;
			buflen = p_end - p_st;
			memcpy(buf, p_st, buflen);
			buf[buflen] = '\0';
			output.push_back(buf);
			matches++;
			p_it += s;
			p_st = p_it;

		}
		else
			p_it++;
	}
	// This is required to catch the final segment
	if(p_it != (p_end+s))
	{
		// The source already contains '\0' so not required to append
		memcpy(buf, p_st, (p_it-p_st+1));
		p_end = p_it;
		output.push_back(buf);
		matches++;
	}
	return matches;
}
