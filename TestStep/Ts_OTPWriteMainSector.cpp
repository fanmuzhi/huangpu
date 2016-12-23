#include "Ts_OTPWriteMainSector.h"

#include <regex>

Ts_OTPWriteMainSector::Ts_OTPWriteMainSector(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
, bBurnPGA_OOPP(false)
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
}

void Ts_OTPWriteMainSector::Execute()
{
	Syn_Exception ex(0);
	uint32_t rc(0);

	//load OTPReadWritePatch
	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);

	if (0 != _pSyn_Dut->_pSyn_DutTestResult->_binCodes.size())
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteMainSector", "Fail"));
		return;
	}

	uint8_t	arMS0[MS0_SIZE] = { 0 };
	bool	bSuccess = true;
	int		nNumCols = _pSyn_Dut->_ColumnNumber;
	int		nLNA_count, nPGA_count, nSNR_count, nLNA_PGA_GAINS_count;
	int		nFlexId_count, nWofBot_count, nWofTop_count, nDutTempAdc_count, nPGA_OOPP_count, nScmWofTop_count, nScmWofBot_count, nProductId_count, nPartNumberId_count;
	MtAndConfigPnInfo    partNumbers;

	//Cablirate
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		//If LNA values have not been stored in the OTP.
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, arMS0, MS0_SIZE, nLNA_count);
		if (nLNA_count == 0)
		{
			BurnToOTP(EXT_TAG_LNA, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nLnaIdx], _pSyn_Dut->_RowNumber);
			BurnToOTP(EXT_TAG_LNA, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nLnaIdx], _pSyn_Dut->_RowNumber);
		}

		//If PGA values (one offset per pixel) have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 1)//kPgaCalTypeOneOffsetPerPixel
		{
			rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, arMS0, MS0_SIZE, nPGA_OOPP_count);
			if (nPGA_OOPP_count == 0)
			{
				BurnToOTP(EXT_TAG_PGA_OOPP, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets, NUM_PGA_OOPP_OTP_ROWS * (nNumCols - 8));
				BurnToOTP(EXT_TAG_PGA_OOPP, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets, NUM_PGA_OOPP_OTP_ROWS * (nNumCols - 8));
				bBurnPGA_OOPP = true;
			}
		}

		//If PGA (one offset per row) values have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 0)//kPgaCalTypeOneOffsetPerRow
		{
			rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPR, arMS0, MS0_SIZE, nPGA_count);
			if (nPGA_count == 0)
			{
				BurnToOTP(EXT_TAG_PGA_OOPR, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nPgaIdx], _pSyn_Dut->_RowNumber);
				BurnToOTP(EXT_TAG_PGA_OOPR, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nPgaIdx], _pSyn_Dut->_RowNumber);
			}
		}
	}
	
	//If SNR values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.m_bExecuted)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SNR, arMS0, MS0_SIZE, nSNR_count);
		if (nSNR_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_SNR);
			arMS0[1] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_Signal);
			arMS0[2] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestResult->_snrResults).OTPVal_Noise);
			arMS0[3] = 0;
			BurnToOTP(EXT_TAG_SNR, arMS0, 4);
			BurnToOTP(EXT_TAG_SNR, arMS0, 4);
		}
	}
	
	//flexID
	//If flex ID values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_FlexId, arMS0, MS0_SIZE, nFlexId_count);
		if (nFlexId_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId;
			arMS0[1] = (uint8_t)((_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId) >> 8);
			BurnToOTP(EXT_TAG_FlexId, arMS0, 8);
			BurnToOTP(EXT_TAG_FlexId, arMS0, 8);
		}
	}

	//If WOF(zone 0) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FD_ZONE0, arMS0, MS0_SIZE, nWofBot_count);
		if (nWofBot_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_FD_ZONE0, arMS0, 4);
			BurnToOTP(EXT_TAG_WOF_FD_ZONE0, arMS0, 4);
		}
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0FUWofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FU_ZONE0, arMS0, MS0_SIZE, nWofBot_count);
		if (nWofBot_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0FUWofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0FUWofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z0FUWofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_FU_ZONE0, arMS0, 4);
			BurnToOTP(EXT_TAG_WOF_FU_ZONE0, arMS0, 4);
		}
	}
	//If WOF(zone 1) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FD_ZONE1, arMS0, MS0_SIZE, nWofTop_count);
		if (nWofTop_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_FD_ZONE1, arMS0, 4);
			BurnToOTP(EXT_TAG_WOF_FD_ZONE1, arMS0, 4);
		}
	}
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z1FUWofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FU_ZONE1, arMS0, MS0_SIZE, nWofTop_count);
		if (nWofTop_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1FUWofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1FUWofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z1FUWofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_FU_ZONE1, arMS0, 4);
			BurnToOTP(EXT_TAG_WOF_FU_ZONE1, arMS0, 4);
		}
	}

	//SCM WOF(zone1 top) 
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z1SCM_wofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_ZONE1, arMS0, MS0_SIZE, nScmWofTop_count);
		if (nScmWofTop_count == 0)
		{	
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestResult->_z1SCM_wofResults.m_nTriggerWithoutStim;
			arMS0[1] = _pSyn_Dut->_pSyn_DutTestResult->_z1SCM_wofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z1SCM_wofResults.m_nGain & 0xFF);
            arMS0[3] = 0;
			BurnToOTP(EXT_TAG_SCM_WOF_ZONE1, arMS0, 4);
			BurnToOTP(EXT_TAG_SCM_WOF_ZONE1, arMS0, 4);
		}
	}

	//SCM WOF(zone0 bottom)
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bExecutedWithStimulus)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_ZONE0, arMS0, MS0_SIZE, nScmWofBot_count);
		if (nScmWofBot_count == 0)
		{	
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_nTriggerWithoutStim;
			arMS0[1] = _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_nGain & 0xFF);
            arMS0[3] = 0;
			BurnToOTP(EXT_TAG_SCM_WOF_ZONE0, arMS0, 4);
			BurnToOTP(EXT_TAG_SCM_WOF_ZONE0, arMS0, 4);
		}
	}

	//If temperature ADC values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_DutTempAdc, arMS0, MS0_SIZE, nDutTempAdc_count);
		if (nDutTempAdc_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc;
			arMS0[1] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc >> 8);
			BurnToOTP(EXT_TAG_DutTempAdc, arMS0, 4);
			BurnToOTP(EXT_TAG_DutTempAdc, arMS0, 4);
		}
	}

	//projID
	if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId != 0)
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PRODUCT_ID, arMS0, MS0_SIZE, nProductId_count);
		if (nProductId_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			*((uint32_t*)(&arMS0[4])) = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId;
			BurnToOTP(EXT_TAG_PRODUCT_ID, arMS0, 8);
			BurnToOTP(EXT_TAG_PRODUCT_ID, arMS0, 8);
		}
	}

	//LNA_PGA_GAINS
	rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA_PGA_GAINS, arMS0, MS0_SIZE, nLNA_PGA_GAINS_count);
	if (nLNA_PGA_GAINS_count == 0&&0!=_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bUseConfigGains)
	{
		/*uint8_t LnaGainValue(0), PgaGainValue(0);
		bool LnaResult(false), PgaResult(false);
		LnaResult = FindGainInPrintFile(0x08, 0x21, LnaGainValue);
		PgaResult = FindGainInPrintFile(0x48, 0x21, PgaGainValue);
		uint16_t PgaRatio(0);
		PgaRatio = (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaOffsetRatio)*100;
		if (LnaResult&&PgaRatio)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = LnaGainValue;
			arMS0[1] = PgaGainValue;
			arMS0[2] = (uint8_t)(PgaRatio & 0xFF);
			arMS0[3] = (uint8_t)(PgaRatio >> 8);
			arMS0[4] = LnaGainValue;
			arMS0[5] = PgaGainValue;
			arMS0[6] = (uint8_t)(PgaRatio & 0xFF);
			arMS0[7] = (uint8_t)(PgaRatio >> 8);

			BurnToOTP(EXT_TAG_LNA_PGA_GAINS, arMS0, 8);
			BurnToOTP(EXT_TAG_LNA_PGA_GAINS, arMS0, 8);
		}*/

		memset(arMS0, 0, sizeof(arMS0));
		arMS0[0] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nImageLnaGain;
		arMS0[1] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nImagePgaGain;
		arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nImagePgaRatio & 0xFF);
		arMS0[3] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nImagePgaRatio >> 8);
		arMS0[4] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nNavLnaGain;
		arMS0[5] = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nNavPgaGain;
		arMS0[6] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nNavPgaRatio & 0xFF);
		arMS0[7] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nNavPgaRatio >> 8);

		BurnToOTP(EXT_TAG_LNA_PGA_GAINS, arMS0, 8);
		BurnToOTP(EXT_TAG_LNA_PGA_GAINS, arMS0, 8);
	}

	//PART_NUMBERS
	if (GetMtAndConfigPartNumbers(&partNumbers))
	{
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PART_NUMBERS, arMS0, MS0_SIZE, nPartNumberId_count);
		if (nPartNumberId_count == 0)
		{
			BurnToOTP(EXT_TAG_PART_NUMBERS, (uint8_t*)&partNumbers, sizeof(partNumbers));
			BurnToOTP(EXT_TAG_PART_NUMBERS, (uint8_t*)&partNumbers, sizeof(partNumbers));
		}
	}

	//FW patch
	Syn_PatchInfo fmPatch;
	if (_pSyn_Dut->FindPatch("FirmwareUpdate", fmPatch))
	{
		if (NULL != fmPatch._pArrayBuf)
		{
			//if (!RegCheckBitSet())
			//{
			//	return;
			//}
			try
			{
				_pSyn_DutCtrl->FpOtpRomWrite(MAIN_SEC, 1, fmPatch._pArrayBuf, fmPatch._uiArraySize);
			}
			catch (exception ex)
			{
				try
				{
					//this is a work around.
					_pSyn_DutCtrl->FpOtpRomWrite(MAIN_SEC, 1, fmPatch._pArrayBuf, fmPatch._uiArraySize);
				}
				catch (...)
				{
				}
			}
		}
	}

	//MT pass bit
}

