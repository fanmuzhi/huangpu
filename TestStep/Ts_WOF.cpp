#include "Ts_WOF.h"

Ts_WOF::Ts_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WOF::~Ts_WOF()
{
}

void Ts_WOF::SetUp()
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
	//_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithoutStimulus = false;
	//_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithStimulus = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_100 = 7;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nMinTriggerThreshold = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nNumResponseBytes = 2000;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nStdDevThreshold = (float)0.9;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nVCC = (float)3.7;
	
	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 18)
	{
		for (size_t t = 1; t <= 18 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_nVCC = stof(listOfArgValue[11]);

	_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo = _pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo;
}

void Ts_WOF::Execute()
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

	/*if (_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus)
	{
		//3.3V
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount = 0;
		ExecuteWofTest();
		WofTestProcessData();

		if (_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_bPass == 0)
		{
			//3.7V
			ExecuteWofTest();
			WofTestProcessData();
		}
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus = true;

	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_wofResults.m_nWithStimCount = 0;
		//3.3V
		ExecuteWofTest();
		WofTestProcessData();
		_pSyn_Dut->_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithoutStimulus = true;
	}*/

	// WofPatch is not empty, and either/both Zone0 or Zone1 are used.
	Syn_PatchInfo WofPatchInfo;
	bool rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	if (!rc||NULL == WofPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WOF Patch is NULL!");
		throw ex;
		return;
	}

	double dCurrentElapsedTime(0);
	ComputeRunningTime(dCurrentElapsedTime);

	Syn_PatchInfo WofCmd1PathInfo, WofCmd2PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd1", WofCmd1PathInfo);
	rc = _pSyn_Dut->FindPatch("WofCmd2", WofCmd2PathInfo);
	if (NULL != WofCmd1PathInfo._pArrayBuf && NULL != WofCmd2PathInfo._pArrayBuf)
	{
		//If we have not yet aquired the WOF data without stimulus.
		if (_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bWithStimulus == 0)
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithoutStimulus = 1;
			GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0WofResults);

			_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_elapsedtime += dCurrentElapsedTime;
		}
		else	//Execute with stimulus.
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithStimulus = 1;
			GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0WofResults);
			SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0WofResults);

			_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_elapsedtime += dCurrentElapsedTime;

			if (_pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_bPass == 0)
			{
				_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
			}
		}
	}

	Syn_PatchInfo WofCmd3PathInfo, WofCmd4PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd3", WofCmd3PathInfo);
	rc = _pSyn_Dut->FindPatch("WofCmd4", WofCmd4PathInfo);
	if (NULL != WofCmd3PathInfo._pArrayBuf && NULL != WofCmd4PathInfo._pArrayBuf)
	{
		//If we have not yet aquired the WOF data without stimulus.
		if (_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bWithStimulus == 0)
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithoutStimulus = 1;
			GetZ1WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1WofResults);

			_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_elapsedtime += dCurrentElapsedTime;
		}
		else	//Execute with stimulus.
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithStimulus = 1;
			GetZ1WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1WofResults);
			SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1WofResults);

			_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_elapsedtime += dCurrentElapsedTime;

			if (_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_bPass == 0)
			{
				_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
			}
		}
	}
}

void Ts_WOF::ProcessData()
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

	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bWithStimulus)  // WithStimulus
	{
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_bPass == 0)
			|| (_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_bPass == 0))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOFWithStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOFWithStimulus", "Pass"));
		}
	}
	else //without Stimulus
	{
		if ((_pSyn_Dut->_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithoutStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z0WofResults.m_bPass == 0)
			|| (_pSyn_Dut->_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithoutStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_bPass == 0))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOFWithoutStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOFWithoutStimulus", "Pass"));
		}
	}
}

void Ts_WOF::CleanUp()
{
	PowerOff();
}

