#include "Ts_SCM_WOF.h"

Ts_SCM_WOF::Ts_SCM_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_SCM_WOF::~Ts_SCM_WOF()
{
}

void Ts_SCM_WOF::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_100 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMinTriggerThreshold = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes = 134;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumNoiseSamples = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nStdDevThreshold = (float)1.5;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200 = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nVCC = (float)3.7;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 12)
	{
		for (size_t t = 1; t <= 12 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nStdDevThreshold = std::stof(listOfArgValue[8]);
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nVCC = std::stof(listOfArgValue[11]);

	////load OTPReadWritePatch
	//Syn_PatchInfo OTPRWPatchInfo;
	//if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	//{
	//	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	//	ex.SetDescription("OtpReadWritePatch Patch is NULL!");
	//	throw ex;
	//	return;
	//}
	//_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);

	//Syn_PatchInfo ScmWofPatchInfo, Cmd1ScmWofPlotInfo, Cmd2ScmWofBinInfo, Cmd3SweepTagInfo;
	//if (!_pSyn_Dut->FindPatch("ScmWofPatch", ScmWofPatchInfo) || NULL == ScmWofPatchInfo._pArrayBuf
	//	|| !_pSyn_Dut->FindPatch("Cmd1ScmWofPlot", Cmd1ScmWofPlotInfo) || NULL == Cmd1ScmWofPlotInfo._pArrayBuf
	//	|| !_pSyn_Dut->FindPatch("Cmd2ScmWofBin", Cmd2ScmWofBinInfo) || NULL == Cmd2ScmWofBinInfo._pArrayBuf
	//	|| !_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagInfo) || NULL == Cmd3SweepTagInfo._pArrayBuf)
	//{
	//	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	//	ex.SetDescription("ScmWofPatch or Cmd1ScmWofPlot or Cmd2ScmWofBin or Cmd3SweepTag Patch is NULL!");
	//	throw ex;
	//}

}

void Ts_SCM_WOF::Execute()
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

	//uint8_t pOtpData[MS0_SIZE] = { 0 };
	//int SCM_WOF_BOTCount = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, pOtpData, MS0_SIZE);//EXT_TAG_WOF_BOT

	//if (GetSite().m_calibrationResults.m_nScmWofBot_count == 0)
	//uint8_t pDst[MS0_SIZE] = { 0 };
	//int count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, pDst, MS0_SIZE);
	//if (0 == count)
	//{

		if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus)  // WithStimulus
		{
			// run WOF test with stimulus at normal voltage
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount = 0;
			//3.3V
			SCM_WofTestExecute();
			SCM_WofTestProcessData();

			// If not PASS, run WOF test with stimulus at high voltage
			if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass == 0)
			{
				//3.7V
				SCM_WofTestExecute();
				SCM_WofTestProcessData();
			}
			_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithStimulus = true;
		}
		else  // Without Stimulus
		{
			//3.3V
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount = 0;
			SCM_WofTestExecute();
			SCM_WofTestProcessData();
			_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithoutStimulus = true;
		}
	//}
	//else //Use the SCM WOF values stored in OTP.
	//{
	//	GetOtpScmWofData(pOtpData, SCM_WOF_BOTCount);
	//	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;
	//}
}

void Ts_SCM_WOF::ProcessData()
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

	//
	double dCurrentElapsedTime(0);
	ComputeRunningTime(dCurrentElapsedTime);
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_elapsedtime += dCurrentElapsedTime;

	if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus)  // WithStimulus
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithStimulus", "Fail"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Fail"));
		}
	}
	else
		if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus)  // WithStimulus
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithStimulus", "Pass"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Pass"));
		}
}

void Ts_SCM_WOF::CleanUp()
{
	PowerOff();
}


