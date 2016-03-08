#include "Ts_CurrentTest.h"

Ts_CurrentTest::Ts_CurrentTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_CurrentTest::~Ts_CurrentTest()
{
}

void Ts_CurrentTest::SetUp()
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
	std::vector<std::string> listOfArgValue;

	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = (int)(50 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = (int)(5 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = (int)(1 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = (int)(0.01 * 1000);
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 6)
	{
		for (size_t t = 1; t <= 6 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain = atoi(listOfArgValue[0].c_str()) % 4;//Gain range is 0-3.
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain = atoi(listOfArgValue[1].c_str()) % 4;//Gain range is 0-3.
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = stoi(listOfArgValue[2]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[2].c_str()) * 1000);
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = stoi(listOfArgValue[3]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[3].c_str()) * 1000);
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = stoi(listOfArgValue[4]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[4].c_str()) * 1000);
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = stoi(listOfArgValue[5]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[5].c_str()) * 1000);

}

void Ts_CurrentTest::Execute()
{	
}

void Ts_CurrentTest::ProcessData()
{

}

void Ts_CurrentTest::CleanUp()
{

}