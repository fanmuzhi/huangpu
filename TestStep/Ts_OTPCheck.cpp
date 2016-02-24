#include "Ts_OTPCheck.h"


Ts_OTPCheck::Ts_OTPCheck(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, pDutCtrl, pDut)
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

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_OTPCheck::Excute()
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

	uint8_t arOTP[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
	int iSize(BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE);

	//load OTPReadWritePatch
	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);

	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, arOTP, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, &arOTP[BS0_SIZE], BS1_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, &arOTP[BS0_SIZE + BS1_SIZE], MS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &arOTP[BS0_SIZE + BS1_SIZE + MS0_SIZE], MS1_SIZE);

	for (auto i = 0; i < BS0_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._BootSector0Array[i] = arOTP[i];
	}
	for (auto i = 0; i < BS1_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._BootSector1Array[i] = arOTP[i + BS0_SIZE];
	}
	for (auto i = 0; i < MS0_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._MainSector0Array[i] = arOTP[i + BS0_SIZE + BS1_SIZE];
	}
	for (auto i = 0; i < MS1_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._MainSector1Array[i] = arOTP[i + BS0_SIZE + BS1_SIZE + MS0_SIZE];
	}

	_pSyn_DutCtrl->FpUnloadPatch();
}

void Ts_OTPCheck::ProcessData()
{

}

void Ts_OTPCheck::CleanUp()
{
	PowerOff();
}
