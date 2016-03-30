#include "Ts_OTPWriteMainSector.h"

Ts_OTPWriteMainSector::Ts_OTPWriteMainSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_OTPWriteMainSector::~Ts_OTPWriteMainSector()
{
}

void Ts_OTPWriteMainSector::SetUp()
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

	//Power On
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

void Ts_OTPWriteMainSector::Execute()
{
	uint8_t	arMS0[MS0_SIZE + MS1_SIZE] = {0};
	bool	bSuccess = true;
	int		nNumCols = _pSyn_Dut->_ColumnNumber;
	int		nLNA_count, nPGA_count, nSNR_count;
	int		nFlexId_count, nWofBot_count, nWofTop_count, nDutTempAdc_count, nPGA_OOPP_count, nScmWofBot_count, nProductId_count;

	//nLNA_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNA_count;
	//nPGA_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPR_count;
	//nSNR_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nSNR_count;
	//nFlexId_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nFlexId_count;
	//nWofBot_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofBot_count;
	nWofTop_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofTop_count;
	//nDutTempAdc_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nDutTempAdc_count;
	//nPGA_OOPP_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPP_count;
	//nScmWofBot_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nScmWofBot_count;
	//nProductId_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nProductId_count;

	//Cablirate
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		//If LNA values have not been stored in the OTP.
		nLNA_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, arMS0, MS0_SIZE);
		if (nLNA_count == 0)
			BurnToOTP(EXT_TAG_LNA, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nLnaIdx], _pSyn_Dut->_RowNumber);

		//If PGA values (one offset per pixel) have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 1)//kPgaCalTypeOneOffsetPerPixel
		{
			nPGA_OOPP_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, arMS0, MS0_SIZE);
			if (nPGA_OOPP_count == 0)
				BurnToOTP(EXT_TAG_PGA_OOPP, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets, NUM_PGA_OOPP_OTP_ROWS * (nNumCols - 8));
		}

		//If PGA (one offset per row) values have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 0)//kPgaCalTypeOneOffsetPerRow
		{
			nPGA_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPR, arMS0, MS0_SIZE);
			if (nPGA_count == 0)
				BurnToOTP(EXT_TAG_PGA_OOPR, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nPgaIdx], _pSyn_Dut->_RowNumber);
		}
	}
	
	//If SNR values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.m_bExecuted)
	{
		nSNR_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SNR, arMS0, MS0_SIZE);
		if (nSNR_count == 0)
		{
			arMS0[0] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_SNR);
			arMS0[1] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_Signal);
			arMS0[2] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_Noise);
			arMS0[3] = 0;
			BurnToOTP(EXT_TAG_SNR, arMS0, 4);
		}
	}
	
	//flexID
	//If flex ID values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted)
	{
		nFlexId_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_FlexId, arMS0, MS0_SIZE);
		if (nFlexId_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId;
			arMS0[1] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId) >> 8);
			BurnToOTP(EXT_TAG_FlexId, arMS0, 8);
		}
	}

	//if (//WOF)
	//If WOF(bottom) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus)
	{
		nWofBot_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_BOT, arMS0, MS0_SIZE);
		if (nWofBot_count == 0)
		{
			if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain100)
			{
				arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_100;
				arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_100;
				arMS0[2] = 100;
				arMS0[3] = 0;
			}
			else
			{
				arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200;
				arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200;
				arMS0[2] = 200;
				arMS0[3] = 0;
			}
			BurnToOTP(EXT_TAG_WOF_BOT, arMS0, 4);
		}
	}

	//if (//SCM_WOF)
	//If SCM WOF(bottom) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithStimulus)
	{
		nScmWofBot_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, arMS0, MS0_SIZE);
		if (nScmWofBot_count == 0)
		{
			if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100)
			{
				arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100;
				arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100;
				arMS0[2] = 100;
				arMS0[3] = 0;
			}
			else
			{
				arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200;
				arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200;
				arMS0[2] = 200;
				arMS0[3] = 0;
			}
			BurnToOTP(EXT_TAG_SCM_WOF_BOT, arMS0, 4);
		}
	}

	//If temperature ADC values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted)
	{
		nDutTempAdc_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_DutTempAdc, arMS0, MS0_SIZE);
		if (nDutTempAdc_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc;
			arMS0[1] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc >> 8);
			BurnToOTP(EXT_TAG_DutTempAdc, arMS0, 4);
		}
	}

	//projID
	if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId != 0)
	{
		nProductId_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PRODUCT_ID, arMS0, MS0_SIZE);
		if (nProductId_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			*((uint32_t*)(&arMS0[4])) = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId;
			BurnToOTP(EXT_TAG_PRODUCT_ID, arMS0, 8);
		}
	}
	
}
void Ts_OTPWriteMainSector::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteMainSector", "Pass"));
}

void Ts_OTPWriteMainSector::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}