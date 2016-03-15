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

	PowerOff();
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

void Ts_OTPWriteBootSector::Execute()
{
	uint8_t arOTP[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
	//_pSyn_DutCtrl->FpOtpRomRead();

}

void Ts_OTPWriteBootSector::ProcessData()
{

}

void Ts_OTPWriteBootSector::CleanUp()
{
}

void Ts_OTPWriteBootSector::WriteBootSector(int nSection, uint8_t* pOtpBootSector, uint8_t* pBootSectorMask)
{
	uint8_t		arBootSectorWithCmd[BS0_SIZE] = { 0 };
	uint8_t		pSwapped[BS0_SIZE];
	int			arrIncrement[4] = { 3, 1, -1, -3 };

	//Put in 4 bytes of command-specific data.
	/*arBootSectorWithCmd[0] = nSection;
	arBootSectorWithCmd[1] = 1;
	arBootSectorWithCmd[2] = 10;
	arBootSectorWithCmd[3] = 0;*/
	memcpy(&arBootSectorWithCmd, pOtpBootSector, BS0_SIZE);

	if (pBootSectorMask != NULL)
	{
		//First, swap the bytes in the mask.
		for (int i = 0; i < BS0_SIZE; i++)
			pSwapped[i] = pBootSectorMask[i + arrIncrement[i % 4]];

		//Remember, the boot sectors are OTP. For the even long words, we can only write a
		//0 to a 1. For the odd long words, we can only write a 1 to a 0.
		for (int i = 0; i<BS0_SIZE; i++)
		{
			//If this byte belongs to an odd long word.
			if ((i / 4) & 0x01)
				arBootSectorWithCmd[i] = pSwapped[i] & pOtpBootSector[i];
			else
				arBootSectorWithCmd[i] = pSwapped[i] | pOtpBootSector[i];
		}
	}

	//Write the boot sector.  
	_pSyn_DutCtrl->FpWrite(1, 0x3C, arBootSectorWithCmd, BS0_SIZE);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);
}