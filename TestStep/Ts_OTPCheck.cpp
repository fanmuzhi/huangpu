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
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

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

	bool bPass = true;

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

	if (!bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sBootSectorFail);
		_pSyn_Dut->_pSyn_DutTestResult->_otpCheckResult._bPass = false;
	}

	//Check 0 (Verify OTP Boot Field)
    //Result Index =, 3  (530,  Si Osc Freq Setting)
    //Check Data Size =, 8
    //Boot Sector =, 0
    //Address =, 4
    //Left Shift Bits =, 0
    //And Mask =, 0x000003ff
	

	//Check 1 (Verify OTP Boot Field)
    //Result Index =, 4  (540,  Si Osc Temp Comp)
    //Check Data Size =, 8
    //Boot Sector =, 0
    //Address =, 4
    //Left Shift Bits =, 12
    //And Mask =, 0x0000003f

	//Check 2 (Verify OTP Boot Field)
    //Result Index =, 5  (550,  HV Osc Bias)
    //Check Data Size =, 8
    //Boot Sector =, 0
    //Address =, 4
    //Left Shift Bits =, 20
    //And Mask =, 0x00000007

	//Check 3 (Verify OTP Boot Field)
    //Result Index =, 6  (560,  HV Osc Freq)
    //Check Data Size =, 8
    //Boot Sector =, 0
    //Address =, 4
    //Left Shift Bits =, 23
    //And Mask =, 0x0000001f

	//Check 4 (Verify OTP Boot Field)
    //Result Index =, 7  (570,  Boot 0 Check Trim Valid Bits)
    //Check Data Size =, 8
    //Boot Sector =, 0
    //Address =, 4
    //Left Shift Bits =, 28
    //And Mask =, 0x0000000f

	//Check 5 (Verify OTP SN Non-Zero)
    //Result Index =, 8  (580,  SN non-zero (0=No)(1=Yes))

	//Check 6 (Verify OTP Tag Header)
    //Result Index =, 9  (590,  LNA Offset Cal Tag (1=Found))
    //Search Flags =, 0x03
    //Check Data Size =, 8
    //Tag Type =, 14
    //Flags =, 0x06
    //Check Tag Valid =, Yes
    //Check Extended Tag =, 0x80000003

	//Check 7 (Verify OTP Tag Header)
    //Result Index =, 10  (600,  PGA Offset Cal Tag(1=Found))
    //Search Flags =, 0x03
    //Check Data Size =, 8
    //Tag Type =, 14
    //Flags =, 0x06
    //Check Tag Valid =, Yes
    //Check Extended Tag =, 0x80000007

	//Check 8 (Read SNR From OTP Tag)
    //Result Index =, 11  (610,  SNR Tag Found (1=Found))
    //Search Flags =, 0x03

	//Check 9 (Read Cal Revision from OTP Tag)
    //Result Index =, 15  (650,  Cal Rev Tag Found)
    //Search Flags =, 0x03
    //PatchId =, 31
    //  Entry =, 0 (xe)
    //    Label =, MT_Patch_6_07_164 xe
    //    Length =, 10260
    //    CRC =, 0x867f
    //  Entry =, 1 (xp)
    //    Label =, MT_Patch_6_07_164 xp
    //    Length =, 10260
    //    CRC =, 0x641c
}

void Ts_OTPCheck::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}
