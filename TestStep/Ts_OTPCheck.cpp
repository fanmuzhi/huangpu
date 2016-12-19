#include "Ts_OTPCheck.h"

#include <regex>

Ts_OTPCheck::Ts_OTPCheck(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
, bPass(true)
{
}

Ts_OTPCheck::~Ts_OTPCheck()
{
}

void Ts_OTPCheck::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PRODUCT_ID = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SNR = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPR = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_FlexId = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE0 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE1 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE0 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE1 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE0 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE1 = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_DutTempAdc = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPP = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PART_NUMBERS = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA_PGA_GAINS = 1;

	std::vector<std::string> listOfArgValue;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bExecuted = false;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 15)
	{
		for (size_t t = 1; t <= 15 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PRODUCT_ID= atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SNR= atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPR = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_FlexId= atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE0 = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE1 = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE0 = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE1 = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_DutTempAdc= atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPP= atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE0= atoi(listOfArgValue[11].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE1= atoi(listOfArgValue[12].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PART_NUMBERS= atoi(listOfArgValue[13].c_str());
	if (0 != listOfArgValue[12].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA_PGA_GAINS = atoi(listOfArgValue[14].c_str());

	//load OTPReadWritePatch
	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);
}

void Ts_OTPCheck::Execute()
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

	uint32_t rc(0);

	uint8_t arOTP[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
	int iSize(BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE);

	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, arOTP, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, &arOTP[BS0_SIZE], BS1_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, &arOTP[BS0_SIZE + BS1_SIZE], MS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &arOTP[BS0_SIZE + BS1_SIZE + MS0_SIZE], MS1_SIZE);

	for (auto i = 0; i < BS0_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i] = arOTP[i];
	}
	for (auto i = 0; i < BS1_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector1Array[i] = arOTP[i + BS0_SIZE];
	}
	for (auto i = 0; i < MS0_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._MainSector0Array[i] = arOTP[i + BS0_SIZE + BS1_SIZE];
	}
	for (auto i = 0; i < MS1_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._MainSector1Array[i] = arOTP[i + BS0_SIZE + BS1_SIZE + MS0_SIZE];
	}

	uint8_t pDst[MS0_SIZE];
	//int nTags(0);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PRODUCT_ID)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PRODUCT_ID, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nProductId_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_PRODUCT_ID is failed!");
			throw ex;
		}
		
		uint32_t projID = *((uint32_t*)&pDst[4]);
		if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId != projID)
		{
			bPass = false;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_PRODUCT_ID", HexToString(pDst, 4, 7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNA_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_LNA is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_LNA", HexToString(pDst, 0, _pSyn_Dut->_RowNumber+4)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SNR)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SNR, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nSNR_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_SNR is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_SNR", HexToString(pDst,0, 3)));

	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPR)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPR, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPR_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_PGA_OOPR is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_PGA_OOPR", HexToString(pDst,0, _pSyn_Dut->_RowNumber+4)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_FlexId)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_FlexId, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nFlexId_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_FlexId is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_FlexId", HexToString(pDst, 0, 1)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE0)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FD_ZONE0, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE0_FD_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_WOF_FD_ZONE0 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_WOF_FD_ZONE0", HexToString(pDst, 4, 7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE1)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FD_ZONE1, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE1_FD_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_WOF_FD_ZONE1 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_WOF_FD_ZONE1", HexToString(pDst, 4, 7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE0)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FU_ZONE0, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE0_FU_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_WOF_FU_ZONE0 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_WOF_FU_ZONE0", HexToString(pDst, 4, 7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE1)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FU_ZONE1, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE1_FU_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_WOF_FU_ZONE1 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_WOF_FU_ZONE1", HexToString(pDst, 4, 7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_DutTempAdc)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_DutTempAdc, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nDutTempAdc_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_DutTempAdc is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_DutTempAdc", HexToString(pDst, 0, 1)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPP)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPP_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_PGA_OOPP is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_PGA_OOPP", HexToString(pDst, 0, NUM_PGA_OOPP_OTP_ROWS * (_pSyn_Dut->_ColumnNumber - HEADER) + 4)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE0)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_ZONE0, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nScmWofBot_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_SCM_WOF_ZONE0 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_SCM_WOF_ZONE0", HexToString(pDst,4,7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE1)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_ZONE1, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nScmWofTop_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_SCM_WOF_ZONE1 is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_SCM_WOF_ZONE1", HexToString(pDst, 4,7)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PART_NUMBERS)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PART_NUMBERS, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPartNumberId_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_PART_NUMBERS is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_PART_NUMBERS", HexToString(pDst, 0, 19)));
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA_PGA_GAINS)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA_PGA_GAINS, pDst, MS0_SIZE, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNAPGAGains_count);
		if (0 != rc)
		{
			ex.SetError(rc);
			ex.SetDescription("OtpRomTagRead EXT_TAG_LNA_PGA_GAINS is failed!");
			throw ex;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_mapMainSectorInfo.insert(std::map<std::string, std::string>::value_type("EXT_TAG_LNA_PGA_GAINS", HexToString(pDst, 0, 11)));
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bExecuted = true;
}

