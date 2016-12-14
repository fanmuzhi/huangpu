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
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
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

	//power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
}

void Ts_WOFLowPower::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = true;
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_uA = 0;

	uint16_t	nAdcId = 2;
	uint16_t	nGainId = _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain;
	uint16_t	nADCBaseline = _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arAdcBaseLines[nAdcId][nGainId];
	//uint16_t	nADCBaseline = site.GetLocalSettingsPtr()->GetAdcBaseLine(site.GetSiteNum(), nAdcId, info.m_nGain);
	uint32_t	arTemp[NUM_ADC_BASE_READINGS] = { 0, 0, 0, 0 };
	uint32_t	nAdcSum = 0, nAdcAve;

	//load Patch
	Syn_PatchInfo WofLowPowerPatchInfo;
	if (_pSyn_Dut->FindPatch("WofLowPowerPatch", WofLowPowerPatchInfo) && NULL != WofLowPowerPatchInfo._pArrayBuf)
	{
		_pSyn_DutCtrl->FpLoadPatch(WofLowPowerPatchInfo._pArrayBuf, WofLowPowerPatchInfo._uiArraySize);
	}

	//Poke appropriate registers.
	//FpPokeCmd(0x80000374, 0x00000012);
	//FpPokeCmd(0x800003A0, 0x00FFFFFF);
	_pSyn_DutCtrl->FpPokeRegister(0x80000374, 0x00000012);
	_pSyn_DutCtrl->FpPokeRegister(0x800003A0, 0x00FFFFFF);

	//Load and execute the patch. The bin file is prefixed with a command ID. Do not load this ID in data block.
	Syn_PatchInfo WofLowPowerBinPatchInfo;
	if (!_pSyn_Dut->FindPatch("WofLowPowerBin", WofLowPowerBinPatchInfo) || NULL == WofLowPowerBinPatchInfo._pArrayBuf)
	{
		Syn_Exception ex(0);
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofLowPowerBin Patch is NULL!");
		throw ex;
		return;
	}

	_pSyn_DutCtrl->FpWrite(1, WofLowPowerBinPatchInfo._pArrayBuf[0], &(WofLowPowerBinPatchInfo._pArrayBuf[1]), WofLowPowerBinPatchInfo._uiArraySize - 1);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	//Get an average of multiple ADC readings.
	for (int i = 0; i < NUM_CURRENT_DRAW_READINGS; i++)
	{
		_pSyn_DutCtrl->GetCurrentSenseValues(_pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain, 64, arTemp);
		nAdcSum += arTemp[nAdcId];
	}
	nAdcAve = nAdcSum / NUM_CURRENT_DRAW_READINGS;

	//Calculate current. Subtract the MPC04 base line offset from average ADC reading.
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_uA = (10 * ((float)nAdcAve - (float)nADCBaseline) * 3) / 4096;

	//Clear the registers; bring them back to their default state.		
	//PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

}

void Ts_WOFLowPower::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 1;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_uA > _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA)|| 
		(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_nCurrent_uA < _pSyn_Dut->_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA))
		_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 0;

	if (!_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofLowPowerFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF-LowPower", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_wofLowPowerResults.m_elapsedtime);
}

void Ts_WOFLowPower::CleanUp()
{
	PowerOff();
}