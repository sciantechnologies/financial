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
