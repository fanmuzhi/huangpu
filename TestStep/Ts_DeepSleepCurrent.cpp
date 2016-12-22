//local
#include "Ts_DeepSleepCurrent.h"

Ts_DeepSleepCurrent::Ts_DeepSleepCurrent(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
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

	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepCurrentInfo._bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepCurrentInfo._highLimit = 25;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 1)
	{
		for (size_t t = 1; t <= 1 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepCurrentInfo._highLimit = std::stof(listOfArgValue[0]);

}

void Ts_DeepSleepCurrent::Execute()
{

	Syn_Exception ex(0);
	_pSyn_DutCtrl->GetBridge(_pSynBridge);

	//Set SleepN to 1
	_pSynBridge->GPIO_SetSleepN(false);

	::Sleep(5);

	//get current
	uint32_t arrValue[2] = { 0, 0 };
	//_pSynBridge->GetCurrentValues(arrValue, false);		//hign gain

	//uint32_t spivcc_current = arrValue[0] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[2];
	//uint32_t vcc_current = arrValue[1] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[3];

	_pSynBridge->GetCurrentValues(arrValue, true);		//low gain

	uint32_t spivcc_current = arrValue[0] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[0];
	uint32_t vcc_current = arrValue[1] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[1];

	_pSyn_Dut->_pSyn_DutTestResult->_deepSleepCurrentResults.spivcc_current = spivcc_current/1000;	//uA
	_pSyn_Dut->_pSyn_DutTestResult->_deepSleepCurrentResults.vcc_current = vcc_current/1000;		//uA

#ifdef _DEBUG
	LOG(DEBUG) << "DeepSleep SPIVCC Current: " << spivcc_current;
	LOG(DEBUG) << "DeepSleep VCC Current: " << vcc_current;
#endif

	//Set SleepN to 0
	_pSynBridge->GPIO_SetSleepN(true);

	_pSyn_Dut->_pSyn_DutTestInfo->_deepSleepCurrentInfo._bExecuted = true;
}

void Ts_DeepSleepCurrent::ProcessData()
{
	//if (_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_DeepSleepCurrentValue > _pSyn_Dut->_pSyn_DutTestInfo->_deepSleepModeInfoInfo.m_SleepCurrentValue)
	//{
	//	_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_bPass = false;
	//	_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofWovarCurrentFail);
	//	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DeepSleepCurrent", "Fail"));
	//}
	//else
	//{
	//	_pSyn_Dut->_pSyn_DutTestResult->_deepSleepModeResults.m_bPass = true;
	//	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DeepSleepCurrent", "Pass"));
	//}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_deepSleepCurrentResults.m_elapsedtime);
}

void Ts_DeepSleepCurrent::CleanUp()
{
}