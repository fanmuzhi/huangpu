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
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = (float)1;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)0.1;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = 0;
	
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
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = (float)atof(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)atof(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = atoi(listOfArgValue[3].c_str());


	//Power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

	//load WofPatch
	Syn_PatchInfo WofPatchInfo;
	if (!_pSyn_Dut->FindPatch("WofPatch", WofPatchInfo) || NULL == WofPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofPatchInfo Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(WofPatchInfo._pArrayBuf, WofPatchInfo._uiArraySize);
	
}


void Ts_RetainMode::Execute()
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
	//_pSyn_DutCtrl->FpEnterSleep();

	//uint16_t nADCBaseline = site.GetLocalSettingsPtr()->GetAblInfo(site.GetDutCtrlSerNum()).m_arAdcBaseLines[2][2];//baseline gain.
	uint16_t nADCBaseline = _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arAdcBaseLines[2][2];
	uint32_t pTemp[NUM_ADC_BASE_READINGS] = {0, 0, 0, 0};
	float	arRawADCLow[NUM_ADC_BASE_READINGS] = {0.0, 0.0, 0.0, 0.0};
	float	nGainDiff;

	//uint8_t pDst[NUM_CURRENT_DRAW_READINGS];

	//1st poke
	//uint32_t nData;
	uint8_t pDst[6];
	_pSyn_DutCtrl->FpPeekRegister(0x80002808, pDst);
	_pSyn_DutCtrl->FpPeekRegister(0x80002814, pDst);
	_pSyn_DutCtrl->FpPokeRegister(0x80002808, 0x3);
	_pSyn_DutCtrl->FpPeekRegister(0x80002814, pDst);
	_pSyn_DutCtrl->FpPokeRegister(0x80002814, 0x20);
	_pSyn_DutCtrl->FpPokeRegister(0x80002814, 0x20);
	_pSyn_DutCtrl->FpPeekRegister(0x80002814, pDst);
	_pSyn_DutCtrl->FpPokeRegister(0x80002814, 0x21);
	_pSyn_DutCtrl->FpPokeRegister(0x80002064, 0x10000000);
	_pSyn_DutCtrl->FpPokeRegister(0x80002064, 0x6F20000); 
	_pSyn_DutCtrl->FpPeekRegister(0x80002064, pDst);
	_pSyn_DutCtrl->FpPokeRegister(0x80002064, 0x20000007);

	//2nd poke
	_pSyn_DutCtrl->FpPokeRegister(0x8000030C, 0x41);
	_pSyn_DutCtrl->FpPokeRegister(0x80000388, 0x07D0);
	_pSyn_DutCtrl->FpPokeRegister(0x8000038C, 0x401008);
	_pSyn_DutCtrl->FpPokeRegister(0x80000390, 0xFFFF00C4);
	_pSyn_DutCtrl->FpPokeRegister(0x80000394, 0x0);
	_pSyn_DutCtrl->FpPokeRegister(0x80000300, 0x19, false);


	::Sleep(_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay);

	//Average the 10 readings for low gain
	for(int i = 0; i < NUM_CURRENT_DRAW_READINGS; i++)
	{
		//GetDutCtrl().GetCurrentSenseValues(2, 64, pTemp);
		_pSyn_DutCtrl->GetCurrentSenseValues(2, 64, pTemp);
		arRawADCLow[0] += pTemp[0] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[1] += pTemp[1] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[2] += pTemp[2] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[3] += pTemp[3] / NUM_CURRENT_DRAW_READINGS;
	}

	//Find the difference between the 2 gains
	nGainDiff = (arRawADCLow[2] - (float)nADCBaseline);	
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent = 10 * ((nGainDiff * 3) / 4096);

	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = true;
}


void Ts_RetainMode::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = true;

	if (_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent > _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent ||
		_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent < _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofWovarCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Pass"));
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_elapsedtime);
}


void Ts_RetainMode::CleanUp()
{
	//_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}
