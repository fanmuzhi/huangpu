#include "Ts_WakeOnFinger.h"

Ts_WakeOnFinger::Ts_WakeOnFinger(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WakeOnFinger::~Ts_WakeOnFinger()
{
}

void Ts_WakeOnFinger::SetUp()
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

	//parse args
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_100 = 7;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMinTriggerThreshold = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes = 2000;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nStdDevThreshold = (float)0.9;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nVCC = (float)3.7;
	
	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 18)
	{
		for (size_t t = 1; t <= 18 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nVCC = stof(listOfArgValue[11]);

	Syn_PatchInfo WofPatchInfo, WofCmd1PathInfo, WofCmd2PathInfo;
	if (!_pSyn_Dut->FindPatch("WofPatch", WofPatchInfo) || NULL == WofPatchInfo._pArrayBuf
		|| !_pSyn_Dut->FindPatch("WofCmd1", WofCmd1PathInfo) || NULL == WofCmd1PathInfo._pArrayBuf
		|| !_pSyn_Dut->FindPatch("WofCmd2", WofCmd2PathInfo) || NULL == WofCmd2PathInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofPatch or WofCmd1 or WofCmd2 Patch is NULL!");
		throw ex;
	}
}

void Ts_WakeOnFinger::Execute()
{
	//If the WOF Test reading has not been stored in the OTP.
	if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofBot_count == 0)
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus == 0)
			_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithoutStimulus = true;
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus = true;

		if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus == 0)  // If NO Stimulus
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount = 0;
			ExecuteWofTest();
			WofTestProcessData();
		}
		else  // With Stimulus
		{
			// run WOF test with stimulus at normal voltage
			ExecuteWofTest();
			WofTestProcessData();

			// If not PASS, run WOF test with stimulus at high voltage
			if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass == 0)
			{
				ExecuteWofTest();
				WofTestProcessData();
			}
		}
	}
	else // If WOF Test exists in OTP
	{
		// Read one time from OTP.
		if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus == 1)
		{
			GetOtpWofData();
			// To save Log file.
			_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithoutStimulus = 1;
			_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus = 1;
		}
	}
}

void Ts_WakeOnFinger::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WakeOnFinger", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WakeOnFinger", "Pass"));

}

void Ts_WakeOnFinger::CleanUp()
{
	PowerOff();
}

