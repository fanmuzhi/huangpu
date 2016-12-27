#include "Ts_WOFFD.h"

#define WOF_REPONSE_HEAD 4
#define WOF_GAIN_START	 1
#define WOF_GAIN_STEP	 1
#define WOF_GAIN_STOP	 3


Ts_WOFFD::Ts_WOFFD(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WOFFD::~Ts_WOFFD()
{
}

void Ts_WOFFD::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nVCC = (float)3.7;
	_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_FingerMode = 0;

	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_100 = 7;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nMinTriggerThreshold = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nNumResponseBytes = 2000;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nStdDevThreshold = (float)0.9;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nVCC = (float)3.7;
	_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_FingerMode = 0;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 13)
	{
		for (size_t t = 1; t <= 13 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	}
	if (0 != listOfArgValue[1].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	}
	if (0 != listOfArgValue[2].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	}
	if (0 != listOfArgValue[3].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	}
	if (0 != listOfArgValue[4].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	}
	if (0 != listOfArgValue[5].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	}
	if (0 != listOfArgValue[6].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	}
	if (0 != listOfArgValue[7].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	}
	if (0 != listOfArgValue[8].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
	}
	if (0 != listOfArgValue[9].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	}
	if (0 != listOfArgValue[10].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	}
	if (0 != listOfArgValue[11].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_nVCC = stof(listOfArgValue[11]);
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_nVCC = stof(listOfArgValue[11]);
	}
	if (0 != listOfArgValue[12].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_FingerMode = stoi(listOfArgValue[12]);
		_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_FingerMode = stoi(listOfArgValue[12]);
	}

}

void Ts_WOFFD::Execute()
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

	double dCurrentElapsedTime(0);

	Syn_PatchInfo WofFdCmd1Patch, WofFdCmd2Patch, WofFdCmd3Patch, WofFdCmd4Patch;
	//Syn_PatchInfo WofFuCmd1Patch, WofFuCmd2Patch, WofFuCmd3Patch, WofFuCmd4Patch;
	rc = _pSyn_Dut->FindPatch("WofFdCmd1", WofFdCmd1Patch);
	rc = _pSyn_Dut->FindPatch("WofFdCmd2", WofFdCmd2Patch);
	rc = _pSyn_Dut->FindPatch("WofFdCmd3", WofFdCmd3Patch);
	rc = _pSyn_Dut->FindPatch("WofFdCmd4", WofFdCmd4Patch);

	/*rc = _pSyn_Dut->FindPatch("WofFuCmd1", WofFuCmd1Patch);
	rc = _pSyn_Dut->FindPatch("WofFuCmd2", WofFuCmd2Patch);
	rc = _pSyn_Dut->FindPatch("WofFuCmd3", WofFuCmd3Patch);
	rc = _pSyn_Dut->FindPatch("WofFuCmd4", WofFuCmd4Patch);*/

	//zone0 bottom
	if (0 == _pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bWithStimulus)
	{
		//zone0 finger down
		if (GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, WofFdCmd1Patch, WofFdCmd2Patch))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bExecutedWithoutStimulus = 1;
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}
	else	//Execute with stimulus.
	{
		//zone0 finger down
		if (GetZ0WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults, WofFdCmd1Patch, WofFdCmd2Patch))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo.m_bExecutedWithStimulus = 1;
			SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z0FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults);
		}

		if (_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}

	//zone1 top
	if (_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bWithStimulus == 0)
	{
		//zone1 finger down
		if (GetZ1WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults, WofFdCmd3Patch, WofFdCmd4Patch))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bExecutedWithoutStimulus = 1;
		}

		ComputeRunningTime(dCurrentElapsedTime);
		_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}
	else	//Execute with stimulus.
	{
		//zone1 finger down
		if (GetZ1WofData(_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults, WofFdCmd3Patch, WofFdCmd4Patch))
		{
			_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo.m_bExecutedWithStimulus = 1;
			SYN_WofTestExecute(_pSyn_Dut->_pSyn_DutTestInfo->_z1FDWofInfo, _pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults);
		}

		if (_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_bPass == 0)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		}

		_pSyn_Dut->_pSyn_DutTestResult->_z1FDWofResults.m_elapsedtime += dCurrentElapsedTime;
	}
}

