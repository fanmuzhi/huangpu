#include "Ts_RetainMode.h"
#include "windows.h"


Ts_RetainMode::Ts_RetainMode(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_RetainMode::~Ts_RetainMode()
{
}

void Ts_RetainMode::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nGain = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = 800;	//uA
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = 0;	//uA
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = 500;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nGain = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = (float)atof(listOfArgValue[1].c_str()) * 1000;
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)atof(listOfArgValue[2].c_str())  * 1000;
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = atoi(listOfArgValue[3].c_str());
	
}


void Ts_RetainMode::Execute()
{
	uint32_t rc(0);
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

	//load NAVPatch
	Syn_PatchInfo NAVPatch;
	if (!_pSyn_Dut->FindPatch("NavPatch", NAVPatch) || NULL == NAVPatch._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofPatchInfo Patch is NULL!");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpLoadPatch(NAVPatch._pArrayBuf, NAVPatch._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Load NAV Patch Failed");
		throw ex;
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

	::Sleep(_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay);

	//read current
	uint32_t arrValue[2] = { 0, 0 };		
	_pSynBridge->GetCurrentValues(arrValue, true);		//	low gain

	float spivcc_current = ((float)arrValue[0]) / 1000;	//uA
	float vcc_current = ((float)arrValue[1]) / 1000;		//uA

	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_VCC_uA = vcc_current;
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_SPIVCC_uA = spivcc_current;

	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = true;
}


void Ts_RetainMode::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = true;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_VCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent)||
		(_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_VCC_uA < _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofWovarCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Pass"));
	}
}


void Ts_RetainMode::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc(0);

	_pSyn_DutCtrl->PowerOff();
	rc = _pSyn_DutCtrl->PowerOn(_pSyn_Dut->_uiDutpwrVddh_mV, _pSyn_Dut->_uiDutpwrVdd_mV);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("PowerOn command failed!");
		throw ex;
		return;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
		return;
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_elapsedtime);
}
