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


#include <framework/MamaRecordedMessageCodec.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

MamaRecordedMessageCodec::MamaRecordedMessageCodec()
{
	mMamaMsgIterator = new MamaMsgIterator(NULL);
	mTempMamaPrice = new MamaPrice();
	mTempMamaDateTime = new MamaDateTime();
	mTempCMamaDateTime = new mamaDateTime;
	// Maximum size for U16
	mReusableStringBuffer = (char*)malloc(65535);
	mTempMamaMsg = new MamaMsg();
	mOriginalBuffer = NULL;
}

MamaRecordedMessageCodec::~MamaRecordedMessageCodec()
{
    delete mMamaMsgIterator;
    delete mTempMamaPrice;
    delete mTempMamaDateTime;
    delete mTempCMamaDateTime;
    free(mReusableStringBuffer);
}

uint32_t MamaRecordedMessageCodec::encodeMsg(char* buffer, MamaMsg* mm)
{
	char* orig_buffer = buffer;
	// Leave 4 bytes for the message size
	buffer += 4;

	uint32_t r = 4;
	MamaMsgIterator* mmi = new MamaMsgIterator(NULL);
    mm->begin(*mmi);
    while (*(*mmi) != NULL)
    {
    	MamaMsgField& mmf = *(*mmi);
    	try
    	{
    		r += encodeField(buffer, mmf);
    	}
		catch(MamaStatus& ms)
		{
			// Pretend this field never happened...
			//printf("Skipping encoding of MAMA Field - MAMA exception encountered: %s\n", ms.toString());
		}
    	//printf("r = %d\n", r);
        ++(*mmi);
    }
    delete mmi;
    // Populate the first 4 bytes with the message size
    uint32_t buffer_size = (uint32_t)r;
    //printf("Encoding Msg of size [%d]...\n", buffer_size);
    memcpy(orig_buffer, &buffer_size, 4);
    return r;
}