void Ts_WOFFD::ProcessData()
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
		if ((pDutTestInfo->_z0FDWofInfo.m_bExecutedWithStimulus && pDutTestResult->_z0FDWofResults.m_bPass == 0)
			|| (pDutTestInfo->_z1FDWofInfo.m_bExecutedWithStimulus && pDutTestResult->_z1FDWofResults.m_bPass == 0))
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
		if ((pDutTestInfo->_z0FDWofInfo.m_bExecutedWithoutStimulus && pDutTestResult->_z0FDWofResults.m_bPass == 0)
			|| (pDutTestInfo->_z1FDWofInfo.m_bExecutedWithoutStimulus && pDutTestResult->_z1FDWofResults.m_bPass == 0))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithoutStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FD_WithoutStimulus", "Pass"));
		}
	}
}

void Ts_WOFFD::CleanUp()
{
}

bool Ts_WOFFD::GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd1Patch, Syn_PatchInfo &WofCmd2Patch)
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	bool		bWithStim = wofInfo.m_bWithStimulus ? true : false;
	uint8_t*	pResponseBuf = bWithStim ? wofResults.m_arDataWithStim : wofResults.m_arDataWithoutStim;

	//Load WOF Patch
	Syn_PatchInfo WofPatchInfo;
	_pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	if (NULL == WofPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WOF Patch is NULL!");
		throw ex;
	}
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	//Fill settins
	WofCmd2Patch._pArrayBuf[0x0C] = WOF_GAIN_START;	
	WofCmd2Patch._pArrayBuf[0x10] = WOF_GAIN_STEP;	
	WofCmd2Patch._pArrayBuf[0x14] = WOF_GAIN_STOP;	

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	wofResults.m_nThreshStart = WofCmd2Patch._pArrayBuf[0x1E];
	wofResults.m_nThreshInc = WofCmd2Patch._pArrayBuf[0x22];
	wofResults.m_nThreshStop = WofCmd2Patch._pArrayBuf[0x26];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd2Patch._pArrayBuf[0x0C];
	wofResults.m_nGainInc = WofCmd2Patch._pArrayBuf[0x10];
	wofResults.m_nGainStop = WofCmd2Patch._pArrayBuf[0x14];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + WOF_REPONSE_HEAD;

	//Run WOF Patch
	rc = _pSyn_DutCtrl->FpRunWOFPlot(WofCmd1Patch._pArrayBuf, WofCmd1Patch._uiArraySize, WofCmd2Patch._pArrayBuf, WofCmd2Patch._uiArraySize, pResponseBuf, wofResults.m_nResponseSize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("GetZ0WofData() Failed");
		throw ex;
	}

	//Clear registers.
	rc = _pSyn_DutCtrl->FpReset();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpReset() Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
	}
	return true;
}

bool Ts_WOFFD::GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd3Patch, Syn_PatchInfo &WofCmd4Patch)
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	bool		bWithStim = wofInfo.m_bWithStimulus ? true : false;
	uint8_t*	pResponseBuf = bWithStim ? wofResults.m_arDataWithStim : wofResults.m_arDataWithoutStim;

	//Load WOF Patch
	Syn_PatchInfo WofPatchInfo;
	_pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	if (NULL == WofPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WOF Patch is NULL!");
		throw ex;
	}
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	//Fill the settings
	WofCmd4Patch._pArrayBuf[0x0A] = WOF_GAIN_START;
	WofCmd4Patch._pArrayBuf[0x0E] = WOF_GAIN_STEP;
	WofCmd4Patch._pArrayBuf[0x12] = WOF_GAIN_STOP;

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	wofResults.m_nThreshStart = WofCmd4Patch._pArrayBuf[0x1F];
	wofResults.m_nThreshInc = WofCmd4Patch._pArrayBuf[0x23];
	wofResults.m_nThreshStop = WofCmd4Patch._pArrayBuf[0x27];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd4Patch._pArrayBuf[0x0A];
	wofResults.m_nGainInc = WofCmd4Patch._pArrayBuf[0x0E];
	wofResults.m_nGainStop = WofCmd4Patch._pArrayBuf[0x12];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + WOF_REPONSE_HEAD;

	//Run WOF Patch
	rc = _pSyn_DutCtrl->FpRunWOFPlot(WofCmd3Patch._pArrayBuf, WofCmd3Patch._uiArraySize, WofCmd4Patch._pArrayBuf, WofCmd4Patch._uiArraySize, pResponseBuf, wofResults.m_nResponseSize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("GetZ1WofData() Failed");
		throw ex;
	}

	//Clear registers.
	rc = _pSyn_DutCtrl->FpReset();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpReset() Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
	}

	return true;
}