void Ts_WOF::GetZ0WofData(WofTestInfo &wofInfo,WofTestResults &wofResults)
{
	bool rc(false);

	int nVcc = (int)(wofInfo.m_nVCC * 1000);
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);

	//Load the patch, execute and wait for cmd1 to complete.
	Syn_PatchInfo WofPatchInfo;
	rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);
	
	Syn_PatchInfo WofCmd1PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd1", WofCmd1PathInfo);
	_pSyn_DutCtrl->FpWrite(1, WofCmd1PathInfo._pArrayBuf[0], &WofCmd1PathInfo._pArrayBuf[1], WofCmd1PathInfo._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	Syn_PatchInfo WofCmd2PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd2", WofCmd2PathInfo);

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	ModifySweepCmdData(WofCmd2PathInfo._pArrayBuf);
	wofResults.m_nThreshStart = WofCmd2PathInfo._pArrayBuf[0x1E];
	wofResults.m_nThreshInc = WofCmd2PathInfo._pArrayBuf[0x22];
	wofResults.m_nThreshStop = WofCmd2PathInfo._pArrayBuf[0x26];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd2PathInfo._pArrayBuf[0x0C];
	wofResults.m_nGainInc = WofCmd2PathInfo._pArrayBuf[0x10];
	wofResults.m_nGainStop = WofCmd2PathInfo._pArrayBuf[0x14];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + 6;

	//Execute and wait for sweep command to complete.
	_pSyn_DutCtrl->FpWrite(1, WofCmd2PathInfo._pArrayBuf[0], &WofCmd2PathInfo._pArrayBuf[1], WofCmd2PathInfo._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	//Send execute command and wait for response.
	uint8_t* pDst = wofResults.m_arDataWithoutStim;
	int timeout = 100;
	do
	{
		_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);
		Sleep(20);
		_pSyn_DutCtrl->FpRead(1, 0xFF, pDst, wofResults.m_nResponseSize);
		timeout--;
	} while (timeout && (!((pDst[0] == 0x00) && (pDst[1] == 0x00))));

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("WOF: Status never complete.");
		throw(ex);
		return;
	}

	//Clear registers.
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_WOF::GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults)
{
	bool rc(false);

	int nVcc = (int)(wofInfo.m_nVCC * 1000);
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);

	//Load the patch, execute and wait for cmd3 to complete.
	Syn_PatchInfo WofPatchInfo;
	rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	Syn_PatchInfo WofCmd3PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd3", WofCmd3PathInfo);
	_pSyn_DutCtrl->FpWrite(1, WofCmd3PathInfo._pArrayBuf[0], &WofCmd3PathInfo._pArrayBuf[1], WofCmd3PathInfo._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	Syn_PatchInfo WofCmd4PathInfo;
	rc = _pSyn_Dut->FindPatch("WofCmd4", WofCmd4PathInfo);

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	ModifySweepCmdData(WofCmd4PathInfo._pArrayBuf);
	wofResults.m_nThreshStart = WofCmd4PathInfo._pArrayBuf[0x1F];
	wofResults.m_nThreshInc = WofCmd4PathInfo._pArrayBuf[0x23];
	wofResults.m_nThreshStop = WofCmd4PathInfo._pArrayBuf[0x27];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd4PathInfo._pArrayBuf[0x0A];
	wofResults.m_nGainInc = WofCmd4PathInfo._pArrayBuf[0x0E];
	wofResults.m_nGainStop = WofCmd4PathInfo._pArrayBuf[0x12];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + 6;

	//Execute and wait for sweep command to complete.
	_pSyn_DutCtrl->FpWrite(1, WofCmd4PathInfo._pArrayBuf[0], &WofCmd4PathInfo._pArrayBuf[1], WofCmd4PathInfo._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	//Send execute command and wait for response.
	uint8_t* pDst = wofResults.m_arDataWithoutStim;
	int timeout = 100;
	do
	{
		_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);
		Sleep(20);
		_pSyn_DutCtrl->FpRead(1, 0xFF, pDst, wofResults.m_nResponseSize);
		timeout--;
	} while (timeout && (!((pDst[0] == 0x00) && (pDst[1] == 0x00))));

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF: Status never complete.");
		throw(ex);
		return;
	}

	//Clear registers.
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}













void Ts_WOF::SYN_WofTestExecute(WofTestInfo &Info, WofTestResults &Results)
{
	int	nTgrIdx, nGainIdx;

	Results.m_bPass = 0;				//Assume fail.
	Results.m_nTriggerWithoutStim = 0;
	Results.m_nTriggerWithStim = 0;
	Results.m_nGain = 0;

	//Find the gain that produced the best SCM WOF trigger data (with stimulus).
	Results.m_nGain = Results.m_nGainStart;
	for (nGainIdx = 0; (nGainIdx < Results.m_nNumGains) && (Results.m_bPass == 0); nGainIdx++)
	{
		//Calc the gain.
		Results.m_nGain = Results.m_nGainStart + (Results.m_nGainInc * nGainIdx);

		//If we got a good trigger at this gain (without stimulus).
		nTgrIdx = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithoutStim[6 + (nGainIdx * Results.m_nNumThresholds)]);
		Results.m_nTriggerWithoutStim = nTgrIdx;
		if ((nTgrIdx >= Info.m_nMinTriggerThreshold) && (nTgrIdx < Info.m_nMaxTriggerThreshold))
		{
			//If we got a good trigger at this gain (with stimulus).
			nTgrIdx = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithStim[6 + (nGainIdx * Results.m_nNumThresholds)]);
			Results.m_nTriggerWithStim = nTgrIdx;
			if ((nTgrIdx >= Info.m_nMinTriggerThreshold) && (nTgrIdx < Info.m_nMaxTriggerThreshold))
				Results.m_bPass = 1;
		}

		if (Results.m_bPass != 0)
		{
			int nDelta = Results.m_nTriggerWithoutStim - Results.m_nTriggerWithStim;
			if (nDelta < Info.m_nDelta_100)
				Results.m_bPass = 0;
		}
	}
}

int Ts_WOF::CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf)
{
	int i;

	//Find the first occurence of 1.
	int nTgrIdx = 3000;
	for (i = 0; i<nNumThresholds; i++)
	{
		if (((pTriggerBuf[i] & 0x01) == 1) && (nTgrIdx == 3000))
			nTgrIdx = i;
	}
	return nTgrIdx;
}