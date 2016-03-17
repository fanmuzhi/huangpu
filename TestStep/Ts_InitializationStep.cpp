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
	if (! this->CheckMPCVoltages())
	{
		//MPC04 self test fail
		//throw error	
		Syn_Exception ex(Syn_ExceptionCode::Syn_ERR_INITIALIZATION_FAILED);
		ex.SetDescription("TestStep InitializationStep:MPC04 self test fail!");
		throw ex;
	}

	//poweron
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

	if (VERSION_SIZE < 36)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_ERR_INITIALIZATION_FAILED);
		ex.SetDescription("TestStep InitializationStep:VERSION_SIZE error!");
		throw ex;
	}

	//MPC04 Get ver
	try
	{
		_pSyn_DutCtrl->FpGetVersion(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo._GerVerArray, VERSION_SIZE);
	}
	catch (Syn_Exception ex)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sDutCommFail);
		//throw ex;
		return;
	}

	uint8_t *pTempArray = _pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo._GerVerArray;

	//fill the gerVerInfo to DUT serial number
	//fill other info
	//memcpy(&(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.buildtime), &pTempArray[0], 4);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.buildtime = (uint32_t)pTempArray[0] << 0 | (uint32_t)pTempArray[1] << 8 | (uint32_t)pTempArray[2] << 16 | (uint32_t)pTempArray[3] << 24;
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.buildnum = pTempArray[4] << 0 | pTempArray[5] << 8 | pTempArray[6] << 16 | pTempArray[7]<<24;
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.vmajor = pTempArray[8];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.vminor = pTempArray[9];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.target = pTempArray[10];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.product = pTempArray[11];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.siliconrev = pTempArray[12];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.formalrel = pTempArray[13];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.platform = pTempArray[14];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.patch = pTempArray[15];
	memcpy(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number, &(pTempArray[16]), 6);
	memcpy(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.security, &(pTempArray[22]), 2);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.patchsig = pTempArray[24] << 0 | pTempArray[25] << 8 | pTempArray[26] << 16 | pTempArray[27]<<24; 
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.iface = pTempArray[28];
	memcpy(_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.otpsig, &(pTempArray[29]), 3);

	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.otpspare1 = pTempArray[32] << 0 | pTempArray[33]<<8;

	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.reserved = pTempArray[34];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.device_type = pTempArray[35];


	char *cTempValue = new char[3];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[4]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[0] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[1] = cTempValue[1];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[5]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[2] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[3] = cTempValue[1];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[0]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[4] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[5] = cTempValue[1];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[1]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[6] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[7] = cTempValue[1];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[2]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[8] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[9] = cTempValue[1];

	sprintf(cTempValue, "%02X", (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[3]);
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[10] = cTempValue[0];
	_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.sSerialNumber[11] = cTempValue[1];

	_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted = true;

	delete[] cTempValue;
	cTempValue = NULL;
}

void Ts_InitializationStep::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep InitializationStep is not executed!");
		throw ex;
	}

	for (int i = 0; i < DUT_SER_NUM_SIZE; i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_arSerialNum[i] = (_pSyn_Dut->_pSyn_DutTestInfo->_getVerInfo.serial_number)[i];
	}

	//ComputeRunningTime(_pDut)
}

void Ts_InitializationStep::CleanUp()
{
	PowerOff();
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