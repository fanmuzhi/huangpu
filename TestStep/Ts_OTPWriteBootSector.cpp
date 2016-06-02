#include "Ts_OTPWriteBootSector.h"

Ts_OTPWriteBootSector::Ts_OTPWriteBootSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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

	//Power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

	_pSyn_DutCtrl->FpUnloadPatch();

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

	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, otpBs0, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, otpBs1, BS1_SIZE);

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

	_pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 0, otpBs0, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 1, otpBs1, BS0_SIZE);
}

void Ts_OTPWriteBootSector::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteBootSector", "Pass"));
}

void Ts_OTPWriteBootSector::CleanUp()
{
	PowerOff();
}