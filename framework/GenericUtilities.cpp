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
