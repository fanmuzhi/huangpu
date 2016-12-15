#include "Ts_Invalidate.h"

Ts_Invalidate::Ts_Invalidate(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_Invalidate::~Ts_Invalidate()
{
}

void Ts_Invalidate::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	//load ImgAcqPatch
	Syn_PatchInfo OtpReadWritePatchInfo;
	if (!(_pSyn_Dut->FindPatch("OtpReadWritePatch", OtpReadWritePatchInfo)) || 0 == OtpReadWritePatchInfo._uiArraySize)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch is NULL!");
		throw ex;
		return;
	}

	_pSyn_DutCtrl->FpLoadPatch(OtpReadWritePatchInfo._pArrayBuf, OtpReadWritePatchInfo._uiArraySize);
}

void Ts_Invalidate::Execute()
{
	uint32_t tag_array[NUM_EXT_TAGS] = {EXT_TAG_PRODUCT_ID,
										EXT_TAG_LNA,
										EXT_TAG_SNR,
										EXT_TAG_PGA_OOPR,
										EXT_TAG_FlexId,
										EXT_TAG_WOF_FD_ZONE0,
										EXT_TAG_WOF_FD_ZONE1,
										EXT_TAG_WOF_FU_ZONE0,
										EXT_TAG_WOF_FU_ZONE1,
										EXT_TAG_DutTempAdc,
										EXT_TAG_PGA_OOPP,
										EXT_TAG_SCM_WOF_ZONE0,
										EXT_TAG_SCM_WOF_ZONE1,
										EXT_TAG_PART_NUMBERS,
										EXT_TAG_LNA_PGA_GAINS };

	for (int i = 0; i < NUM_EXT_TAGS; i++)
	{
		Invalidate(tag_array[i]);
	}
}

void Ts_Invalidate::ProcessData()
{
}

void Ts_Invalidate::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}

void Ts_Invalidate::Invalidate(uint32_t tagId)
{
	uint8_t pDst[MS0_SIZE] = {0};
	unsigned int count_threshold = 1;
	int timeout(10);

	while (timeout&&!(_pSyn_DutCtrl->FpOtpRomTagRead(tagId, pDst, MS0_SIZE) < count_threshold))
	{
		char Argument_to_write[2052] = { 0 };
		int	numBytes(0);
		get_invalidate_arguments(tagId,&Argument_to_write[0], numBytes);

		_pSyn_DutCtrl->FpOtpRomTagWrite((uint8_t*)Argument_to_write, numBytes);
		
		timeout--;
	}
}

void Ts_Invalidate::get_invalidate_arguments(uint32_t tagId, char* outString, int &oNumBytes)
{
	uint8_t extended_tag[4];
	extended_tag[1] = 0x00;
	extended_tag[2] = 0x00;
	extended_tag[3] = 0x80;

	switch (tagId)
	{
		case EXT_TAG_PRODUCT_ID:
			extended_tag[0] = 0x02;
			extended_tag[1] = 0x00;
			extended_tag[2] = 0x00;
			extended_tag[3] = 0x00;
			break;

		case EXT_TAG_LNA:
			extended_tag[0] = 0x03;
			break;

		case EXT_TAG_SNR:
			extended_tag[0] = 0x05;
			break;

		case EXT_TAG_PGA_OOPR:
			extended_tag[0] = 0x07;
			break;

		case EXT_TAG_FlexId:
			extended_tag[0] = 0x08;
			break;

		case EXT_TAG_WOF_FD_ZONE0:
			extended_tag[0] = 0x09;
			break;

		case EXT_TAG_DutTempAdc:
			extended_tag[0] = 0x0A;
			break;

		case EXT_TAG_WOF_FD_ZONE1:
			extended_tag[0] = 0x0B;
			break;

		case EXT_TAG_PGA_OOPP:
			extended_tag[0] = 0x0C;
			break;

		case EXT_TAG_SCM_WOF_ZONE0:
			extended_tag[0] = 0x0D;
			break;
			
		case EXT_TAG_SCM_WOF_ZONE1:
			extended_tag[0] = 0x0E;
			break;

		case EXT_TAG_PART_NUMBERS:
			extended_tag[0] = 0x0F;
			break;

		case EXT_TAG_LNA_PGA_GAINS:
			extended_tag[0] = 0x12;
			break;

		case EXT_TAG_WOF_FU_ZONE0:
			extended_tag[0] = 0x13;
			break;

		case EXT_TAG_WOF_FU_ZONE1:
			extended_tag[0] = 0x14;
			break;

		default:
			Syn_Exception ex(0);
			ex.SetError(Syn_ExceptionCode::Syn_ERR_FP_INVALID_PARAMS);
			ex.SetDescription("Unknown TAG_CAL value!");
			throw ex;
			return;
	}

	outString[0] = 0;
	outString[1] = 0;
	outString[2] = 0;
	outString[3] = 0;

	outString[4] = 0x21;
	outString[5] = 0;
	outString[6] = 0;
	outString[7] = 0;

	outString[8] = 0;
	outString[9] = 0;
	outString[10] = 0;
	outString[11] = 0;

	outString[12] = extended_tag[0];
	outString[13] = extended_tag[1];
	outString[14] = extended_tag[2];
	outString[15] = extended_tag[3];

	outString[16] = 0;
	outString[17] = 0;

	outString[18] = 0x0E;

	outString[19] = 0x00;

	outString[20] = 0x00;

	//DUMMY BYTES
	outString[21] = 0;
	outString[22] = 0;
	outString[23] = 0;
	//DUMMY BYTES

	oNumBytes = 24;
}