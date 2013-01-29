#ifndef __H_MamaRecordedMessageCodec
#define __H_MamaRecordedMessageCodec

#include <mama/mamacpp.h>

using namespace std;
using namespace Wombat;

//             +------+-------+--------------------------------+----+
// Msg Format: |fid[2]|type[1]|size [2 where unknown from type]|data|
//             +------+-------+--------------------------------+----+

class MamaRecordedMessageCodec
{
public:
	MamaRecordedMessageCodec ();
    ~MamaRecordedMessageCodec ();
    // All encode parameters return the number of bytes now occupied
    uint32_t encodeMsg(char* buffer, MamaMsg* mm);
    uint32_t encodeField(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeBool(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeChar(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeI8(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeU8(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeI16(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeU16(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeI32(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeU32(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeI64(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeU64(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeF32(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeF64(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeTime(char*& buffer, MamaMsgField& mmf);
    uint32_t encodePrice(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeString(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeVectorString(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeVectorI32(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeVectorF64(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeMsgField(char*& buffer, MamaMsgField& mmf);
    uint32_t encodeVectorMsg(char*& buffer, MamaMsgField& mmf);
    uint32_t decodeMsg(char* buffer, MamaMsg* mm);
    uint32_t decodeField(char*& buffer, MamaMsg* mm);
    uint32_t decodeBool(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeChar(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeI8(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeU8(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeI16(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeU16(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeI32(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeU32(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeI64(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeU64(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeF32(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeF64(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeTime(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodePrice(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeString(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeVectorString(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeVectorI32(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeVectorF64(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeMsgField(char*& buffer, MamaMsg* mm, mama_fid_t mff);
    uint32_t decodeVectorMsg(char*& buffer, MamaMsg* mm, mama_fid_t mff);
private:
    MamaMsgIterator* mMamaMsgIterator;
    MamaPrice* mTempMamaPrice;
    MamaDateTime* mTempMamaDateTime;
    mamaDateTime* mTempCMamaDateTime;
    MamaMsg* mTempMamaMsg;
    char* mReusableStringBuffer;
    char* mOriginalBuffer;

};
#endif