void Ts_OTPWriteMainSector::ProcessData()
{
	Syn_Exception ex(0);
	uint32_t rc(0);

	int count = 0;
	int result = 1;		//0 for match
	bool bPass = true;
	uint8_t pDst[MS0_SIZE];

	//check projID match
	rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PRODUCT_ID, pDst, MS0_SIZE, count);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("OtpRomTagRead EXT_TAG_PRODUCT_ID is failed!");
		throw ex;
	}
	if (count > 0)
	{
		//result = memcmp(&pDst[4], &_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId, 4);
		uint32_t projID = *((uint32_t*)&pDst[4]);
		if (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId != projID)
		{
			bPass = false;
		}
	}
	else
	{
		bPass = false;
	}

	//check LNA match
	rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pDst, MS0_SIZE, count);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("OtpRomTagRead EXT_TAG_LNA is failed!");
		throw ex;
	}
	if (count > 0)
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
		{
			result = memcmp(&pDst[4], &_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch[_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx], _pSyn_Dut->_RowNumber);
			if (result != 0)
			{
				bPass = false;
			}
		}
	}
	else
	{
		bPass = false;
	}

	//check PGA OOPP match
	rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pDst, MS0_SIZE, count);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("OtpRomTagRead EXT_TAG_PGA_OOPP is failed!");
		throw ex;
	}
	if (count > 0)
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
		{
			if (bBurnPGA_OOPP)
			{
				//compare result in real time calibration
				result = memcmp(&pDst[4], &_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets, (_pSyn_Dut->_ColumnNumber - 8) * NUM_PGA_OOPP_OTP_ROWS);
			}
			else
			{
				//compare result in OTP
				result = memcmp(&pDst[4], &_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPGAOtpArray, (_pSyn_Dut->_ColumnNumber - 8)* NUM_PGA_OOPP_OTP_ROWS);
			}
			if (result != 0)
			{
				bPass = false;
			}
		}
	}
	else
	{
		bPass = false;
	}

	if (0 != _pSyn_Dut->_pSyn_DutTestResult->_binCodes.size() || !bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteMainSector", "Fail"));
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sMainSectorFail);
		return;
	}

	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteMainSector", "Pass"));
}

