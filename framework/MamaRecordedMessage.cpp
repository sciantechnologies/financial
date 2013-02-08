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


#include <framework/MamaRecordedMessage.h>
#include <framework/MamaRecordedMessageCodec.h>
#include <cstdlib>

MamaRecordedMessage::MamaRecordedMessage()
{
    mCachedMamaMsg = new MamaMsg();
    mCachedMamaMsg->create();
    mTimeStamp = 0.0L;
    mMetaVersion = MAMA_RECORDED_MSG_DEFAULT_META;
    mMessageSize = 0;
    mSymbol = (char*)malloc(MAMA_RECORDED_MSG_MAX_SYMBOL_LENGTH);
    mMessageCodecEngine = new MamaRecordedMessageCodec();
    mEncodedMessage = (char*)malloc(40480);
}

MamaRecordedMessage::~MamaRecordedMessage()
{

}

int MamaRecordedMessage::getMetaVersion(char*& meta_start_buf)
{
	char meta_ver_buf[3];
	memcpy(meta_ver_buf, meta_start_buf, 2);
	meta_ver_buf[2] = '\0';
	meta_start_buf += 3;
	return atoi(meta_ver_buf);
}

long double MamaRecordedMessage::getTimeStamp(char*& timestamp_start_buf)
{
	char time_stamp_buf[21];
	memcpy(time_stamp_buf, timestamp_start_buf, 20);
	time_stamp_buf[20] = '\0';
	timestamp_start_buf += 21;
	return atof(time_stamp_buf);
}

unsigned long int MamaRecordedMessage::getMsgBodySize(char*& msgbodysize_start_buf)
{
	char body_size_buf[10];
	memcpy(body_size_buf, msgbodysize_start_buf, 9);
	body_size_buf[9] = '\0';
	msgbodysize_start_buf += 10;
	return atol(body_size_buf);
}

char* MamaRecordedMessage::getSymbol(char*& symbol_start_buf, int symbol_length)
{
	memcpy(mSymbol, symbol_start_buf, symbol_length);
	mSymbol[symbol_length] = '\0';
	symbol_start_buf += symbol_length;
	return mSymbol;
}

void MamaRecordedMessage::setBridgeFormat(mamaBridge& mb)
{
	mMsgFormatBridge = mb;
}

// msg_buffer is expected to include the enclosure meta tags
bool MamaRecordedMessage::populate(char* msg_buffer, int header_size)
{
	mCachedMamaMsg->clear();

	// This will get us past the meta tags
	char* header_buffer = msg_buffer + 5;
	char* msg_body_buffer = msg_buffer + header_size;

	// Extract the meta version as it will impact potential decoding algorithms
	mMetaVersion = this->getMetaVersion(header_buffer);

	switch(mMetaVersion)
	{
	case 1:
		//printf("Found a Meta version of 1...\n");
		mTimeStamp = this->getTimeStamp(header_buffer);
		mMessageSize = this->getMsgBodySize(header_buffer);
		int symbol_length = header_size - (header_buffer - msg_buffer) - 5;
		mSymbol = this->getSymbol(header_buffer, symbol_length);
		break;
	}

	// In header file but annoyingly, not currently implemented
	//mCachedMamaMsg->createFromBuffer((void*)msg_body_buffer, (size_t)mMessageSize);
	uint32_t bytes_written_decode = mMessageCodecEngine->decodeMsg(msg_body_buffer, mCachedMamaMsg);
	//this->dump();
	return true;
}

MamaMsg* MamaRecordedMessage::getMamaMsg(char*& symbol)
{
	symbol = mSymbol;
	return mCachedMamaMsg;
}

void MamaRecordedMessage::dump()
{
	printf("Found header:\n");
	printf("\tSize:\t%i\n", mMessageSize);
	printf("\tStamp:\t%Lf\n", mTimeStamp);
	printf("\tSymbol:\t%s\n", mSymbol);
	printf("\tMessage:\t%s\n", mCachedMamaMsg->toString());
	// Add random price in to test functionality
	/*
	uint32_t bytes_written_encode = mMessageCodecEngine->encodeMsg(mEncodedMessage, mCachedMamaMsg);
	printf("Encoded Bytes Written: %d\n", bytes_written_encode);

	MamaMsg* mm = new MamaMsg();
	mm->clear();
	mm->create();
	//uint32_t bytes_written_decode = mMessageCodecEngine->decodeMsg(mEncodedMessage, mm);
	//printf("Decoded Message: %s\n", mm->toString());
	printf("Decoded Bytes Written: %d\n", bytes_written_decode);
	delete mm;
*/
}