void Ts_WOFFD::SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results)
{
	std::vector<int>	wofValWithoutFinger, wofValWithFinger;
	int	nGainIdx;

	Results.m_bPass = 0;				//Assume fail.
	Results.m_nTriggerWithoutStim = 0;
	Results.m_nTriggerWithStim = 0;
	Results.m_nGain = 0;

	//Find the gain that produced the best SCM WOF trigger data (with stimulus).
	Results.m_nGain = Results.m_nGainStart;
	for (nGainIdx = 0; (nGainIdx < Results.m_nNumGains) && (Results.m_bPass == 0); nGainIdx++)
	{
		int nTgrIdex_withoutFinger(0),nTgrIdex_withFinger(0);
		//value without stimulus.
		bool rc1 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithoutStim[WOF_REPONSE_HEAD + (nGainIdx * Results.m_nNumThresholds)], nTgrIdex_withoutFinger);
		bool rc2 = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithStim[WOF_REPONSE_HEAD + (nGainIdx * Results.m_nNumThresholds)], nTgrIdex_withFinger);
		//LOG(DEBUG) << "WOFFD TgrIdex_withoutFinger:" << dec << nTgrIdex_withoutFinger << ",TgrIdex_withFinger:" << dec << nTgrIdex_withFinger;
#ifdef _DEBUG
		LOG(DEBUG) << "WOFFD Gain:" << dec << Results.m_nGainStart + (Results.m_nGainInc * nGainIdx) << ",NoFinger:" << dec << nTgrIdex_withoutFinger << ",WithFinger:" << dec << nTgrIdex_withFinger << ",Delta:" << nTgrIdex_withoutFinger - nTgrIdex_withFinger;
#endif
		if (!rc1 || !rc2)
		{
			return;
		}
		if (nTgrIdex_withoutFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withoutFinger < Info.m_nMinTriggerThreshold)
		{
			//if any Trigger value out of limition, fail
			return;
		}
		if (nTgrIdex_withFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withFinger < Info.m_nMinTriggerThreshold)
		{
			//if any Trigger value out of limition, fail
			return;
		}

		wofValWithoutFinger.push_back(nTgrIdex_withoutFinger);
		wofValWithFinger.push_back(nTgrIdex_withFinger);
	}

	//LOG(DEBUG) << "WOFFD:";
	//int bestDelta = 0;
	for (nGainIdx = 0; (nGainIdx < Results.m_nNumGains) && (Results.m_bPass == 0); nGainIdx++)
	{
		//Calc the gain.
		int nTgrIdex_withoutFinger = wofValWithoutFinger[nGainIdx];
		int nTgrIdex_withFinger = wofValWithFinger[nGainIdx];
		//LOG(DEBUG) << "WOFFD Gain:" << dec << Results.m_nGainStart + (Results.m_nGainInc * nGainIdx) << ",NoFinger:" <<  dec << nTgrIdex_withoutFinger << ",WithFinger:" << dec << nTgrIdex_withFinger;
		int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
		if (nDelta >= Info.m_nDelta_100)
		{
			//bestDelta = nDelta;
			Results.m_nGain = Results.m_nGainStart + (Results.m_nGainInc * nGainIdx);
			Results.m_nTriggerWithoutStim = nTgrIdex_withoutFinger;
			Results.m_nTriggerWithStim = nTgrIdex_withFinger;
			Results.m_bPass = 1;
#ifdef _DEBUG
			LOG(DEBUG) << "WOFFD(PASS) Gain:" << dec << Results.m_nGain << ",NoFinger:" << dec << nTgrIdex_withoutFinger << ",WithFinger:" << dec << nTgrIdex_withFinger << ",Delta:" << nTgrIdex_withoutFinger - nTgrIdex_withFinger;
#endif
			return;
		}
	}
}

bool Ts_WOFFD::CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx)
{
	bool bFound(false);

	//Find the first occurence of 1.
	oTgrIdx = nNumThresholds;
	int iExpected = 0;
	for (int i = 0; i<nNumThresholds; i++)
	{
		if (((pTriggerBuf[i] & 0x01) != iExpected) && (oTgrIdx == nNumThresholds))
		{
			oTgrIdx = i;
			bFound = true;
			break;
		}
	}

	return bFound;
}