void Ts_SCM_WOF::SCM_WofTestExecute()
{
	Syn_Exception ex(0);

	int			timeout;
	Syn_PatchInfo ScmWofPatchInfo, Cmd1ScmWofPlotInfo, Cmd2ScmWofBinInfo, Cmd3SweepTagInfo, WofCmd2Info;
	_pSyn_Dut->FindPatch("ScmWofPatch", ScmWofPatchInfo);
	_pSyn_Dut->FindPatch("Cmd1ScmWofPlot", Cmd1ScmWofPlotInfo);
	_pSyn_Dut->FindPatch("Cmd2ScmWofBin", Cmd2ScmWofBinInfo);
	_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagInfo);
	_pSyn_Dut->FindPatch("WofCmd2", WofCmd2Info);

	if (NULL == ScmWofPatchInfo._pArrayBuf || NULL == Cmd1ScmWofPlotInfo._pArrayBuf || NULL == Cmd2ScmWofBinInfo._pArrayBuf || NULL == Cmd3SweepTagInfo._pArrayBuf || NULL == WofCmd2Info._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("SCMWOF Patch is NULL!");
		throw ex;
		return;
	}

	uint8_t*    pWofCmd2Gains = WofCmd2Info._pArrayBuf;
	uint8_t*	pWofCmd1 = Cmd1ScmWofPlotInfo._pArrayBuf;
	int			nCmd1Size = Cmd1ScmWofPlotInfo._uiArraySize;
	uint8_t*	pWofCmd2 = Cmd2ScmWofBinInfo._pArrayBuf;
	int			nCmd2Size = Cmd2ScmWofBinInfo._uiArraySize;
	uint8_t*	pWofCmd3 = Cmd3SweepTagInfo._pArrayBuf;
	int			nCmd3Size = Cmd3SweepTagInfo._uiArraySize;
	uint8_t		pGetPrintMerged[3000] = { 0 };
	int			nGetPrintMergedSize = nCmd2Size + nCmd3Size;
	int			nVCC = (int)(_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nVCC * 1000);

	PowerOff();
	if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount == 0)
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	else
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVCC, nVCC, true);

	//Get gains to use out of the Cmd2 file.
	uint8_t arGainIdx[MAX_THRESHOLD_GAINS] = { 0x1E, 0x22, 0x26, 0x0C, 0x10, 0x14 };
	uint8_t arGainMergeIdx[MAX_THRESHOLD_GAINS] = { 0x1E, 0x22, 0x26, 0x0B, 0x0F, 0x13 };
	for (int i = 0; i<MAX_THRESHOLD_GAINS; i++)
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arStartStopThreshGains[i] = pWofCmd2Gains[arGainIdx[i]];

	//Modify Cmd2 
	//Write cmd2. 
	uint8_t* pTempBuf = new uint8_t[nCmd2Size];
	memcpy(pTempBuf, pWofCmd2, nCmd2Size);
	pTempBuf[0x1E] = 0;
	pTempBuf[0x23] = 0;
	pTempBuf[0x26] = 0xDF;
	pTempBuf[0xB] = 0x80;
	pTempBuf[0xF] = 0;
	pTempBuf[0x13] = 0;

	//Merge GetPrint
	for (int i = 0; i<MAX_THRESHOLD_GAINS; i++)
		pWofCmd3[arGainMergeIdx[i]] = _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arStartStopThreshGains[i];
	pGetPrintMerged[0] = 0x70;
	pGetPrintMerged[1] = 0;
	pGetPrintMerged[2] = 0;
	pGetPrintMerged[3] = 0;
	memcpy(&pGetPrintMerged[4], pTempBuf, nCmd2Size);
	memcpy(&pGetPrintMerged[nCmd2Size + 4], pWofCmd3, nCmd3Size);

	pGetPrintMerged[nCmd2Size + 4 + 11] = 100;
	pGetPrintMerged[nCmd2Size + 4 + 15] = 100;
	pGetPrintMerged[nCmd2Size + 4 + 19] = 200;
	pGetPrintMerged[nCmd2Size + 4 + 38] = 63;

	delete[] pTempBuf;

	//Download patch.
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(ScmWofPatchInfo._pArrayBuf, ScmWofPatchInfo._uiArraySize);

	//Write cmd1.
	_pSyn_DutCtrl->FpWrite(1, pWofCmd1[0], &pWofCmd1[1], nCmd1Size - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//write cmd2 with GetPrintMerged
	_pSyn_DutCtrl->FpWrite(1, 0x02, pGetPrintMerged, nGetPrintMergedSize + 4);
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	//Execute command to read SCM WOF data.
	_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	//Get response data.
	_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);
	timeout = 1000;
	while (timeout && !((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[0] == 0x00) && (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[1] == 0x00)))
	{
		//Try retrieving response data again.
		//Execute command to read SCM WOF data.
		_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);
		_pSyn_DutCtrl->FpWaitForCMDComplete();
		_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCMWOF: cannot get plot");
		throw(ex);
	}

	//Organize bits from the response of the sensor
	for (int i = MAX_THRESHOLD_GAINS; i < _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] = ((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] == 0) || (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] == 2)) ? 1 : 0;
	}
}

void Ts_SCM_WOF::SCM_WofTestProcessData()
{
	int			i;
	int			nTgrIdx;
	uint8_t		m_arGains[2] = { 0 };
	int			nRecSize = (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes - 6) / 2;
	int			bTgrFound = 0;
	int			nDelta;
	uint8_t*	pBuf;

	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;	//Assume true.

	if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus == 0)  // If NO Stimulus
	{
		//Find the trigger index from gain of 100.
		nTgrIdx = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i + 6] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = nTgrIdx - 1;

		//Get the trigger index for the second gain.
		nTgrIdx = 0;
		bTgrFound = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i + 6 + nRecSize] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = nTgrIdx - 1;

		//If invalid threshold from gain of 100.
		if ((nTgrIdx < _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMinTriggerThreshold) || (nTgrIdx >= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMaxTriggerThreshold))
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
		}
	}
	else if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount == 0)	//With stimulus at 3.3V.
	{
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 1;

		//Get trg index at gain 100.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = nTgrIdx - 1;

		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 - _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100;
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_100)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 0;
			nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 - _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200;
			if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200)
			{
				_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 0;
				_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
			}
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount++;
	}
	else //With stimulus at 3.7V.
	{
		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}

		// To save log for 3.7v pass
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200_3p7 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 - (nTgrIdx - 1);
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200_3p7)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 0;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
		}
	}
}

void Ts_SCM_WOF::GetOtpScmWofData(uint8_t pOtpData[MS0_SIZE], int OtpScmWofDataCount)
{
	uint32_t    extTagData = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bValid = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200_3p7 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 0;

	/*Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
	Syn_Exception ex(0);
	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	ex.SetDescription("Wof:OtpReadWritePatch Patch is null!");
	throw ex;
	return;
	}

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);*/


	//if (GetMS0RecordData(TAG_CAL, EXT_TAG_WOF_BOT, pOtpData, MS0_SIZE, site) != 0)
	if (0 != OtpScmWofDataCount)
	{
		extTagData = *((uint32_t*)pOtpData);
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = (uint8_t)((extTagData & 0x00FF0000) >> 16);
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim / 100) - 1;
		if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim == 0)	//gain 100
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
	}

	// OTP data is real gain. The write_logfile() function multiply 100 after increasing 1.
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bValid = 1;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;
}
