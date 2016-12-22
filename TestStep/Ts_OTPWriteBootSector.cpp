#include "Ts_OTPWriteBootSector.h"

Ts_OTPWriteBootSector::Ts_OTPWriteBootSector(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_OTPWriteBootSector::~Ts_OTPWriteBootSector()
{
}

void Ts_OTPWriteBootSector::SetUp()
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

void Ts_OTPWriteBootSector::Execute()
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

	uint8_t otpBs0[BS0_SIZE] = { 0 };
	uint8_t otpBs1[BS1_SIZE] = { 0 };
	//_pSyn_DutCtrl->FpOtpRomRead();

	uint32_t rc(0);

	rc = _pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, otpBs0, BS0_SIZE);
	if (0 != rc)
	{
		Syn_Exception ex(rc);
		ex.SetDescription("BOOT_SEC0::FpOtpRomTagRead is failed!");
		throw ex;
		return;
	}
	rc = _pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, otpBs1, BS1_SIZE);
	if (0 != rc)
	{
		Syn_Exception ex(rc);
		ex.SetDescription("BOOT_SEC1::FpOtpRomTagRead is failed!");
		throw ex;
		return;
	}

	//Communication Option and flash bit
	int index = 51;
	int mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index];
	if ((otpBs0[index] | mask) == mask)		//check if writable
	{
		otpBs0[index] = mask;
		otpBs0[index + 8] = mask;
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sOtpReadWriteFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteBootSector", "Fail"));
	}

	//FW secure
	index = 50;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x40;
	if (mask)
	{
		otpBs0[index] |= 0x40;
		otpBs0[index + 8] |= 0x40;
	}

	//HW secure
	index = 1;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x2;
	if (mask)
	{
		otpBs0[index] |= 0x2;
		otpBs0[index + 8] |= 0x2;
	}

	//Sleep Enable
	index = 49;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x10;
	if (mask)
	{
		otpBs0[index] |= 0x10;
		otpBs0[index + 8] |= 0x10;
	}

	//serial number
	otpBs1[49] = otpBs1[49+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[4];
	otpBs1[48] = otpBs1[48+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[5];
	otpBs1[35] = otpBs1[35+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[0];
	otpBs1[34] = otpBs1[34+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[1];
	otpBs1[33] = otpBs1[33+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[2];
	otpBs1[32] = otpBs1[32+8] = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number[3];

	/*
	//OSC trim
	if (_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.m_bExecuted)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_bPass)
		{
			uint8_t trimVal1 = ((_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_nOscTrim & 0xFFFFF) >> 8) & 0xFF;
			uint8_t trimVal2 = ((_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_nOscTrim & 0xFFFFF)) & 0xFF;
			//uint8_t arSiOscTrimValid[2]	= {0x0, 0x10};
			otpBs0[20] = otpBs0[28]	= trimVal1;
			otpBs0[21] = otpBs0[29]	= trimVal2;
			otpBs0[22] = otpBs0[30]	= 0x0;
			otpBs0[23] = otpBs0[31]	= 0x10;
		}
	}

	//TC trim
	if (_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.m_bExecuted)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_bPass)
		{
			otpBs0[19] |= 0x20;
			otpBs0[27] |= 0x20;
		}
	}

	//slow OSC trim
	if (_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_bExecuted)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_bPass)
		{
			uint8_t hvTrimValue1 =  ((_pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_nTrim << 7) | (_pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_nBias << 4));
			uint8_t hvTrimValue2 =  (0x0F & (_pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_nTrim >> 1)) | (0x80 | 0x40);
			otpBs0[18] = otpBs0[26] = otpBs0[18] | hvTrimValue1;
			otpBs0[19] = otpBs0[27] = otpBs0[19] | hvTrimValue2;
		}
	}
	*/

	//write to boot secotr
	//maybe 0x403,do not care
	rc = _pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 0, otpBs0, BS0_SIZE);
	rc = _pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 1, otpBs1, BS0_SIZE);
}

void Ts_OTPWriteBootSector::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteBootSector", "Pass"));
}

void Ts_OTPWriteBootSector::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}