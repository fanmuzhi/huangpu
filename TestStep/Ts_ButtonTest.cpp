#include "Ts_ButtonTest.h"

Ts_ButtonTest::Ts_ButtonTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_ButtonTest::~Ts_ButtonTest()
{
}

void Ts_ButtonTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId = 8;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk = 0x10;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_expectedState = 1;

	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId = 8;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk = 0x10;
	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_expectedState = 0;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	int iJudegTag(-1);
	if (0 != listOfArgValue[0].length())
	{
		iJudegTag = atoi(listOfArgValue[0].c_str());
		if (0==iJudegTag)
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	}
	if (0 != listOfArgValue[1].length())
	{
		if (0 == iJudegTag)
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId = atoi(listOfArgValue[1].c_str());
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId = atoi(listOfArgValue[1].c_str());
	}
	if (0 != listOfArgValue[2].length())
	{
		if (0 == iJudegTag)
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk = strtoul(listOfArgValue[2].c_str(), NULL, 16);
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk = strtoul(listOfArgValue[2].c_str(), NULL, 16);
	}
	if (0 != listOfArgValue[3].length())
	{
		if (0 == iJudegTag)
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_expectedState = atoi(listOfArgValue[3].c_str());
		else
			_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_expectedState = atoi(listOfArgValue[3].c_str());
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bExecuted = 0;
}

void Ts_ButtonTest::Execute()
{
	bool nBtnState = 0;
	_pSyn_DutCtrl->GetBridge(_pSynBridge);
	if (_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus)
	{
		//Execute with stimulus.
		//Get the state of the Button pin NUM_DRDY_CHECKS times.
		for (int i = 0; i<NUM_BTN_CHECKS; i++)
		{
			_pSynBridge->GPIO_CheckSwitch(&nBtnState);
			//_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_arCurrStates[i] = (*nBtnState & _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk) ? 1 : 0;
			_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_arCurrStates[i] = nBtnState;
		}

		SYN_ProcessBtnTestData(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo, _pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults);

		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bExecuted = 1;
	}
	else	
	{
		//Execute without stimulus.
		//Get the state of the Button pin NUM_DRDY_CHECKS times.
		for (int i = 0; i<NUM_BTN_CHECKS; i++)
		{
			_pSynBridge->GPIO_CheckSwitch(&nBtnState);
			//_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_arCurrStates[i] = (*nBtnState & _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk) ? 1 : 0;
			_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_arCurrStates[i] = nBtnState;
		}

		SYN_ProcessBtnTestData(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo, _pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults);

		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bExecuted = 1;
	}
}

void Ts_ButtonTest::ProcessData()
{
	if (_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus)  // WithStimulus
	{
		ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_elapsedtime);

		if (_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_bPass)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ButtonTestWithStimulus", "Pass"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ButtonTestWithStimulus", "Fail"));
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sButtonFail);
		}
	}
	else //without Stimulus
	{
		ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_elapsedtime);

		if (_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_bPass)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ButtonTestWithoutStimulus", "Pass"));
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ButtonTestWithoutStimulus", "Fail"));
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sButtonFail);
		}
	}
}

void Ts_ButtonTest::CleanUp()
{
	_pSyn_DutCtrl->FpReset();
}

void Ts_ButtonTest::SYN_ProcessBtnTestData(BtnTestInfo &pInfo, BtnTestResults &pResults)
{
	int i;
	uint8_t failState = 0;

	//If the Button pin is not match with ActiveState, it's a fail.
	for (i = 0; i<NUM_BTN_CHECKS; i++)
	{
		if (pResults.m_arCurrStates[i] != pInfo.m_expectedState)
		{
			pResults.m_bPass = 0;
			failState = pResults.m_arCurrStates[i];
			break;
		}
	}

	// Save current status
	pResults.m_btnCurrState = (pResults.m_bPass) ? pResults.m_arCurrStates[0] : failState;
}