void Ts_OTPWriteMainSector::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}

void Ts_OTPWriteMainSector::BurnToOTP(long nRecType, uint8_t* pSrc, int numBytes)
{
	int		timeout(2000), nSize(0);
	uint8_t arOutBuf[MS0_SIZE + MS1_SIZE] = { 0 };

	arOutBuf[4] = 0x11;
	arOutBuf[12] = (uint8_t)(nRecType & 0xFF);
	arOutBuf[15] = (uint8_t)((nRecType >> 24) & 0xFF);
	arOutBuf[18] = 0x0E;

	if ((nRecType == EXT_TAG_PGA_OOPR) || (nRecType == EXT_TAG_PGA_OOPP) ||(nRecType == EXT_TAG_LNA) || 
		(nRecType == EXT_TAG_WOF_FD_ZONE0) || (nRecType == EXT_TAG_WOF_FD_ZONE1) ||
		(nRecType == EXT_TAG_WOF_FU_ZONE0) || (nRecType == EXT_TAG_WOF_FU_ZONE1) ||
		(nRecType == EXT_TAG_SCM_WOF_ZONE0) || (nRecType == EXT_TAG_SCM_WOF_ZONE1)|| (nRecType == EXT_TAG_LNA_PGA_GAINS))
	{
		//PGA and LNA records have an extra 4 bytes (0x00000007).
		arOutBuf[24] = 0x07;
		arOutBuf[16] = numBytes + 4;
		arOutBuf[17] = (numBytes + 4) >> 8;
		memcpy(&arOutBuf[28], pSrc, numBytes);
		nSize = numBytes + 28;
	}
	else //All other records.
	{
		arOutBuf[16] = numBytes;
		arOutBuf[17] = numBytes >> 8;
		memcpy(&arOutBuf[24], pSrc, numBytes);
		nSize = numBytes + 24;
	}

	uint8_t pStatus[4] = { 0 };
	uint8_t arExtendedTag[2] = { 0 };
	uint8_t arNumBytes[2] = { 0 };

	uint32_t rc = _pSyn_DutCtrl->FpOtpRomTagWrite(arOutBuf, nSize);
	if (0 != rc)
	{
		Syn_Exception ex(0);
		ex.SetDescription("BurnToOTP::FpOtpRomTagWrite is failed.");
		throw(ex);
	}
}