void Ts_OTPCheck::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep OTPCheck is not executed!");
		throw ex;
	}
	_pSyn_Dut->_pSyn_DutTestResult->_otpCheckResult._bPass = true;

	// check first 4 bytes in bootsector0
	for (auto i = 0; i < 4; i++)
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i]
			!= _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i])
		{
			bPass = false;
		}
		if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i + 8]		
			!= _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i + 8])	// the second row
		{
			bPass = false;
		}
	}

	//check OSC trim valid bit (mask: 0xFX 0xXX 0xXX 0xXX)
	if ((_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[19] & 0xF0) != 0xF0)
	{
		bPass = false;
	}
	if ((_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[27] & 0xF0) != 0xF0)
	{
		bPass = false;
	}

	// check last 4 byts
	for (auto i = 48; i < 52; i++)
	{
		//ignore the regcheck bit
		if (i == 49)
		{
			int temp = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i] ^ _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i];
			int temp2 = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i + 8] ^ _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i + 8];
			if ( (temp != 0x40) && (temp != 0))
			{
				bPass = false;
			}
			if ( (temp2 != 0x40) && (temp2 != 0))
			{
				bPass = false;
			}
		}
		else
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i]
				!= _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i])
			{
				bPass = false;
			}
			if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._BootSector0Array[i + 8]
				!= _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i + 8])	// the second row
			{
				bPass = false;
			}
		}
	}

	//check count
	int count_threshold = 1;
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PRODUCT_ID)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nProductId_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNA_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SNR)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nSNR_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPR)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPR_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_FlexId)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nFlexId_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE0)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE0_FD_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FD_ZONE1)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE1_FD_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE0)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE0_FU_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_WOF_FU_ZONE1)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE1_FU_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_DutTempAdc)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nDutTempAdc_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PGA_OOPP)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPP_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE0)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nScmWofBot_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_SCM_WOF_ZONE1)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nScmWofTop_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_PART_NUMBERS)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPartNumberId_count < count_threshold)
			bPass = false;
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bCheckTAG_LNA_PGA_GAINS)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNAPGAGains_count < count_threshold)
			bPass = false;
	}

	if (!bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sBootSectorFail);
		_pSyn_Dut->_pSyn_DutTestResult->_otpCheckResult._bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPCheck", "Fail"));
		return;
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPCheck", "Pass"));

}

void Ts_OTPCheck::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}

string Ts_OTPCheck::HexToString(uint8_t arrValue[],int stratPos,int endPos)
{
	char c[20];

	string strValue;
	for (int i = stratPos; i <= endPos; i++)
	{
		int length = sprintf(c, "%02X", arrValue[i]);
		for (int j = 0; j < length; j++)
		{
			strValue.push_back(c[j]);
		}

		if (i != endPos)
			strValue += ",";
	}

	return strValue;
}

bool Ts_OTPCheck::GetConfigFileArray(string sConfigFileName, uint8_t *pConfigFilerArray, int arrSize)
{
	const std::regex pattern("(\\d{3})-(\\d{6})-(\\d{2})r(\\d{1,2})");
	std::smatch results;
	if (std::regex_search(sConfigFileName, results, pattern))
	{
		std::string sFileName = results.str();
		//std::cout << sFileName << std::endl;
		TranslatePartNum(sFileName, pConfigFilerArray);
	}
	else
	{
		return false;
	}

	return true;
}

void Ts_OTPCheck::TranslatePartNum(const string& sPN, uint8_t* pDst)
{
	string        sTemp;

	memset(pDst, 0, MAX_PART_NUMBER_LENGTH);

	//With one character following the 'r', valid PNs have at least 15 characters.
	if (sPN.length() >= 15)
	{
		//Remove all '-' characters up to the 'r'.
		sTemp = "0";
		sTemp += sPN.substr(0, 3);
		sTemp += sPN.substr(4, 6);
		sTemp += sPN.substr(11, 2);

		//The 1 or 2 characters following the 'r' can be either 0-99 or A-Z.
		char   nChr1 = (sPN.substr(14)).at(0);
		if ((nChr1 >= 'A') && (nChr1 <= 'Z'))
		{
			pDst[7] = 0x9A + (nChr1 - 'A');
		}
		else
		{
			if ((nChr1 >= '0') && (nChr1 <= '9'))
			{
				pDst[7] = nChr1 - '0';
				if (sPN.length() >= 16)    //If there are two revision characters.
				{
					char   nChr2 = (sPN.substr(15)).at(0);
					if ((nChr2 >= '0') && (nChr2 <= '9'))
						pDst[7] = (pDst[7] << 4) + nChr2 - '0';
				}
			}
		}

		//Put the translated part number into given buffer. Convert to BCD.
		for (int i = 0; i < (MAX_PART_NUMBER_LENGTH - 2); i++)
		{
			pDst[i] = sTemp.at(i * 2) << 4;
			pDst[i] += sTemp.at((i * 2) + 1) & 0x0F;
		}
	}
}