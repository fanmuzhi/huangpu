#include "Ts_Viper_SCMWOF.h"

Ts_Viper_SCMWOF::Ts_Viper_SCMWOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_Viper_SCMWOF::~Ts_Viper_SCMWOF()
{
}

void Ts_Viper_SCMWOF::SetUp()
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

	//_pSyn_Dut->_pSyn_DutTestInfo->_TopSCM_wofInfo.m_bExecutedWithoutStimulus = false;
	//_pSyn_Dut->_pSyn_DutTestInfo->_TopSCM_wofInfo.m_bExecutedWithStimulus = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_100 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nMinTriggerThreshold = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nNumResponseBytes = 134;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nStdDevThreshold = (float)1.5;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nVCC = (float)3.6;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 12)
	{
		for (size_t t = 1; t <= 12 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	//TOP WOF
	if (0 != listOfArgValue[0].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	}
	if (0 != listOfArgValue[1].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	}
	if (0 != listOfArgValue[2].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	}
	if (0 != listOfArgValue[3].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	}
	if (0 != listOfArgValue[4].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	}
	if (0 != listOfArgValue[5].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	}
	if (0 != listOfArgValue[6].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	}
	if (0 != listOfArgValue[7].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	}
	if (0 != listOfArgValue[8].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nStdDevThreshold = std::stof(listOfArgValue[8]);
	}
	if (0 != listOfArgValue[9].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	}
	if (0 != listOfArgValue[10].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	}
	if (0 != listOfArgValue[11].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_nVCC = std::stof(listOfArgValue[11]);
	}
}

void Ts_Viper_SCMWOF::Execute()
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

	double dCurrentElapsedTime(0);

	//Bottom SCM_WOF
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bWithStimulus)  // WithStimulus
	{
		// run WOF test with stimulus at normal voltage
		bool bWithStimulus = _pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bWithStimulus;
		if (this->ExecuteZone0SCMWofTest(_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults,false))
		{
			this->SYN_SCMWofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults);
			_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bExecutedWithStimulus = true;
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_elapsedtime += dCurrentElapsedTime;

		if (_pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		}
	}
	else  // Without Stimulus
	{
		//only execute
		if (this->ExecuteZone0SCMWofTest(_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults,false))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bExecutedWithoutStimulus = true;
			_pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_bPass = 1;
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_elapsedtime += dCurrentElapsedTime;
	}

}

void Ts_Viper_SCMWOF::ProcessData()
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

	if (_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bWithStimulus)  // WithStimulus
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bExecutedWithStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithStimulus", "Pass"));
		}
	}
	else //without Stimulus
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo.m_bExecutedWithoutStimulus && _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Pass"));
		}
	}
}

void Ts_Viper_SCMWOF::CleanUp()
{
	PowerOff();
}