void Ts_WakeOnFinger::ExecuteWofTest()
{
	Syn_PatchInfo WofPatchInfo, WofCmd1PathInfo, WofCmd2PathInfo;
	_pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	_pSyn_Dut->FindPatch("WofCmd1", WofCmd1PathInfo);
	_pSyn_Dut->FindPatch("WofCmd2", WofCmd2PathInfo);

	if (NULL == WofPatchInfo._pArrayBuf || NULL == WofCmd1PathInfo._pArrayBuf || NULL == WofCmd2PathInfo._pArrayBuf)
		return;

	int			timeout, timeout2;
	uint8_t		pStatus[4] = {0};
	uint8_t		pResult[2] = {0};
	uint8_t*	pWofCmd1 = WofCmd1PathInfo._pArrayBuf;
	int			nCmd1Size = WofCmd1PathInfo._uiArraySize;
	uint8_t*	pWofCmd2 = WofCmd2PathInfo._pArrayBuf;
	int			nCmd2Size = WofCmd2PathInfo._uiArraySize;
	int			nVCC = (int)_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nVCC * 1000;

	if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount == 0)
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	else
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVCC, nVCC, true);

	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	//Write cmd1.
	_pSyn_DutCtrl->FpWrite(1, pWofCmd1[0], &pWofCmd1[1], nCmd1Size - 1);

	//Wait for cmd1 to complete.
	timeout = 1000;
	_pSyn_DutCtrl->FpWaitForCMDComplete(timeout);
	
	//timeout = 1000;
	_pSyn_DutCtrl->FpReadBuff(pResult, sizeof(pResult));

	//Write cmd2. 
	uint8_t* pTempBuf = new uint8_t[nCmd2Size];
	memcpy(pTempBuf, pWofCmd2, nCmd2Size);
	pTempBuf[12] = 100;
	pTempBuf[16] = 100;
	pTempBuf[20] = 200;
	pTempBuf[30] = 0;
	pTempBuf[34] = 1;
	pTempBuf[38] = 63;
	_pSyn_DutCtrl->FpWrite(1, pTempBuf[0], &pTempBuf[1], nCmd2Size - 1);
	delete[] pTempBuf;
	pTempBuf = NULL;

	//Wait for cmd2 to complete.
	timeout = 1000;
	_pSyn_DutCtrl->FpWaitForCMDComplete(timeout);

	//timeout = 1000;
	_pSyn_DutCtrl->FpReadBuff(pResult, sizeof(pResult));

	//Execute command to read WOF data.
	_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);

	//Wait for command complete
	timeout = 1000;
	_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
	while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
	{
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		timeout--;

		if (timeout % 10 == 0)
			_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("WOF: 8th status never complete.");
		throw(ex);
	}

	//Get response data.
	timeout2 = 1000;
	_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes);
	while (timeout2 && !((_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[0] == 0x00) && (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[1] == 0x00)))
	{
		//Try retrieving response data again.
		_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);

		//Wait for response to be ready.
		timeout = 1000;
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
		{
			_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
			timeout--;

			if (timeout % 10 == 0)
				_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);
		}
		if (timeout == 0)
		{
			Syn_Exception ex(0);
			ex.SetDescription("WOF: 9th status never complete.");
			throw(ex);
		}

		//Check error code.
		timeout = 1000;
		_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes);
		while (timeout && !((_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[0] == 0x02) || ((_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[0] == 0x00) && (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[1] == 0x00))))
		{
			_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes);
			timeout--;
		}
		if (timeout == 0)
		{
			Syn_Exception ex(0);
			ex.SetDescription("WOF: Command 1, 7th never complete.");
			throw(ex);
		}
		timeout2--;
	}
	if (timeout2 == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("WOF: Cannot get plot.");
		throw(ex);
	}

	//Clear registers.
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_WakeOnFinger::WofTestProcessData()
{
	int			i;
	int			nTgrIdx;
	uint8_t		m_arGains[2] = { 0 };
	int			nRecSize = (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes - 6) / 2;
	int			bTgrFound = 0;
	int			nDelta;
	uint8_t*	pBuf;

	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass = 1;	//Assume true.

	if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus == 0)  // If NO Stimulus
	{
		//Find the trigger index from gain of 100.
		nTgrIdx = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[i + 6] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_100 = nTgrIdx - 1;

		//Get the trigger index for the second gain.
		bTgrFound = 0;
		nTgrIdx = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[i + 6 + nRecSize] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200 = nTgrIdx - 1;

		//If invalid threshold from gain of 100.
		if ((nTgrIdx < _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMinTriggerThreshold) || (nTgrIdx >= _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nMaxTriggerThreshold))
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim = 1;
	}
	else if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount == 0)	//With stimulus at 3.3V.
	{
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain100 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200_3p7V = 1;

		//Get trg index at gain 100.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[6];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_100 = nTgrIdx - 1;

		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_100 - _pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_100;
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_100)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain100 = 0;
			nDelta = _pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200 - _pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200;
			if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200)
			{
				_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200 = 0;
				_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass = 0;
			}
		}
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount++;
	}
	else //With stimulus at 3.7V.
	{
		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}

		// To save log for 3.7v pass
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200_3p7 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200 - (nTgrIdx - 1);
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_nDelta_200_3p7)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200_3p7V = 0;
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass = 0;
		}
	}
}

void Ts_WakeOnFinger::GetOtpWofData()
{
	uint8_t     pOtpData[MS0_SIZE];
	uint32_t    extTagData = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bValid = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200_3p7V = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200_3p7 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim = 0;

	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
		Syn_Exception ex(0);
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("Wof:OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);

	//_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_BOT, pOtpData, MS0_SIZE);
	//GetMS0RecordData(TAG_CAL, EXT_TAG_WOF_BOT, pOtpData, MS0_SIZE, site)
	

	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_BOT, pOtpData, MS0_SIZE) != 0)
	{
		extTagData = *((uint32_t*)pOtpData);
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim = (uint8_t)((extTagData & 0x00FF0000) >> 16);
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim = (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim / 100) - 1;
		if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nRecIdxWithoutStim == 0)	//gain 100
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain100 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_100 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_100 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPassAtGain200 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithoutStim_200 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nTriggerIdxWithStim_200 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
	}

	// OTP data is real gain. The write_logfile() function multiply 100 after increasing 1.
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bValid = 1;
	_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass = 1;
}