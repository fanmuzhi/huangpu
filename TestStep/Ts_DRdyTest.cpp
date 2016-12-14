#include "Ts_DRdyTest.h"
#include "windows.h"


Ts_DRdyTest::Ts_DRdyTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_DRdyTest::~Ts_DRdyTest()
{
}

void Ts_DRdyTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = 0x10;	//bit 4
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_portId = 1;		//port B


	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_portId= atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		//_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk= atoi(listOfArgValue[1].c_str());
		_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);

}

void Ts_DRdyTest::Execute()
{
	int portID = _pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_portId;
	int pinMsk = _pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk;
	uint32_t nDRdyState;
	uint8_t  pResult[2] = {0,0};

	//set DRdy pin to input.
	_pSyn_DutCtrl->GpioSetPinType(portID, pinMsk, 5);

	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, false);
	_pSyn_DutCtrl->FpDisableSleep();
	_pSyn_DutCtrl->FpWaitForCMDComplete();
	::Sleep(5);

	//read DRdy
	for (int i = 0; i < NUM_DRDY_CHECKS; i++)
	{
		_pSyn_DutCtrl->GpioPinRead(portID, pinMsk, &nDRdyState);
		_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_arHiStates[i] = (nDRdyState & pinMsk) ? 1 : 0;
	}

	_pSyn_DutCtrl->FpReadAndCheckStatus(0);

	::Sleep(5);
	//read DRdy
	for (int i = 0; i < NUM_DRDY_CHECKS; i++)
	{
		_pSyn_DutCtrl->GpioPinRead(portID, pinMsk, &nDRdyState);
		_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_arLoStates[i] = (nDRdyState & pinMsk) ? 1 : 0;
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted = true;

}

void Ts_DRdyTest::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep DRdyTest is not executed!");
		throw ex;
	}
	bool bPass = true;

	for (int i = 0; i < NUM_DRDY_CHECKS; i++)
	{
		if (_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_arHiStates[i] == 0)
			bPass = false;
		if (_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_arLoStates[i] == 1)
			bPass = false;
	}

	_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_bPass = bPass;

	if (!bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sDRdyPinFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DRdyTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DRdyTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_elapsedtime);
}

void Ts_DRdyTest::CleanUp()
{
	PowerOff();
}
