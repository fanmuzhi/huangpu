#include "Ts_WOFFD.h"

Ts_WOFFD::Ts_WOFFD(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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
	ComputeRunningTime(dCurrentElapsedTime);

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

		//_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_elapsedtime += dCurrentElapsedTime;
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

		//_pSyn_Dut->_pSyn_DutTestResult->_z1WofResults.m_elapsedtime += dCurrentElapsedTime;

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

		if ((pDutTestInfo->_z0FUWofInfo.m_bExecutedWithStimulus && pDutTestResult->_z0FUWofResults.m_bPass == 0)
			|| (pDutTestInfo->_z1FUWofInfo.m_bExecutedWithStimulus && pDutTestResult->_z1FUWofResults.m_bPass == 0))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FU_WithStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FU_WithStimulus", "Pass"));
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

		if ((pDutTestInfo->_z0FUWofInfo.m_bExecutedWithoutStimulus && pDutTestResult->_z0FUWofResults.m_bPass == 0)
			|| (pDutTestInfo->_z1FUWofInfo.m_bExecutedWithoutStimulus && pDutTestResult->_z1FUWofResults.m_bPass == 0))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FU_WithoutStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_FU_WithoutStimulus", "Pass"));
		}
	}
}

void Ts_WOFFD::CleanUp()
{
	PowerOff();
}

bool Ts_WOFFD::GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd1Patch, Syn_PatchInfo &WofCmd2Patch)
{
	bool rc(false);

	bool		bWithStim = wofInfo.m_bWithStimulus;
	uint8_t*	pResponseBuf = bWithStim ? wofResults.m_arDataWithStim : wofResults.m_arDataWithoutStim;


	int nVcc = (int)(wofInfo.m_nVCC * 1000);
	PowerOff();
	//PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

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

bool Ts_WOFFD::GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd3Patch, Syn_PatchInfo &WofCmd4Patch)
{
	bool rc(false);

	bool		bWithStim = wofInfo.m_bWithStimulus;
	uint8_t*	pResponseBuf = bWithStim ? wofResults.m_arDataWithStim : wofResults.m_arDataWithoutStim;
	int nVcc = (int)(wofInfo.m_nVCC * 1000);

	Syn_PatchInfo WofPatchInfo;
	rc = _pSyn_Dut->FindPatch("WofPatch", WofPatchInfo);
	//rc = _pSyn_Dut->FindPatch("WofCmd3", WofCmd3PathInfo);
	//rc = _pSyn_Dut->FindPatch("WofCmd4", WofCmd4PathInfo);
	if (NULL == WofPatchInfo._pArrayBuf || NULL == WofCmd3Patch._pArrayBuf || NULL == WofCmd4Patch._pArrayBuf)
	{
		return false;
	}

	PowerOff();
	//PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVcc, nVcc, true);
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

	//Load the patch, execute and wait for cmd3 to complete.
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);

	_pSyn_DutCtrl->FpWrite(1, WofCmd3Patch._pArrayBuf[0], &WofCmd3Patch._pArrayBuf[1], WofCmd3Patch._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//Get start, stop and increment for sweep thresholds and gains. Calc size of sensor response.
	ModifySweepWofCmdData(WofCmd4Patch._pArrayBuf);
	wofResults.m_nThreshStart = WofCmd4Patch._pArrayBuf[0x1F];
	wofResults.m_nThreshInc = WofCmd4Patch._pArrayBuf[0x23];
	wofResults.m_nThreshStop = WofCmd4Patch._pArrayBuf[0x27];
	wofResults.m_nNumThresholds = ((wofResults.m_nThreshStop - wofResults.m_nThreshStart) / wofResults.m_nThreshInc) + 1;
	wofResults.m_nGainStart = WofCmd4Patch._pArrayBuf[0x0A];
	wofResults.m_nGainInc = WofCmd4Patch._pArrayBuf[0x0E];
	wofResults.m_nGainStop = WofCmd4Patch._pArrayBuf[0x12];
	wofResults.m_nNumGains = ((wofResults.m_nGainStop - wofResults.m_nGainStart) / wofResults.m_nGainInc) + 1;
	wofResults.m_nResponseSize = (wofResults.m_nNumThresholds * wofResults.m_nNumGains) + 6;

	//Execute and wait for sweep command to complete.
	_pSyn_DutCtrl->FpWrite(1, WofCmd4Patch._pArrayBuf[0], &WofCmd4Patch._pArrayBuf[1], WofCmd4Patch._uiArraySize - 1);
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
		//value without stimulus.
		int temp = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithoutStim[6 + (nGainIdx * Results.m_nNumThresholds)]);
		wofValWithoutFinger.push_back(temp);

		temp = CalcWofTriggerIdx(Results.m_nNumThresholds, &Results.m_arDataWithStim[6 + (nGainIdx * Results.m_nNumThresholds)]);
		wofValWithFinger.push_back(temp);
	}

	int bestDelta = 0;
	for (nGainIdx = 0; (nGainIdx < Results.m_nNumGains) && (Results.m_bPass == 0); nGainIdx++)
	{
		//Calc the gain.

		int nTgrIdex_withoutFinger = wofValWithoutFinger[nGainIdx];
		int nTgrIdex_withFinger = wofValWithFinger[nGainIdx];
		if (nTgrIdex_withoutFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withoutFinger < Info.m_nMinTriggerThreshold)
		{
			//if any Trigger value out of limition, fail
			Results.m_bPass = 0;
			return;
		}
		if (nTgrIdex_withFinger >= Info.m_nMaxTriggerThreshold || nTgrIdex_withFinger < Info.m_nMinTriggerThreshold)
		{
			//if any Trigger value out of limition, fail
			Results.m_bPass = 0;
			return;
		}

		int nDelta = nTgrIdex_withoutFinger - nTgrIdex_withFinger;
		if (nDelta >= bestDelta)
		{
			bestDelta = nDelta;
			Results.m_nGain = Results.m_nGainStart + (Results.m_nGainInc * nGainIdx);
			Results.m_nTriggerWithoutStim = nTgrIdex_withoutFinger;
			Results.m_nTriggerWithStim = nTgrIdex_withFinger;
		}
	}

	if (bestDelta < Info.m_nDelta_100)
	{
		Results.m_bPass = 0;
	}
	else
	{
		Results.m_bPass = 1;
	}

}

int Ts_WOFFD::CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf)
{
	int i;

	//Find the first occurence of 1.
	int nTgrIdx = 3000;
	int iExpected = pTriggerBuf[0];
	for (i = 0; i<nNumThresholds; i++)
	{
		if (((pTriggerBuf[i] & 0x01) != iExpected) && (nTgrIdx == 3000))
			nTgrIdx = i;
	}
	return nTgrIdx;
}