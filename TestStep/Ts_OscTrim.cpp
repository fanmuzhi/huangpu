#include "Ts_OscTrim.h"

Ts_OscTrim::Ts_OscTrim(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_OscTrim::~Ts_OscTrim()
{
}

void Ts_OscTrim::SetUp()
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

	//Parse Args
	_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nLowerLimit_Hz = 122;
	_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nUpperLimit_Hz = 123;
	_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nInitialTrim = 0x380;
	_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.m_OscTrimDefault = 0x0;

	std::vector<std::string> listOfArgValue;
	_pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._bExecuted = false;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nLowerLimit_Hz = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nUpperLimit_Hz = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.nInitialTrim = strtol(listOfArgValue[2].c_str(),NULL,0);
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.m_OscTrimDefault = strtol(listOfArgValue[3].c_str(),NULL,0);

	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_OscTrim::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo.m_bExecuted = true;
	_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_nOscTrim = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_bPass = 1;

	uint32_t nOscTrim(0);
	GetOscValues(nOscTrim);
	//if (0==nOscTrim)
	//{
	//	TrimOsc(_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo, _pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults, _pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV);
	//}
	//else
	//{
	//	_pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_nOscTrim = nOscTrim;
	//}
	TrimOsc(_pSyn_Dut->_pSyn_DutTestInfo->_OscTrimInfo, _pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults, _pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV);
}

void Ts_OscTrim::ProcessData()
{
	if (0 == _pSyn_Dut->_pSyn_DutTestResult->_OscTrimResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sOscTrimCalFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OscTrim", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OscTrim", "Pass"));
	}
}

void Ts_OscTrim::CleanUp()
{
	PowerOff();
}

void Ts_OscTrim::GetOscValues(uint32_t &oOscTrim)
{
	int	timeout(100);
	uint8_t	pDst[6] = { 0 };
	//uint8_t data_written_oscTrim[5] = { 0x8, 0x0B, 0x00, 0x80, 4 };//register address specific for OscTrim.

	_pSyn_DutCtrl->FpPeekRegister(0x80000B08, pDst);
	//OscTrim acquired here. It needs to be sent to a function later on.
	oOscTrim = (pDst[3] << 8) + pDst[2];
}