bool Ts_OTPWriteMainSector::RegCheckBitSet()
{
	uint8_t	bootSector0[BS0_SIZE];
	uint8_t regCheckBit;
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, bootSector0, BS0_SIZE);

	//Check RegCheck bit in words 12 and 14
	if (bootSector0[49] != bootSector0[57])
	{
		Syn_Exception ex(0);
		ex.SetDescription("Invalidation did not complete.");
		throw(ex);
	}
	regCheckBit = (bootSector0[49] >> 4);	
	return ((regCheckBit & 0x4)>>2) != 0;
}

bool Ts_OTPWriteMainSector::GetMtAndConfigPartNumbers(MtAndConfigPnInfo* pInfo)
{
	string sConfigFileName = _pSyn_Dut->_sConfigFileName;

	memset(pInfo->mt_partnum, NULL, MAX_PART_NUMBER_LENGTH);
	memset(pInfo->mt_config, NULL, MAX_PART_NUMBER_LENGTH);

	uint8_t pPartNumber[MAX_PART_NUMBER_LENGTH] = { 0x05, 0x55, 0x00, 0x06, 0x19, 0x01, 0x00, 0x0A };
	uint8_t pConfigFile[MAX_PART_NUMBER_LENGTH] = { 0x05, 0x80, 0x00, 0x60, 0x33, 0x01, 0x00, 0x01 };
	//uint8_t pConfigFile[MAX_PART_NUMBER_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	if (!GetConfigFileArray(sConfigFileName, pConfigFile))
	{
		return false;
	}

	memcpy(pInfo->mt_partnum, pPartNumber, MAX_PART_NUMBER_LENGTH);//MT Part Number              
	memcpy(pInfo->mt_config, pConfigFile, MAX_PART_NUMBER_LENGTH);

	//Get time info
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);
	pInfo->mt_month = tm.tm_mon;
	pInfo->mt_day = tm.tm_mday - 1;
	pInfo->mt_year = tm.tm_year + 1900;

	return true;
}

bool Ts_OTPWriteMainSector::GetConfigFileArray(string sConfigFileName, uint8_t *pConfigFilerArray, int arrSize)
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

void Ts_OTPWriteMainSector::TranslatePartNum(const string& sPN, uint8_t* pDst)
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

bool Ts_OTPWriteMainSector::FindGainInPrintFile(uint8_t iFirstValue, uint8_t iSecondValue, uint8_t &oGainValue)
{
	Syn_PatchInfo PrintFileInfo;
	bool rc = _pSyn_Dut->FindPatch("PrintFile", PrintFileInfo);
	if (!rc || NULL == PrintFileInfo._pArrayBuf)
		return false;

	rc = false;
	for (int i = 0; i < PrintFileInfo._uiArraySize - 2; i++)
	{
		if (iFirstValue == PrintFileInfo._pArrayBuf[i] && iSecondValue == PrintFileInfo._pArrayBuf[i + 1])
		{
			oGainValue = PrintFileInfo._pArrayBuf[i + 2];
			rc = true;
			break;
		}
	}


	return rc;
}
