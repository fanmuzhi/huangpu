#include "Ts_ButtonTest.h"

Ts_ButtonTest::Ts_ButtonTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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

	if (0 != listOfArgValue[0].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
	}
	if (0 != listOfArgValue[1].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId = atoi(listOfArgValue[1].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId = atoi(listOfArgValue[1].c_str());
	}
	if (0 != listOfArgValue[2].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk = stoul(listOfArgValue[2]);
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk = stoul(listOfArgValue[2]);
	}
	if (0 != listOfArgValue[3].length())
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_expectedState = atoi(listOfArgValue[3].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_expectedState = atoi(listOfArgValue[3].c_str());
	}

	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_ButtonTest::Execute()
{
	if (_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bWithStimulus == 0)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bExecuted = 1;

		uint32_t	nBtnState;

		//Configure Button pin as input.
		//GPIO_3 is connected to J4 of the microcontroller.
		_pSyn_DutCtrl->GpioSetPinType(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId, _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk, 5);

		//Get the state of the Button pin NUM_DRDY_CHECKS times.
		for (int i = 0; i<NUM_BTN_CHECKS; i++)
		{
			_pSyn_DutCtrl->GpioPinRead(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId, _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk, &nBtnState);
			_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_arCurrStates[i] = (nBtnState & _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk) ? 1 : 0;
		}

		SYN_ProcessBtnTestData(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithoutStimInfo, _pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults);

		ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithoutStimResults.m_elapsedtime);
	}
	else	//Execute with stimulus.
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bExecuted = 1;

		uint32_t	nBtnState;

		//Configure Button pin as input.
		//GPIO_3 is connected to J4 of the microcontroller.
		_pSyn_DutCtrl->GpioSetPinType(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId, _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk, 5);

		//Get the state of the Button pin NUM_DRDY_CHECKS times.
		for (int i = 0; i<NUM_BTN_CHECKS; i++)
		{
			_pSyn_DutCtrl->GpioPinRead(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId, _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk, &nBtnState);
			_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_arCurrStates[i] = (nBtnState & _pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk) ? 1 : 0;
		}

		SYN_ProcessBtnTestData(_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo, _pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults);

		ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_btnTestWithStimResults.m_elapsedtime);
	}
}

void Ts_ButtonTest::ProcessData()
{
	if (_pSyn_Dut->_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bWithStimulus)  // WithStimulus
	{
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
	PowerOff();
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