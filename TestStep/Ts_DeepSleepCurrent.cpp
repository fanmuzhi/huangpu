//local
#include "Ts_DeepSleepCurrent.h"

Ts_DeepSleepCurrent::Ts_DeepSleepCurrent(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_DeepSleepCurrent::~Ts_DeepSleepCurrent()
{
}

void Ts_DeepSleepCurrent::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_SleepCurrentValue = 25;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 1)
	{
		for (size_t t = 1; t <= 1 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_SleepCurrentValue = std::stof(listOfArgValue[0]);

	//Power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);

}

void Ts_DeepSleepCurrent::Execute()
{
	_pSyn_DutCtrl->GpioSetPinType(7, 1<<4, 5);
	_pSyn_DutCtrl->GpioPinWrite(7, 1<<4, 1<<4);

	uint16_t nADCBaseline = _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arAdcBaseLines[2][2];
	uint32_t pTemp[NUM_ADC_BASE_READINGS] = { 0, 0, 0, 0 };
	float	arRawADCLow[NUM_ADC_BASE_READINGS] = { 0.0, 0.0, 0.0, 0.0 };
	float	nGainDiff;

	for (int i = 0; i < NUM_CURRENT_DRAW_READINGS; i++)
	{
		_pSyn_DutCtrl->GetCurrentSenseValues(2, 64, pTemp);
		arRawADCLow[0] += pTemp[0] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[1] += pTemp[1] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[2] += pTemp[2] / NUM_CURRENT_DRAW_READINGS;
		arRawADCLow[3] += pTemp[3] / NUM_CURRENT_DRAW_READINGS;
	}

	//Find the difference between the 2 gains
	nGainDiff = (arRawADCLow[2] - (float)nADCBaseline);
	float SleepModeCurrent = 10 * ((nGainDiff * 3) / 4096);

	_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_DeepSleepCurrentValue = SleepModeCurrent;

#ifdef _DEBUG
	LOG(DEBUG) << "DeepSleepCurrent: " << SleepModeCurrent;
#endif

	_pSyn_DutCtrl->GpioSetPinType(7, 1 << 4, 6);
	_pSyn_DutCtrl->GpioPinWrite(7, 1 << 4, 0);

	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_bExecuted = true;
}

void Ts_DeepSleepCurrent::ProcessData()
{
	if (_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_DeepSleepCurrentValue > _pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_SleepCurrentValue)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofWovarCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DeepSleepCurrent", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_bPass = true;
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DeepSleepCurrent", "Pass"));
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_elapsedtime);
}

void Ts_DeepSleepCurrent::CleanUp()
{
	PowerOff();
}