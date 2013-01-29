#include <vector>
#include <string>

using namespace std;

#ifndef __H_GenericUtilities
#define __H_GenericUtilities

class GenericUtilities
{
public:
    static size_t tokenizeString(const char* input, vector<string>& output, const char* delim);
};

#endif