uint32_t MamaRecordedMessageCodec::encodeField(char*& buffer, MamaMsgField& mmf)
{

	uint32_t r = 0;

	mamaFieldType mft = mmf.getType();
	// These modifications will be applied regardless of data type
	mama_fid_t mff = mmf.getFid();

	memcpy(buffer, &mff, 2);
	buffer += 2;
	memcpy(buffer, &mft, 4);
	buffer += 4;
	r += 6;

	switch(mft)
	{
	case MAMA_FIELD_TYPE_BOOL:
		r += encodeBool(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_CHAR:
		r += encodeChar(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_I8:
		r += encodeI8(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_U8:
		r += encodeU8(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_I16:
		r += encodeI16(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_U16:
		r += encodeU16(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_I32:
		r += encodeI32(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_U32:
		r += encodeU32(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_I64:
		r += encodeI64(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_U64:
		r += encodeU64(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_F32:
		r += encodeF32(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_F64:
		r += encodeF64(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_TIME:
		r += encodeTime(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_PRICE:
		r += encodePrice(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_STRING:
		r += encodeString(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_VECTOR_STRING:
		r += encodeVectorString(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_VECTOR_I32:
		r += encodeVectorI32(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_VECTOR_F64:
		r += encodeVectorF64(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_MSG:
		r += encodeMsgField(buffer, mmf);
		break;
	case MAMA_FIELD_TYPE_VECTOR_MSG:
		r += encodeVectorMsg(buffer, mmf);
		break;
	default:
		break;
	}
	return r;
}

uint32_t MamaRecordedMessageCodec::encodeBool(char*& buffer, MamaMsgField& mmf)
{
	bool mfv = mmf.getBool();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding Boolean Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeChar(char*& buffer, MamaMsgField& mmf)
{
	char mfv = mmf.getChar();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding Char Field [%d] = %c...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeI8(char*& buffer, MamaMsgField& mmf)
{
	mama_i8_t mfv = mmf.getI8();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding I8 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeU8(char*& buffer, MamaMsgField& mmf)
{
	mama_u8_t mfv = mmf.getU8();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding U8 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeI16(char*& buffer, MamaMsgField& mmf)
{
	mama_i16_t mfv = mmf.getI16();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding I16 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeU16(char*& buffer, MamaMsgField& mmf)
{
	mama_u16_t mfv = mmf.getU16();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding U16 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeI32(char*& buffer, MamaMsgField& mmf)
{
	mama_i32_t mfv = mmf.getI32();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding I32 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeU32(char*& buffer, MamaMsgField& mmf)
{
	mama_u32_t mfv = mmf.getU32();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding U32 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeI64(char*& buffer, MamaMsgField& mmf)
{
	mama_i64_t mfv = mmf.getI64();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding I64 Field [%d] = %d...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeU64(char*& buffer, MamaMsgField& mmf)
{

	mama_u64_t mfv = mmf.getU64();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding U64 Field [%d] = ..%llu...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeF32(char*& buffer, MamaMsgField& mmf)
{
	mama_f32_t mfv = mmf.getF32();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding F32 Field [%d] = %f...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeF64(char*& buffer, MamaMsgField& mmf)
{
	mama_f64_t mfv = mmf.getF64();
	size_t s = sizeof(mfv);
	memcpy(buffer, &mfv, s);
	buffer += s;
	//printf("Encoding U16 Field [%d] = %g...\n", s, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeTime(char*& buffer, MamaMsgField& mmf)
{
	mmf.getDateTime(*mTempMamaDateTime);

	// Get the C version as well because some functionality is missing from C++ for some reason
	*mTempCMamaDateTime = mTempMamaDateTime->getCValue();

	mama_u32_t seconds = 0;											// 4 bytes
	mama_u32_t microseconds = 0;									// 4 bytes
	mamaDateTimePrecision precision = (mamaDateTimePrecision)0;		// 4 bytes
	mama_u8_t hints = 0;											// 1 byte

	if(mTempMamaDateTime->hasDate())
		hints = hints | MAMA_DATE_TIME_HAS_DATE;
	if(mTempMamaDateTime->hasTime())
		hints = hints | MAMA_DATE_TIME_HAS_TIME;

	mamaDateTime_getWithHints(*mTempCMamaDateTime, &seconds,
			&microseconds, &precision, &hints);

	memcpy(buffer, &seconds, 4);
	buffer += 4;
	memcpy(buffer, &microseconds, 4);
	buffer += 4;
	memcpy(buffer, &precision, 4);
	buffer += 4;
	memcpy(buffer, &hints, 1);
	buffer += 1;

	//printf("Encoding DateTime Field [%d] = %d.%d (%d:%d)...\n", 13, seconds, microseconds, hints, precision);

	return 13;
}

uint32_t MamaRecordedMessageCodec::encodePrice(char*& buffer, MamaMsgField& mmf)
{
	// Creates format of [8 bytes value|4 bytes mamaPricePrecision|1 byte mamaPriceHints]
	mmf.getPrice(*mTempMamaPrice);
	double v = mTempMamaPrice->getValue();
	mamaPricePrecision mpp = mTempMamaPrice->getPrecision();
	mamaPriceHints mph = mTempMamaPrice->getHints();
	printf("value: %d, value: %d, value: %d\n", sizeof(v), sizeof(mpp), sizeof(mph));
	size_t s = sizeof(v) + sizeof(mpp) + sizeof(mph);
	memcpy(buffer, &v, sizeof(v));
	memcpy(buffer, &mpp, sizeof(mpp));
	memcpy(buffer, &mph, sizeof(mph));
	buffer += s;
	//printf("Encoding MamaPrice Field [%d] = %g...\n", s, v);
	return s;
}

uint32_t MamaRecordedMessageCodec::encodeString(char*& buffer, MamaMsgField& mmf)
{
	const char* mfv = mmf.getString();
	mama_u16_t s = (mama_u16_t)strlen(mfv);
	// Add the size to the buffer as this is a non-fixed width field type
	memcpy(buffer, &s, 2);
	buffer += 2;
	memcpy(buffer, mfv, s);
	buffer += s;
	//printf("Encoding String Field[%d] = %s...\n", s, mfv);
	return (s+2);
}

uint32_t MamaRecordedMessageCodec::encodeVectorString(char*& buffer, MamaMsgField& mmf)
{
	//printf("Encoding Vector String Field...\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::encodeVectorI32(char*& buffer, MamaMsgField& mmf)
{
	//printf("Encoding Vector I32 Field...\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::encodeVectorF64(char*& buffer, MamaMsgField& mmf)
{
	//printf("Encoding Vector F64 Field...\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::encodeMsgField(char*& buffer, MamaMsgField& mmf)
{
	//printf("Encoding Unsupported Message Field...\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::encodeVectorMsg(char*& buffer, MamaMsgField& mmf)
{
	const MamaMsg** mmpp = NULL;
	char* orig_buffer = buffer;
	// Reserve these four bytes for the vector size
	buffer += 4;
	MamaMsg* mmp = NULL;
	mama_size_t count = 0;
	uint16_t r = 4;
	uint16_t msg_size = 0;

	mmf.getVectorMsg(mmpp, count);

	if(count == 0)
	{
		return 0;
	}
	uint16_t msg_length = (uint16_t)count;
	for(mama_size_t i = 0; i < count; i++)
	{
		mmp = (MamaMsg*)*mmpp;
		//printf("Encountering Message: %s\n", mmp->toString());
		msg_size += encodeMsg(buffer, mmp);
		r += msg_size;
		buffer += msg_size;
		mmpp++;
	}
	// Now we know the total vector size, write it in.
	memcpy(orig_buffer, &r, 2);
	memcpy(orig_buffer+2, &msg_length, 2);

	//printf("Encoded [%d] Vector Message Fields of size [%d]...\n", count, r);
	return (uint32_t)r;
}

uint32_t MamaRecordedMessageCodec::decodeMsg(char* buffer, MamaMsg* mm)
{
	uint32_t buffer_size = 0;
	// include buffer size in initialization of return integer
	uint32_t r = 4;
	int i = 0;
	int i_limit = 50;

	// Establish the buffer size...
	memcpy(&buffer_size, buffer, 4);
	buffer += 4;
	//prinft("Decoding Msg of size [%d]...\n", buffer_size);
	while(r < buffer_size && i < i_limit)
	{
		try
		{
			r += decodeField(buffer, mm);
		}
		catch(MamaStatus& ms)
		{
			// Pretend this field never happened...
			//printf("Skipping decoding of MAMA Field - MAMA exception encountered: %s\n", ms.toString());
		}

		//printf("r = %d [%d]\n", r, buffer_size);
		i++;
	}

	return r;
}

uint32_t MamaRecordedMessageCodec::decodeField(char*& buffer, MamaMsg* mm)
{
	// These modifications will be applied regardless of data type
	mamaFieldType mft = (mamaFieldType)0;
	mama_fid_t mff = 0;
	// Extract the fid
	memcpy(&mff, buffer, 2);
	buffer += 2;
	// Extract the field type
	memcpy(&mft, buffer, 4);
	buffer += 4;
	// 6 bytes already decoded...
	uint32_t r = 6;

	switch(mft)
	{
	case MAMA_FIELD_TYPE_BOOL:
		r += decodeBool(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_CHAR:
		r += decodeChar(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_I8:
		r += decodeI8(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_U8:
		r += decodeU8(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_I16:
		r += decodeI16(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_U16:
		r += decodeU16(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_I32:
		r += decodeI32(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_U32:
		r += decodeU32(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_I64:
		r += decodeI64(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_U64:
		r += decodeU64(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_F32:
		r += decodeF32(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_F64:
		r += decodeF64(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_TIME:
		r += decodeTime(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_PRICE:
		r += decodePrice(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_STRING:
		r += decodeString(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_VECTOR_STRING:
		r += decodeVectorString(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_VECTOR_I32:
		r += decodeVectorI32(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_VECTOR_F64:
		r += decodeVectorF64(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_MSG:
		r += decodeMsgField(buffer, mm, mff);
		break;
	case MAMA_FIELD_TYPE_VECTOR_MSG:
		r += decodeVectorMsg(buffer, mm, mff);
		break;
	default:
		break;
	}

	return r;
}

uint32_t MamaRecordedMessageCodec::decodeBool(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	bool mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding Boolean Field [%d] = %d...\n", s, mfv);
	mm->addBoolean(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeChar(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	char mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding Char Field [%d] = %c...\n", s, mfv);
	mm->addChar(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeI8(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_i8_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding I8 Field [%d] = %d...\n", s, mfv);
	mm->addI8(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeU8(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_u8_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding U8 Field [%d] = %d...\n", s, mfv);
	mm->addU8(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeI16(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_i16_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding I16 Field [%d] = %d...\n", s, mfv);
	mm->addI16(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeU16(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_u16_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding U16 Field [%d] = %d...\n", s, mfv);
	mm->addU16(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeI32(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_i32_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding I32 Field [%d] = %d...\n", s, mfv);
	mm->addI32(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeU32(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_u32_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding U32 Field [%d] = %d...\n", s, mfv);
	mm->addU32(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeI64(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_i64_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding I64 Field [%d] = %d...\n", s, mfv);
	mm->addI64(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeU64(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	// correctly interpret U64 on avis
	mama_u64_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding U64 Field [%d] = %d...\n", s, mfv);
	// TODO: Wait on bug fix http://bugs.openmama.org/show_bug.cgi?id=4 so we can actually
	//mm->addU64(NULL, mff, mfv);
	mm->addU32(NULL, mff, (mama_u32_t)mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeF32(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_f32_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding F32 Field [%d] = %f...\n", s, mfv);
	mm->addF32(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeF64(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mama_f64_t mfv = 0;
	size_t s = sizeof(mfv);
	memcpy(&mfv, buffer, s);
	buffer += s;
	//prinft("Decoding F64 Field [%d] = %d...\n", s, mfv);
	mm->addF64(NULL, mff, mfv);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeTime(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mTempMamaDateTime->clear();

	mama_u32_t seconds = 0;											// 4 bytes
	mama_u32_t microseconds = 0;									// 4 bytes
	mamaDateTimePrecision precision = (mamaDateTimePrecision)0;		// 4 bytes
	mama_u8_t hints = 0;											// 1 byte

	memcpy(&seconds, buffer, 4);
	buffer += 4;
	memcpy(&microseconds, buffer, 4);
	buffer += 4;
	memcpy(&precision, buffer, 4);
	buffer += 4;
	memcpy(&hints, buffer, 1);
	buffer += 1;

	mTempMamaDateTime->setWithHints(seconds, microseconds, precision, hints);

	// Additional processing which really should have been taken care of by the
	// above MAMA function
	if((hints & MAMA_DATE_TIME_HAS_DATE) == 0x00)
		mTempMamaDateTime->clearDate();
	if((hints & MAMA_DATE_TIME_HAS_TIME) == 0x00)
			mTempMamaDateTime->clearTime();

	// TODO: Wait on bug fix http://bugs.openmama.org/show_bug.cgi?id=4 so we can actually
	//mm->addDateTime(NULL, mff, *mTempMamaDateTime);
	mm->addU32(NULL, mff, seconds);


	//prinft("Decoding DateTime Field [%d] = %d.%d (%d:%d)...\n", 13, seconds, microseconds, hints, precision);

	return 13;
}

uint32_t MamaRecordedMessageCodec::decodePrice(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	mTempMamaPrice->clear();
	double v = 0;
	mamaPricePrecision mpp = (mamaPricePrecision)0;
	mamaPriceHints mph = (mamaPriceHints)0;
	size_t v_s = sizeof(v);
	size_t mpp_s = sizeof(mpp);
	size_t mph_s = sizeof(mph);
	size_t s = v_s + mpp_s + mph_s;
	memcpy(&v, buffer, v_s);
	buffer += v_s;
	memcpy(&mpp, buffer, mpp_s);
	buffer += mpp_s;
	memcpy(&mph, buffer, mph_s);
	buffer += mph_s;
	mTempMamaPrice->set(v, mph);
	mTempMamaPrice->setPrecision(mpp);
	mm->addPrice(NULL, mff, *mTempMamaPrice);
	//prinft("Decoding MamaPrice Field [%d] = %g...\n", s, v);
	return s;
}

uint32_t MamaRecordedMessageCodec::decodeString(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	char* mfv = mReusableStringBuffer;
	mama_u16_t s = (mama_u16_t)0;
	// First two bytes are the string size
	memcpy(&s, buffer, 2);
	buffer += 2;
	// Copy this many bytes to the reusable string buffer
	memcpy(mfv, buffer, s);
	buffer += s;
	// Null terminate the buffer
	mfv[s] = '\0';
	mm->addString(NULL, mff, mfv);
	//prinft("Decoding String Field[%d] = %s...\n", s, mfv);
	return (s+2);
}

uint32_t MamaRecordedMessageCodec::decodeVectorString(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	//printf("*NOT* Decoding Vector String\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::decodeVectorI32(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	//printf("*NOT* Decoding Vector I32\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::decodeVectorF64(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	//printf("*NOT* Decoding Vector F64\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::decodeMsgField(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	//printf("*NOT* Decoding Message Field\n");
	// This format is not even supported by the avis payload at the moment, so skipping...
	return 0;
}

uint32_t MamaRecordedMessageCodec::decodeVectorMsg(char*& buffer, MamaMsg* mm, mama_fid_t mff)
{
	return 0;

	uint16_t vector_size = 0;
	uint16_t vector_length = 0;
	uint32_t r = 4;
	uint32_t msg_size = 0;
	memcpy(&vector_size, buffer, 2);
	buffer += 2;
	memcpy(&vector_length, buffer, 2);
	buffer += 2;
	MamaMsg** vectorMsg = new MamaMsg*[vector_length];

	//prinft("Decoding Vector Message of size %d\n", vector_size+4);

    for (int i=0; i< vector_length; i++)
	{
    	//prinft("Decoding Vector Message [%d]...\n", i);
	    vectorMsg[i] = new MamaMsg;
	    vectorMsg[i]->create();
		msg_size += decodeMsg(buffer, vectorMsg[i]);
		buffer += msg_size;
		r += msg_size;
	}
    mm->addVectorMsg(NULL, mff, vectorMsg, vector_length);
    for (int i=0; i< vector_length; i++)
	{
    	vectorMsg[i]->clear();
		delete vectorMsg[i];
	}
    delete vectorMsg;

	return r;
}
