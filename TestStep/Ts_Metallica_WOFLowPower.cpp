#include "Ts_Metallica_WOFLowPower.h"


Ts_Metallica_WOFLowPower::Ts_Metallica_WOFLowPower(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_Metallica_WOFLowPower::~Ts_Metallica_WOFLowPower()
{
}

void Ts_Metallica_WOFLowPower::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA = 50;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms = 200;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms = atoi(listOfArgValue[3].c_str());
}

void Ts_Metallica_WOFLowPower::Execute()
{
	uint32_t rc(0);
	Syn_Exception ex(0);
	///No BIN File for Metallica instead we use a static 'wof2_met_109a' array declared above.
	static uint8_t wof2_met_109a[] = { 0x76, 0x02, 0x04, 0x00, 0x00, 0x0F, 0x30, 0x40, 0x00, 0xCC, 0x0C, 0x0C, 0x02, 0x00, 0x01,
		0x10, 0x10, 0xE8, 0x03, 0x1C, 0x00, 0x03, 0xF0, 0xFF, 0xFF, 0x99, 0x03, 0xF9, 0x02, 0xD8, 0x28,
		0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08 };

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

	//Poke appropriate registers.
	rc = _pSyn_DutCtrl->FpPokeRegister(0x800003A0, 0x00FFFFFF);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Poke 1st Register Failed");
		throw ex;
	}
	rc = _pSyn_DutCtrl->FpPokeRegister(0x80000374, 0x00000012);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Poke 2nd Register Failed");
		throw ex;
	}
	
	//Load and execute the patch. The bin file is prefixed with a command ID. Do not load this ID in data block.
	rc = _pSyn_DutCtrl->FpRunWOF2CFG(&wof2_met_109a[0], sizeof(wof2_met_109a));
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Metallica Run Wof2 CFG Failed");
		throw ex;
	}

	//enter sleep
	rc = _pSyn_DutCtrl->FpTidleSet(0x03E8);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Enter Sleep Failed");
		throw ex;
	}

	::Sleep(_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms);

	//get current
	uint32_t arrValue[2] = { 0, 0 };
	_pSynBridge->GetCurrentValues(arrValue, true);		//low gain

	float spivcc_current = ((float)(arrValue[0]) - (float)(_pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[0])) / 1000;
	float vcc_current = ((float)(arrValue[1]) - (float)(_pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[1])) / 1000;

#ifdef _DEBUG
	LOG(DEBUG) << "WOFLowPower SPIVCC Current (uA): " << spivcc_current;
	LOG(DEBUG) << "WOFLowPower VCC Current: (uA) " << vcc_current;
#endif

	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA = vcc_current;
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_SPIVCC_uA = spivcc_current;

	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = true;
}

void Ts_Metallica_WOFLowPower::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 1;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA) ||
		(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA < _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA) ||
		(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_SPIVCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA))
		_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 0;

	if (!_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofLowPowerFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Pass"));
}

void Ts_Metallica_WOFLowPower::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc = _pSyn_DutCtrl->FpReset();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpReset() Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
		return;
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_elapsedtime);
}