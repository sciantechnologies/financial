#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <mama/mamacpp.h>

using namespace Wombat;
using namespace boost::interprocess;

#ifndef __H_GenericMamaCache
#define __H_GenericMamaCache

class GenericMamaCache
{
public:
	GenericMamaCache ();
    ~GenericMamaCache();
    void applyUpdate(MamaMsg& msg);
    void clear();
    MamaMsg* getMsg();
private:
    MamaMsg* mMsg;
    MamaMsgIterator* mMamaMsgIterator;
    //boost::mutex mMutex;
};

#endif
