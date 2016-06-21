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

	//Parse Args
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PRODUCT_ID = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SNR = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPR = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_FlexId = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE0 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE1 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE0 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE1 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE0 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE1 = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_DutTempAdc = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPP = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PART_NUMBERS = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA_PGA_GAINS = 0;

	std::vector<std::string> listOfArgValue;
	_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bExecuted = false;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 15)
	{
		for (size_t t = 1; t <= 15 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PRODUCT_ID = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SNR = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPR = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_FlexId = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE0 = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE1 = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE0 = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE1 = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_DutTempAdc = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPP = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE0 = atoi(listOfArgValue[11].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE1 = atoi(listOfArgValue[12].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PART_NUMBERS = atoi(listOfArgValue[13].c_str());
	if (0 != listOfArgValue[12].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA_PGA_GAINS = atoi(listOfArgValue[14].c_str());

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
	std::vector<uint32_t> listOfTag;

	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PRODUCT_ID)
		listOfTag.push_back(EXT_TAG_PRODUCT_ID);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA)
		listOfTag.push_back(EXT_TAG_LNA);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SNR)
		listOfTag.push_back(EXT_TAG_SNR);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPR)
		listOfTag.push_back(EXT_TAG_PGA_OOPR);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_FlexId)
		listOfTag.push_back(EXT_TAG_FlexId);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE0)
		listOfTag.push_back(EXT_TAG_WOF_FD_ZONE0);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FD_ZONE1)
		listOfTag.push_back(EXT_TAG_WOF_FD_ZONE1);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE0)
		listOfTag.push_back(EXT_TAG_WOF_FU_ZONE0);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_WOF_FU_ZONE1)
		listOfTag.push_back(EXT_TAG_WOF_FU_ZONE1);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_DutTempAdc)
		listOfTag.push_back(EXT_TAG_DutTempAdc);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PGA_OOPP)
		listOfTag.push_back(EXT_TAG_PGA_OOPP);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE0)
		listOfTag.push_back(EXT_TAG_SCM_WOF_ZONE0);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_SCM_WOF_ZONE1)
		listOfTag.push_back(EXT_TAG_SCM_WOF_ZONE1);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_PART_NUMBERS)
		listOfTag.push_back(EXT_TAG_PART_NUMBERS);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_invalidateInfo._bInvalidateTAG_LNA_PGA_GAINS)
		listOfTag.push_back(EXT_TAG_LNA_PGA_GAINS);

	for (int i = 1; i <= listOfTag.size(); i++)
	{
		Invalidate(listOfTag[i-1]);
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