#include "Ts_SCM_WOFWithStimulus.h"

Ts_SCM_WOFWithStimulus::Ts_SCM_WOFWithStimulus(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_SCM_WOFWithStimulus::~Ts_SCM_WOFWithStimulus()
{
}

void Ts_SCM_WOFWithStimulus::SetUp()
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
}

void Ts_SCM_WOFWithStimulus::Execute()
{

}

void Ts_SCM_WOFWithStimulus::ProcessData()
{

}

void Ts_SCM_WOFWithStimulus::CleanUp()
{

}