//local
#include "Ts_SlowOscTrim.h"

Ts_SlowOscTrim::Ts_SlowOscTrim(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_SlowOscTrim::~Ts_SlowOscTrim()
{
}

void Ts_SlowOscTrim::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.nLowerLimit_Hz = 475000;
	_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.nUpperLimit_Hz = 501000;
	_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultTrim = 0x0;
	_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultBias = 0x0;

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
		_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.nLowerLimit_Hz = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.nUpperLimit_Hz = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultTrim = strtol(listOfArgValue[2].c_str(), NULL, 0);
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultBias = strtol(listOfArgValue[3].c_str(), NULL, 0);

	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_SlowOscTrim::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo.m_bExecuted = true;
	_pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_bPass = 1;

	uint32_t nSlowTrim;
	GetSlowOscValues(nSlowTrim);
	if (nSlowTrim == 0x1F04)
	{
		//OscTrim has not been calibrated. Calibrate it.
		TrimSlowOsc(_pSyn_Dut->_pSyn_DutTestInfo->_SlowOscInfo, _pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults, _pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV);
	}
}

void Ts_SlowOscTrim::ProcessData()
{
	if (0 == _pSyn_Dut->_pSyn_DutTestResult->_SlowOscResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sSlowOscTrimCalFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SlowOsc", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SlowOsc", "Pass"));
	}
}

void Ts_SlowOscTrim::CleanUp()
{
	PowerOff();
}

void Ts_SlowOscTrim::GetSlowOscValues(uint32_t &oSlowOscTrim)
{
	//int			timeout;
	uint8_t		pDst[6] = { 0 };

	//uint8_t data_written_slowOsc[5] = { 0x30, 0x03, 0x00, 0x80, 4 };//register address specific for SlowOscTrim.

	//write data
	_pSyn_DutCtrl->FpPeekRegister(0x80000330, pDst);

	//SlowOsc acquired here. It needs to be sent to a function later on.
	oSlowOscTrim = (pDst[3] << 8) + pDst[2];
}