bool Ts_Viper_SCMWOF::ExecuteZone0SCMWofTest(SCM_WofTestInfo& info, SCM_WofTestResults& results, bool UseConfigVotage)
{
	Syn_PatchInfo ScmWofPatchInfo, Cmd1ScmWofPlotInfo, Cmd2ScmWofBinInfo, Cmd3SweepTagInfo;// WofCmd2Info;
	_pSyn_Dut->FindPatch("ScmWofPatch", ScmWofPatchInfo);
	_pSyn_Dut->FindPatch("Cmd1ScmWofPlot", Cmd1ScmWofPlotInfo);
	_pSyn_Dut->FindPatch("Cmd2ScmWofBin", Cmd2ScmWofBinInfo);
	_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagInfo);
	//_pSyn_Dut->FindPatch("WofCmd2", WofCmd2Info);

	if (NULL == ScmWofPatchInfo._pArrayBuf || NULL == Cmd1ScmWofPlotInfo._pArrayBuf || NULL == Cmd2ScmWofBinInfo._pArrayBuf || NULL == Cmd3SweepTagInfo._pArrayBuf)// || NULL == WofCmd2Info._pArrayBuf)
	{
		//Syn_Exception ex(0);
		//ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		//ex.SetDescription("SCMWOF Patch is NULL!");
		//throw ex;
		return false;
	}
	bool		bWithStim = info.m_bWithStimulus;
	int			timeout;
	//uint8_t*    pWofCmd2Gains = WofCmd2Info._pArrayBuf;
	uint8_t*	pWofCmd1 = Cmd1ScmWofPlotInfo._pArrayBuf;
	int			nCmd1Size = Cmd1ScmWofPlotInfo._uiArraySize;
	uint8_t*	pWofCmd2 = Cmd2ScmWofBinInfo._pArrayBuf;
	int			nCmd2Size = Cmd2ScmWofBinInfo._uiArraySize;
	uint8_t*	pWofCmd3 = Cmd3SweepTagInfo._pArrayBuf;
	int			nCmd3Size = Cmd3SweepTagInfo._uiArraySize;
	uint8_t		pGetPrintMerged[3000] = { 0 };
	int			nGetPrintMergedSize = nCmd2Size + nCmd3Size;
	uint8_t*	pResponseBuf = bWithStim ? results.m_arDataWithStim : results.m_arDataWithoutStim;
	int			nVcc = (int)(info.m_nVCC * 1000);

	PowerOff();
	if (UseConfigVotage)
	{
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);
	}
	else
	{
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	}
	ModifySweepSCMWofCmdData(pWofCmd3);

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	results.m_nThreshStart = pWofCmd3[0x1E];
	results.m_nThreshInc = pWofCmd3[0x22];
	results.m_nThreshStop = pWofCmd3[0x26];
	results.m_nNumThresholds = ((results.m_nThreshStop - results.m_nThreshStart) / results.m_nThreshInc) + 1;
	results.m_nGainStart = pWofCmd3[0x0B];
	results.m_nGainInc = pWofCmd3[0x0F];
	results.m_nGainStop = pWofCmd3[0x13];
	results.m_nNumGains = ((results.m_nGainStop - results.m_nGainStart) / results.m_nGainInc) + 1;
	results.m_nResponseSize = (results.m_nNumThresholds * results.m_nNumGains) + 6;

	//Merge the bin and sweep tag files.
	pGetPrintMerged[0] = 0x78;	//Line length (not used).
	memcpy(&pGetPrintMerged[4], pWofCmd2, nCmd2Size);
	memcpy(&pGetPrintMerged[nCmd2Size + 4], pWofCmd3, nCmd3Size);

	//Download patch.
	_pSyn_DutCtrl->FpLoadPatch(ScmWofPatchInfo._pArrayBuf, ScmWofPatchInfo._uiArraySize);

	//Write and wait for cmd1 to complete.
	_pSyn_DutCtrl->FpWrite(1, pWofCmd1[0], &pWofCmd1[1], nCmd1Size - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	///Write and wait for GetPrintMerged to complete.
	_pSyn_DutCtrl->FpWrite(1, 0x02, pGetPrintMerged, nGetPrintMergedSize + 4);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//Send execute command and wait for response.
	timeout = 100;
	do
	{
		_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);
		::Sleep(20);
		_pSyn_DutCtrl->FpRead(1, 0xFF, pResponseBuf, results.m_nResponseSize);
		timeout--;
	} while (timeout && (!((pResponseBuf[0] == 0x00) && (pResponseBuf[1] == 0x00))));

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF: Status never complete.");
		throw(ex);
		return false;
	}

	//Clear registers.
	PowerOff();
	return true;
}

