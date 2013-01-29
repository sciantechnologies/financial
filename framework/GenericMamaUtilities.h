
#include <mama/mamacpp.h>

using namespace Wombat;

#ifndef __H_GenericMamaUtilities
#define __H_GenericMamaUtilities

class GenericMamaUtilities
{
public:
    static bool addOrUpdateField(MamaMsgField* mmf, MamaMsg& msg);
};

#endif
