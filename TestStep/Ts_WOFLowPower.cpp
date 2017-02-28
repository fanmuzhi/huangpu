#include "Ts_WOFLowPower.h"


Ts_WOFLowPower::Ts_WOFLowPower(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WOFLowPower::~Ts_WOFLowPower()
{
}

void Ts_WOFLowPower::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA = 100;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms = 200;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxSpiCurrent_uA = 50;
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinSpiCurrent_uA = 0;
	
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
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxSpiCurrent_uA = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinSpiCurrent_uA = atoi(listOfArgValue[5].c_str());
}

void Ts_WOFLowPower::Execute()
{
	uint32_t rc(0);
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
	Syn_PatchInfo WofLowPowerBinPatchInfo;
	if (!_pSyn_Dut->FindPatch("WofLowPowerBin", WofLowPowerBinPatchInfo) || NULL == WofLowPowerBinPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofLowPowerBin Patch is NULL!");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpRunWOF2CFG(WofLowPowerBinPatchInfo._pArrayBuf, WofLowPowerBinPatchInfo._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Run Wof2 CFG Failed");
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

	float spivcc_current = ((float)arrValue[0]) / 1000;
	float vcc_current = ((float)arrValue[1]) / 1000;

#ifdef _DEBUG
	LOG(DEBUG) << "WOFLowPower SPIVCC Current (uA): " << spivcc_current;
	LOG(DEBUG) << "WOFLowPower VCC Current: (uA) " << vcc_current;
#endif

	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA = vcc_current;
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_SPIVCC_uA = spivcc_current;

	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = true;
}

void Ts_WOFLowPower::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 1;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA)|| 
		(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_VCC_uA < _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA) ||
		(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_SPIVCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxSpiCurrent_uA))
		_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 0;

	if (!_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofLowPowerFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Pass"));
}

void Ts_WOFLowPower::CleanUp()
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