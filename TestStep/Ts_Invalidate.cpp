#include "Ts_Invalidate.h"

Ts_Invalidate::Ts_Invalidate(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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

	//power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

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
										EXT_TAG_WOF_BOT,
										EXT_TAG_DutTempAdc,
										EXT_TAG_WOF_TOP,
										EXT_TAG_PGA_OOPP,
										EXT_TAG_SCM_WOF_BOT,
										EXT_TAG_SCM_WOF_TOP,
										EXT_TAG_PART_NUMBERS};

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
	PowerOff();
}

void Ts_Invalidate::Invalidate(uint32_t tagId)
{
	uint8_t pDst[MS0_SIZE] = {0};
	int count_threshold = 1;
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
	char extended_tag[4];
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

		case EXT_TAG_WOF_BOT:
			extended_tag[0] = 0x09;
			break;

		case EXT_TAG_DutTempAdc:
			extended_tag[0] = 0x0A;
			break;

		case EXT_TAG_WOF_TOP:
			extended_tag[0] = 0x0B;
			break;

		case EXT_TAG_PGA_OOPP:
			extended_tag[0] = 0x0C;
			break;

		case EXT_TAG_SCM_WOF_BOT:
			extended_tag[0] = 0x0D;
			break;
			
		case EXT_TAG_SCM_WOF_TOP:
			extended_tag[0] = 0x0E;
			break;

		case EXT_TAG_PART_NUMBERS:
			extended_tag[0] = 0x0F;
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