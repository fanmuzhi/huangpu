#include "Ts_AFETest.h"

Ts_AFETest::Ts_AFETest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_AFETest::~Ts_AFETest()
{
}

void Ts_AFETest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_nNumResBytes = 2056;
	_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_nDelay_ms = 500;
	
	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

	//power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
	//load Patch
	Syn_PatchInfo AfePatchInfo;
	if (!_pSyn_Dut->FindPatch("AfePatch", AfePatchInfo) || NULL == AfePatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("AfePatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(AfePatchInfo._pArrayBuf, AfePatchInfo._uiArraySize);
}

void Ts_AFETest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_bExecuted = true;

	//Get the response.
	_pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_AFETestInfo.m_nNumResBytes);
}

void Ts_AFETest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_pResponse[4] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sAFETestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("AFETest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("AFETest", "Pass"));


	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_AFETestResults.m_elapsedtime);
}

void Ts_AFETest::CleanUp()
{
	PowerOff();
}