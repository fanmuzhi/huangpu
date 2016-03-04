#include "Ts_InitializationStep.h"


Ts_InitializationStep::Ts_InitializationStep(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_InitializationStep::~Ts_InitializationStep()
{
}


void Ts_InitializationStep::SetUp()
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

	//InitializationStep
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bBga = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId = 0x08F3;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nBuildNum = 154;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nMaxFileSize_MB = 100;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bVerboseMode = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId = 0;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 14)
	{
		for (size_t t = 1; t <= 14 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bBga = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId = strtoul(listOfArgValue[1].c_str(), NULL, 0);
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nBuildNum = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nMaxFileSize_MB = atoi(listOfArgValue[11].c_str());
	if (0 != listOfArgValue[12].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bVerboseMode = atoi(listOfArgValue[12].c_str());
	if (0 != listOfArgValue[13].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nProductId = strtoul(listOfArgValue[13].c_str(), NULL, 0);
}

void Ts_InitializationStep::Execute()
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
	//GPIO init
	_pSyn_DutCtrl->GpioSetPinType(8, 0x10, 5);	//Pin 13 = input (possible button)	
	_pSyn_DutCtrl->GpioSetPinType(1, 0x10, 5);	//Pin 3 = input (DRDY). Needs xbar setup.
	_pSyn_DutCtrl->GpioSetPinType(6, 0x80, 6);	//Pin 14 = output (Osc)
	_pSyn_DutCtrl->GpioPinWrite(6, 0x80, 0x80);	//Set pin 14 high.
	_pSyn_DutCtrl->GpioSetPinType(7, 0x10, 6);	//Pin 16 = output (Ext reset)
	_pSyn_DutCtrl->GpioPinWrite(7, 0x10, 0x0);	//Set pin 16 high (reverse logic).

	//MPC04 self test

	//MPC04 Get ver
	_pSyn_DutCtrl->FpGetVersion(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo._GerVerArray, VERSION_SIZE);

	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted = true;
}

void Ts_InitializationStep::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep InitializationStep is not executed!");
		throw ex;
	}

}

void Ts_InitializationStep::CleanUp()
{

}


bool Ts_InitializationStep::CheckMPCVoltages()
{
	bool	  bSuccess		 = true;
	float	  c1			 = (float)0.000732;
	float	  nMeasuredVdd   = 0;
	float	  nMeasuredVio   = 0;
	float	  nMeasuredVddtx = 0;
	float	  nMeasuredVled  = 0;

	float	  nTargetVdd    =  (float) _pSyn_Dut->_uiDutpwrVdd_mV/ 1000;
	float	  nTargetVio     = (float) _pSyn_Dut->_uiDutpwrVio_mV/ 1000;
	float	  nTargetVddtx   = (float) _pSyn_Dut->_uiDutpwrVddh_mV/ 1000;
	float	  nTargetVled    = (float) _pSyn_Dut->_uiDutpwrVled_mV/ 1000;

	float	  nPercentLimit  = (float) 0.05; //hardcoded value of 5%
	float	  nPercentError  = (float) 0;

	uint32_t  arValues[MPC_SELF_TEST_BUFFER]; //comes from mpc api

	//Set Voltages to values specified in config and wait for voltages to settle.
	_pSyn_DutCtrl->SetVoltages(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV);

	//Get Self Test Values:
	_pSyn_DutCtrl->FpMpcGetSelfTestResults(64, arValues);

	////Calculate voltages
	nMeasuredVdd   = arValues[12] * c1 * 2;
	nMeasuredVio   = arValues[13] * c1 * 2;
	nMeasuredVddtx = arValues[14] * c1 * 2;
	nMeasuredVled  = arValues[15] * c1 * 2;

	//Save measured voltages for log.
	_pSyn_Dut->_pSyn_DutTestResult->_initResults.m_nMeasuredVdd		= nMeasuredVdd;
	_pSyn_Dut->_pSyn_DutTestResult->_initResults.m_nMeasuredVio		= nMeasuredVio;
	_pSyn_Dut->_pSyn_DutTestResult->_initResults.m_nMeasuredVddtx	= nMeasuredVddtx;
	_pSyn_Dut->_pSyn_DutTestResult->_initResults.m_nMeasuredVled	= nMeasuredVled;

	//Find percentage error
	nPercentError = (nTargetVdd == (float)0.0) ? (float)0.05 : (float)fabs((nMeasuredVdd - nTargetVdd)/nTargetVdd);
	if (nPercentError > nPercentLimit)
		bSuccess = false;

	nPercentError = (nTargetVio == (float)0.0) ? (float)0.05 : (float)fabs((nMeasuredVio - nTargetVio)/nTargetVio);
	if (nPercentError > nPercentLimit)
		bSuccess = false;

	nPercentError = (nTargetVddtx == (float)0.0) ? (float)0.05 : (float)fabs((nMeasuredVddtx - nTargetVddtx)/nTargetVddtx);
	if (nPercentError > nPercentLimit)
		bSuccess = false;

	nPercentError = (nTargetVled == (float)0.0) ? (float)0.05 : (float)fabs((nMeasuredVled - nTargetVled)/nTargetVled);
	if (nPercentError > nPercentLimit)
		bSuccess = false;

	return bSuccess;
}