void Ts_Viper_SCMWOF::SYN_SCMWofTestExecute(const SCM_WofTestInfo& pInfo, SCM_WofTestResults& pResults)
{
	pResults.m_bPass = 0;				//Assume fail.
	pResults.m_nTriggerWithoutStim = 0;
	pResults.m_nTriggerWithStim = 0;
	pResults.m_nGain = 0;
	int TempGain200(0), TempTriggerWithoutStimGain200(0), TempTriggerWithStimGain200(0);

	if (pResults.m_nNumGains < 2)
	{
		Syn_Exception ex(0);
		ex.SetDescription("Viper SCMWOF: Gain counts is less than 2.");
		throw(ex); return;
	}

	for (int nGainIdx = 0; nGainIdx < pResults.m_nNumGains; nGainIdx++)
	{
		int nTgrIdex_withoutFinger(0);
		int nTgrIdex_withFinger(0);
		bool rc1 = CalcScmWofTriggerIdx(pResults.m_nNumThresholds, &pResults.m_arDataWithoutStim[6 + (nGainIdx * pResults.m_nNumThresholds)], nTgrIdex_withoutFinger);
		bool rc2 = CalcScmWofTriggerIdx(pResults.m_nNumThresholds, &pResults.m_arDataWithStim[6 + (nGainIdx * pResults.m_nNumThresholds)], nTgrIdex_withFinger);
		pResults.m_nGain = pResults.m_nGainStart + (pResults.m_nGainInc * nGainIdx);
		pResults.m_nTriggerWithoutStim = nTgrIdex_withoutFinger;
		pResults.m_nTriggerWithStim = nTgrIdex_withFinger;
		if (!rc1 || !rc2)
			continue;
		if (nTgrIdex_withoutFinger >= pInfo.m_nMaxTriggerThreshold || nTgrIdex_withFinger < pInfo.m_nMinTriggerThreshold)
			continue;
		if (nTgrIdex_withFinger >= pInfo.m_nMaxTriggerThreshold || nTgrIdex_withFinger < pInfo.m_nMinTriggerThreshold)
			continue;

		int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
		if (0 == nGainIdx)
		{
			//Gain100
			int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
			if (nDelta > pInfo.m_nDelta_100)
			{
				pResults.m_bPass = 1;
				return;
			}
		}
		else
		{
			//Temp value
			TempGain200 = pResults.m_nGainStart + (pResults.m_nGainInc * nGainIdx);
			TempTriggerWithoutStimGain200 = nTgrIdex_withoutFinger;
			TempTriggerWithStimGain200 = nTgrIdex_withFinger;

			//Gain200
			int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
			if (nDelta > pInfo.m_nDelta_200)
			{
				pResults.m_bPass = 1;
				return;
			}
		}
	}

	//3.6V
	this->ExecuteZone0SCMWofTest(_pSyn_Dut->_pSyn_DutTestInfo->_z0SCM_wofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0SCM_wofResults, true);
	int nTgrIdex_withoutFinger(0);
	int nTgrIdex_withFinger(0);
	bool rc1 = CalcScmWofTriggerIdx(pResults.m_nNumThresholds, &pResults.m_arDataWithoutStim[6 + (1 * pResults.m_nNumThresholds)], nTgrIdex_withoutFinger);
	bool rc2 = CalcScmWofTriggerIdx(pResults.m_nNumThresholds, &pResults.m_arDataWithStim[6 + (1 * pResults.m_nNumThresholds)], nTgrIdex_withFinger);
	if (!rc1 || !rc2)
		return;
	if (nTgrIdex_withoutFinger >= pInfo.m_nMaxTriggerThreshold || nTgrIdex_withFinger < pInfo.m_nMinTriggerThreshold)
		return;
	if (nTgrIdex_withFinger >= pInfo.m_nMaxTriggerThreshold || nTgrIdex_withFinger < pInfo.m_nMinTriggerThreshold)
		return;
	int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
	if (nDelta > pInfo.m_nDelta_200_3p7)
	{
		pResults.m_nGain = TempGain200;
		pResults.m_nTriggerWithoutStim = TempTriggerWithoutStimGain200;
		pResults.m_nTriggerWithStim = TempTriggerWithStimGain200;
		pResults.m_bPass = 1;
		return;
	}
}

bool Ts_Viper_SCMWOF::CalcScmWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx)
{
	bool bFound(false);
	oTgrIdx = 0;

	//Find the first occurence of 1.
	for (int i = 0; i<nNumThresholds; i++)
	{
		if (((pTriggerBuf[i] & 0x01) == 0) && (false == bFound))
		{
			oTgrIdx = i;
			bFound = true;
		}
	}

	return bFound;
}