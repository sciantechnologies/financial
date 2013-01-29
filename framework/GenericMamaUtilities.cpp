#include <framework/GenericMamaUtilities.h>

bool GenericMamaUtilities::addOrUpdateField(MamaMsgField* mmf, MamaMsg& msg)
{
	mamaFieldType mft = mmf->getType();
	// These modifications will be applied regardless of data type
	mama_fid_t mff = mmf->getFid();

	switch(mft)
	{
	case MAMA_FIELD_TYPE_BOOL:
		{
		bool bool_v = true;
		if(msg.tryBoolean(NULL, mff, bool_v))
			msg.updateBoolean(NULL, mff, mmf->getBool());
		else
			msg.addBoolean(NULL, mff, mmf->getBool());
		}
		break;
	case MAMA_FIELD_TYPE_CHAR:
		{
		char char_v = ' ';
		if(msg.tryChar(NULL, mff, char_v))
			msg.updateChar(NULL, mff, mmf->getChar());
		else
			msg.addChar(NULL, mff, mmf->getChar());
		}
		break;
	case MAMA_FIELD_TYPE_I8:
		{
		mama_i8_t i8_v = 0;
		if(msg.tryI8(NULL, mff, i8_v))
			msg.updateI8(NULL, mff, mmf->getI8());
		else
			msg.addI8(NULL, mff, mmf->getI8());
		}
		break;
	case MAMA_FIELD_TYPE_U8:
		{
		mama_u8_t u8_v = 0;
		if(msg.tryU8(NULL, mff, u8_v))
			msg.updateU8(NULL, mff, mmf->getU8());
		else
			msg.addU8(NULL, mff, mmf->getU8());
		}
		break;
	case MAMA_FIELD_TYPE_I16:
		{
		mama_i16_t i16_v = 0;
		if(msg.tryI16(NULL, mff, i16_v))
			msg.updateI16(NULL, mff, mmf->getI16());
		else
			msg.addI16(NULL, mff, mmf->getI16());
		}
		break;
	case MAMA_FIELD_TYPE_U16:
		{
		mama_u16_t u16_v = 0;
		if(msg.tryU16(NULL, mff, u16_v))
			msg.updateU16(NULL, mff, mmf->getU16());
		else
			msg.addU16(NULL, mff, mmf->getU16());
		}
		break;
	case MAMA_FIELD_TYPE_I32:
		{
		mama_i32_t i32_v = 0;
		if(msg.tryI32(NULL, mff, i32_v))
			msg.updateI32(NULL, mff, mmf->getI32());
		else
			msg.addI32(NULL, mff, mmf->getI32());
		}
		break;
	case MAMA_FIELD_TYPE_U32:
		{
		mama_u32_t u32_v = 0;
		if(msg.tryU32(NULL, mff, u32_v))
			msg.updateU32(NULL, mff, mmf->getU32());
		else
			msg.addU32(NULL, mff, mmf->getU32());
		}
		break;
	case MAMA_FIELD_TYPE_I64:
		{
		mama_i64_t i64_v = 0;
		if(msg.tryI64(NULL, mff, i64_v))
			msg.updateI64(NULL, mff, mmf->getI64());
		else
			msg.addI64(NULL, mff, mmf->getI64());
		}
		break;
	case MAMA_FIELD_TYPE_U64:
		{
		mama_u64_t u64_v = 0;
		if(msg.tryU64(NULL, mff, u64_v))
			msg.updateU64(NULL, mff, mmf->getU64());
		else
			msg.addU64(NULL, mff, mmf->getU64());
		}
		break;
	case MAMA_FIELD_TYPE_F32:
		{
		mama_f32_t f32_v = 0;
		if(msg.tryF32(NULL, mff, f32_v))
			msg.updateF32(NULL, mff, mmf->getF32());
		else
			msg.addF32(NULL, mff, mmf->getF32());
		}
		break;
	case MAMA_FIELD_TYPE_F64:
		{
		mama_f64_t f64_v = 0;
		if(msg.tryF64(NULL, mff, f64_v))
			msg.updateF64(NULL, mff, mmf->getF64());
		else
			msg.addF64(NULL, mff, mmf->getF64());
		}
		break;
	case MAMA_FIELD_TYPE_TIME:
		{
		MamaDateTime* mTempMamaDateTime = new MamaDateTime();
		mmf->getDateTime(*mTempMamaDateTime);
		if(msg.tryDateTime(NULL, mff, *mTempMamaDateTime))
			msg.updateDateTime(NULL, mff, *mTempMamaDateTime);
		else
			msg.addDateTime(NULL, mff, *mTempMamaDateTime);
		}
		break;
	case MAMA_FIELD_TYPE_PRICE:
		{
		MamaPrice* mTempMamaPrice = new MamaPrice();
		mmf->getPrice(*mTempMamaPrice);
		if(msg.tryPrice(NULL, mff, *mTempMamaPrice))
			msg.updatePrice(NULL, mff, *mTempMamaPrice);
		else
			msg.addPrice(NULL, mff, *mTempMamaPrice);
		}
		break;
	case MAMA_FIELD_TYPE_STRING:
		{
		const char* string_v = mmf->getString();
		if(msg.tryString(NULL, mff, string_v))
			msg.updateString(NULL, mff, string_v);
		else
			msg.addString(NULL, mff, string_v);
		}
		break;
	// Lets leave vectors out of caching for the moment... Order books can be rebuilt
	case MAMA_FIELD_TYPE_VECTOR_STRING:
	case MAMA_FIELD_TYPE_VECTOR_I32:
	case MAMA_FIELD_TYPE_VECTOR_F64:
	case MAMA_FIELD_TYPE_MSG:
	case MAMA_FIELD_TYPE_VECTOR_MSG:
	default:
		break;
	}

	return true;
}
