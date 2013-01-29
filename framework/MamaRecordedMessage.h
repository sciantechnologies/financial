#ifndef __H_MamaRecordedMessage
#define __H_MamaRecordedMessage

#include <mama/mamacpp.h>
#include <framework/MamaRecordedMessageCodec.h>


// Default Meta Tag Version
#define		MAMA_RECORDED_MSG_DEFAULT_META			1

// Default Meta Tag Version
#define		MAMA_RECORDED_MSG_MAX_SYMBOL_LENGTH		256

using namespace std;
using namespace Wombat;

class MamaRecordedMessage
{
public:
	MamaRecordedMessage ();
    ~MamaRecordedMessage ();
    bool populate(char* msg_buffer, int header_size);
    void dump();
    void setBridgeFormat(mamaBridge& mb);
    MamaMsg* getMamaMsg(char*& symbol);
private:
    int getMetaVersion(char*& meta_start_buf);
    long double getTimeStamp(char*& timestamp_start_buf);
    unsigned long int getMsgBodySize(char*& msgbodysize_start_buf);
    char* getSymbol(char*& symbol_start_buf, int symbol_length);
    MamaMsg* mCachedMamaMsg;
    MamaRecordedMessageCodec* mMessageCodecEngine;
    char* mSymbol;
    long double mTimeStamp;
    int mMetaVersion;
    unsigned long int mMessageSize;
    mamaBridge mMsgFormatBridge;
    char* mEncodedMessage;
};
#endif
