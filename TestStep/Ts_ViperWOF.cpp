#include "Ts_ViperWOF.h"


Ts_ViperWOF::Ts_ViperWOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_ViperWOF::~Ts_ViperWOF()
{
}

void Ts_ViperWOF::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_100 = 7;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMinTriggerThreshold = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumResponseBytes = 2000;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nStdDevThreshold = (float)0.9;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nVCC = (float)3.6;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 12)
	{
		for (size_t t = 1; t <= 12 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	}
	if (0 != listOfArgValue[1].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	}
	if (0 != listOfArgValue[2].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	}
	if (0 != listOfArgValue[3].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	}
	if (0 != listOfArgValue[4].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	}
	if (0 != listOfArgValue[5].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	}
	if (0 != listOfArgValue[6].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	}
	if (0 != listOfArgValue[7].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	}
	if (0 != listOfArgValue[8].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
	}
	if (0 != listOfArgValue[9].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	}
	if (0 != listOfArgValue[10].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	}
	if (0 != listOfArgValue[11].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nVCC = stof(listOfArgValue[11]);
	}
	
}

void Ts_ViperWOF::Execute()
{
	bool rc(false);

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

	// WofPatch is not empty, and either/both Zone0 or Zone1 are used.
	Syn_PatchInfo WofPatchInfo;
	rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	if (!rc || NULL == WofPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WOF Patch is NULL!");
		throw ex;
		return;
	}

	double dCurrentElapsedTime(0);

	//zone0 bottom
	if (0 == _pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bWithStimulus)
	{
		//zone0 without stimulus
		if (GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, false))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bExecutedWithoutStimulus = 1;
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}
	else	//Execute with stimulus.
	{
		//zone0 with stimulus
		if (GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, false))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bExecutedWithStimulus = 1;
			SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, false);
			if (_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_bPass == 0)
			{
				//zone0 finger down with 3.6V
				GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, true);
				SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, true);
			}
		}
		if (_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}
}


void Ts_ViperWOF::ProcessData()
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

	Syn_DutTestInfo *pDutTestInfo = _pSyn_Dut->_pSyn_DutTestInfo;
	Syn_DutTestResult *pDutTestResult = _pSyn_Dut->_pSyn_DutTestResult;

	if (pDutTestInfo->_z0FDWofInfo.m_bWithStimulus)
	{
		if (pDutTestInfo->_z0FDWofInfo.m_bExecutedWithStimulus && pDutTestResult->_z0FDWofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithStimulus", "Pass"));
		}
	}
	else
	{
		if (pDutTestInfo->_z0FDWofInfo.m_bExecutedWithoutStimulus && pDutTestResult->_z0FDWofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithoutStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithoutStimulus", "Pass"));
		}
	}
}

void Ts_ViperWOF::CleanUp()
{
	PowerOff();
}

bool Ts_ViperWOF::GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, bool UseConfigVoltage)
{
	bool rc(false);

	bool		bWithStim = wofInfo.m_bWithStimulus;
	uint8_t*	pResponseBuf = bWithStim ? wofResults.m_arDataWithStim : wofResults.m_arDataWithoutStim;

	Syn_PatchInfo WofCmd1Patch, WofCmd2Patch;
	rc = _pSyn_Dut->FindPatch("WofFdCmd1", WofCmd1Patch);
	rc = _pSyn_Dut->FindPatch("WofFdCmd2", WofCmd2Patch);

	int nVcc = (int)(wofInfo.m_nVCC * 1000);
	PowerOff();

	if (UseConfigVoltage)
	{
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);
	}
	else
	{
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	}

	Syn_PatchInfo WofPatchInfo;
	rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	if (NULL == WofPatchInfo._pArrayBuf || NULL == WofCmd1Patch._pArrayBuf || NULL == WofCmd2Patch._pArrayBuf)
	{
		return false;
	}

	//Load the patch, execute and wait for cmd1 to complete.
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	_pSyn_DutCtrl->FpWrite(1, WofCmd1Patch._pArrayBuf[0], &WofCmd1Patch._pArrayBuf[1], WofCmd1Patch._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	ModifySweepWofCmdData(WofCmd2Patch._pArrayBuf);
	wofResults.m_nThreshStart = WofCmd2Patch._pArrayBuf[0x1E];
	wofResults.m_nThreshInc = WofCmd2Patch._pArrayBuf[0x22];
	wofResults.m_nThreshStop = WofCmd2Patch._pArrayBuf[0x26];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd2Patch._pArrayBuf[0x0C];
	wofResults.m_nGainInc = WofCmd2Patch._pArrayBuf[0x10];
	wofResults.m_nGainStop = WofCmd2Patch._pArrayBuf[0x14];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + 6;

	//Execute and wait for sweep command to complete.
	_pSyn_DutCtrl->FpWrite(1, WofCmd2Patch._pArrayBuf[0], &WofCmd2Patch._pArrayBuf[1], WofCmd2Patch._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//Send execute command and wait for response.
	int timeout = 100;
	do
	{
		_pSyn_DutCtrl->FpWrite(1, 0xF9, (uint8_t*)0, 0);
		::Sleep(20);
		_pSyn_DutCtrl->FpRead(1, 0xFF, pResponseBuf, wofResults.m_nResponseSize);
		timeout--;
	} while (timeout && (!((pResponseBuf[0] == 0x00) && (pResponseBuf[1] == 0x00))));

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("WOF: Status never complete.");
		throw(ex);
		return false;
	}

	//Clear registers.
	PowerOff();
	return true;
}

