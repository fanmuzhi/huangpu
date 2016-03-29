#include "Ts_SCM_WOFWithoutStimulus.h"

Ts_SCM_WOFWithoutStimulus::Ts_SCM_WOFWithoutStimulus(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_SCM_WOFWithoutStimulus::~Ts_SCM_WOFWithoutStimulus()
{
}

void Ts_SCM_WOFWithoutStimulus::SetUp()
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
	if (ilistSize < 11)
	{
		for (size_t t = 1; t <= 11 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_100 = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nStdDevThreshold = std::stof(listOfArgValue[7]);
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200 = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nVCC = std::stof(listOfArgValue[10]);

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

	Syn_PatchInfo ScmWofPatchInfo, Cmd1ScmWofPlotInfo, Cmd2ScmWofBinInfo, Cmd3SweepTagInfo;
	if (!_pSyn_Dut->FindPatch("ScmWofPatch", ScmWofPatchInfo) || NULL == ScmWofPatchInfo._pArrayBuf
		|| !_pSyn_Dut->FindPatch("Cmd1ScmWofPlot", Cmd1ScmWofPlotInfo) || NULL == Cmd1ScmWofPlotInfo._pArrayBuf
		|| !_pSyn_Dut->FindPatch("Cmd2ScmWofBin", Cmd2ScmWofBinInfo) || NULL == Cmd2ScmWofBinInfo._pArrayBuf
		|| !_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagInfo) || NULL == Cmd3SweepTagInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("ScmWofPatch or Cmd1ScmWofPlot or Cmd2ScmWofBin or Cmd3SweepTag Patch is NULL!");
		throw ex;
	}

}

void Ts_SCM_WOFWithoutStimulus::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithoutStimulus = true;

	uint8_t pOtpData[MS0_SIZE] = { 0 };
	int SCM_WOF_BOTCount = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, pOtpData, MS0_SIZE);//EXT_TAG_WOF_BOT

	//if (GetSite().m_calibrationResults.m_nScmWofBot_count == 0)
	uint8_t pDst[MS0_SIZE] = { 0 };
	int count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_SCM_WOF_BOT, pDst, MS0_SIZE);
	if (0 == count)
	{
		if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus == 0)
			_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithoutStimulus = true;
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bExecutedWithStimulus = true;

		if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus == 0)  // If NO Stimulus
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount = 0;
			SCM_WofTestExecute();
			SCM_WofTestProcessData();
		}
		else  // With Stimulus
		{
			// run WOF test with stimulus at normal voltage
			SCM_WofTestExecute();
			SCM_WofTestProcessData();

			// If not PASS, run WOF test with stimulus at high voltage
			if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass == 0)
			{
				SCM_WofTestExecute();
				SCM_WofTestProcessData();
			}
		}
	}
	else //Use the SCM WOF values stored in OTP.
	{
		GetOtpScmWofData(pOtpData, SCM_WOF_BOTCount);
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;
	}
}

void Ts_SCM_WOFWithoutStimulus::ProcessData()
{
	if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SCM_WOFWithoutStimulus", "Pass"));
}

void Ts_SCM_WOFWithoutStimulus::CleanUp()
{
	PowerOff();
}