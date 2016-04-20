#include "Ts_OTPWriteMainSector.h"

Ts_OTPWriteMainSector::Ts_OTPWriteMainSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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

	//Power On
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_OTPWriteMainSector::Execute()
{
	Syn_Exception ex(0);

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
	int		nLNA_count, nPGA_count, nSNR_count;
	int		nFlexId_count, nWofBot_count, nWofTop_count, nDutTempAdc_count, nPGA_OOPP_count, nScmWofTop_count, nScmWofBot_count, nProductId_count;

	//nLNA_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nLNA_count;
	//nPGA_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPR_count;
	//nSNR_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nSNR_count;
	//nFlexId_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nFlexId_count;
	//nWofBot_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofBot_count;
	//nWofTop_count = _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofTop_count;
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
		{
			BurnToOTP(EXT_TAG_LNA, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nLnaIdx], _pSyn_Dut->_RowNumber);
		}

		//If PGA values (one offset per pixel) have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 1)//kPgaCalTypeOneOffsetPerPixel
		{
			nPGA_OOPP_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, arMS0, MS0_SIZE);
			if (nPGA_OOPP_count == 0)
			{
				BurnToOTP(EXT_TAG_PGA_OOPP, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets, NUM_PGA_OOPP_OTP_ROWS * (nNumCols - 8));
				bBurnPGA_OOPP = true;
			}
		}

		//If PGA (one offset per row) values have not been stored in the OTP.
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nCalType == 0)//kPgaCalTypeOneOffsetPerRow
		{
			nPGA_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPR, arMS0, MS0_SIZE);
			if (nPGA_count == 0)
			{
				BurnToOTP(EXT_TAG_PGA_OOPR, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults).m_pPrintPatch[(_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo).m_nPgaIdx], _pSyn_Dut->_RowNumber);
			}
		}
	}
	
	//If SNR values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.m_bExecuted)
	{
		nSNR_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SNR, arMS0, MS0_SIZE);
		if (nSNR_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
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

	//If WOF(zone 0) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithStimulus)
	{
		nWofBot_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_BOT, arMS0, MS0_SIZE);
		if (nWofBot_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_BOT, arMS0, 4);
		}
	}
	//If WOF(zone 1) values have not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithStimulus)
	{
		nWofTop_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_TOP, arMS0, MS0_SIZE);
		if (nWofTop_count == 0)
		{
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_nTriggerWithoutStim;
			arMS0[1] = (uint8_t)_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_nTriggerWithStim;
			arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_nGain & 0xFF);
			arMS0[3] = 0;

			BurnToOTP(EXT_TAG_WOF_TOP, arMS0, 4);
		}
	}

	//SCM WOF(zone1 top) 
	if (_pSyn_Dut->_pSyn_DutTestInfo->_TopSCM_wofInfo.m_bExecutedWithStimulus)
	{
		nScmWofTop_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_TOP, arMS0, MS0_SIZE);
		if (nScmWofTop_count == 0)
		{	
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestResult->_TopSCM_wofResults.m_nTriggerWithoutStim;
			arMS0[1] = _pSyn_Dut->_pSyn_DutTestResult->_TopSCM_wofResults.m_nTriggerWithStim;
            arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_TopSCM_wofResults.m_nGain & 0xFF);
            arMS0[3] = 0;
			BurnToOTP(EXT_TAG_SCM_WOF_TOP, arMS0, 4);
		}
	}

	//SCM WOF(zone0 bottom)
	if (_pSyn_Dut->_pSyn_DutTestInfo->_BottomSCM_wofInfo.m_bExecutedWithStimulus)
	{
		nScmWofBot_count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, arMS0, MS0_SIZE);
		if (nScmWofBot_count == 0)
		{	
			memset(arMS0, 0, sizeof(arMS0));
			arMS0[0] = _pSyn_Dut->_pSyn_DutTestResult->_BottomSCM_wofResults.m_nTriggerWithoutStim;
			arMS0[1] = _pSyn_Dut->_pSyn_DutTestResult->_BottomSCM_wofResults.m_nTriggerWithStim;
            arMS0[2] = (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_BottomSCM_wofResults.m_nGain & 0xFF);
            arMS0[3] = 0;
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

	//FW patch
	Syn_PatchInfo fmPatch;
	if (_pSyn_Dut->FindPatch("FrimwareUpdate", fmPatch))
	{
		if (!RegCheckBitSet())
		{
			return;
		}
		_pSyn_DutCtrl->FpOtpRomWrite(MAIN_SEC, 1, fmPatch._pArrayBuf, fmPatch._uiArraySize);

	}

	//MT pass bit
}

void Ts_OTPWriteMainSector::ProcessData()
{
	Syn_Exception ex(0);
	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);

	int count = 0;
	int result = 1;		//0 for match
	bool bPass = true;
	uint8_t pDst[MS0_SIZE];

	//check projID match
	count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PRODUCT_ID, pDst, MS0_SIZE);
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
	count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pDst, MS0_SIZE);
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
	count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pDst, MS0_SIZE);
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
	PowerOff();
}

void Ts_OTPWriteMainSector::BurnToOTP(long nRecType, uint8_t* pSrc, int numBytes)
{
	int		timeout(2000), nSize(0);
	uint8_t arOutBuf[MS0_SIZE + MS1_SIZE] = { 0 };

	arOutBuf[4] = 0x11;
	arOutBuf[12] = (uint8_t)(nRecType & 0xFF);
	arOutBuf[15] = (uint8_t)((nRecType >> 24) & 0xFF);
	arOutBuf[18] = 0x0E;

	if ((nRecType == EXT_TAG_PGA_OOPR) || (nRecType == EXT_TAG_PGA_OOPP) ||(nRecType == EXT_TAG_LNA) || (nRecType == EXT_TAG_WOF_BOT) || (nRecType == EXT_TAG_SCM_WOF_BOT))
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

	_pSyn_DutCtrl->FpOtpRomTagWrite(arOutBuf, nSize);

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