void Ts_ViperWOF::SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results, bool useConfigVolts)
{
	Results.m_bPass = 0;				//Assume fail.
	Results.m_nTriggerWithoutStim = 0;
	Results.m_nTriggerWithStim = 0;
	Results.m_nGain = 0;
	//int TempGain200(0), TempTriggerWithoutStimGain200(0), TempTriggerWithStimGain200(0);

	if (Results.m_nNumGains < 2)
	{
		Syn_Exception ex(0);
		ex.SetDescription("Viper WOF: Gain counts is less than 2.");
		throw(ex); return;
	}

	if (!useConfigVolts)
	{
		//Gain100 and Gain200
		for (int nGainIdx = 0; (nGainIdx < Results.m_nNumGains) && (Results.m_bPass == 0); nGainIdx++)
		{
			//Calc the gain.
			int nTgrIdex_withoutFinger(0), nTgrIdex_withFinger(0);
			bool rc1 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithoutStim[6 + (nGainIdx * Results.m_nNumThresholds)], nTgrIdex_withoutFinger);
			bool rc2 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithStim[6 + (nGainIdx * Results.m_nNumThresholds)], nTgrIdex_withFinger);
			Results.m_nGain = Results.m_nGainStart + (Results.m_nGainInc * nGainIdx);
			Results.m_nTriggerWithoutStim = nTgrIdex_withoutFinger;
			Results.m_nTriggerWithStim = nTgrIdex_withFinger;
			LOG(DEBUG) << "WOF Gain:" << dec << Results.m_nGain << ",NoFinger:" << dec << nTgrIdex_withoutFinger << ",WithFinger:" << dec << nTgrIdex_withFinger;
			//if (!rc1 || !rc2)
			//	continue;
			if (nTgrIdex_withoutFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withoutFinger < Info.m_nMinTriggerThreshold)
				continue;
			if (nTgrIdex_withFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withFinger < Info.m_nMinTriggerThreshold)
				continue;

			if (0 == nGainIdx)
			{
				//Gain100
				int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
				if (nDelta > Info.m_nDelta_100)
				{
					Results.m_bPass = 1;
					return;
				}
			}
			else
			{
				//Gain200
				int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
				if (nDelta > Info.m_nDelta_200)
				{
					Results.m_bPass = 1;
					return;
				}
			}
		}
	}
	else
	{
		//3.6V
		//GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, true);
		int nTgrIdex_withoutFinger(0), nTgrIdex_withFinger(0);
		bool rc1 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithoutStim[6 + (1 * Results.m_nNumThresholds)], nTgrIdex_withoutFinger);
		bool rc2 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithStim[6 + (1 * Results.m_nNumThresholds)], nTgrIdex_withFinger);
		LOG(DEBUG) << "WOF Gain:"<< dec << Results.m_nGain << "(3.6V) - NoFinger:" << dec << nTgrIdex_withoutFinger << ",WithFinger:" << dec << nTgrIdex_withFinger;
		//if (!rc1 || !rc2)
		//	return;
		//if (nTgrIdex_withoutFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withoutFinger < Info.m_nMinTriggerThreshold)
		//	return;
		//if (nTgrIdex_withFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withFinger < Info.m_nMinTriggerThreshold)
		//	return;
		int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
		if (nDelta > Info.m_nDelta_200_3p7)
		{
			Results.m_bPass = 1;
			return;
		}
	}
}


bool Ts_ViperWOF::CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx)
{
	int bFound = 0;
	int i;

	//Find the first occurence of 1.
	oTgrIdx = 0;
	for (i=0; (i < nNumThresholds) && (bFound == 0); i++)
	{
		oTgrIdx = i;
		if ((pTriggerBuf[i] & 0x01) == 1)
		{
			if (i > 0)
				oTgrIdx -= 1;
			bFound = 1;
		}
	}
	return bFound;
}