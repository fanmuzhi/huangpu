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

	//MPC04 self test

	//MPC04 Get ver

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