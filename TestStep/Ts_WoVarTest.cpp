#include "Ts_WoVarTest.h"

Ts_WoVarTest::Ts_WoVarTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WoVarTest::~Ts_WoVarTest()
{
}

void Ts_WoVarTest::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_nNumResBytes = 1000;
	_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_nDelay_ms = 500;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

	//Power On
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load OTPReadWritePatch
	Syn_PatchInfo WovarPatchInfo;
	if (!_pSyn_Dut->FindPatch("WovarPatch", WovarPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WovarPatchInfo Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(WovarPatchInfo._pArrayBuf, WovarPatchInfo._uiArraySize);
}

void Ts_WoVarTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_bExecuted = true;

	//Get the response.
	_pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_woVarInfo.m_nNumResBytes);

	//Reset as work around for bug in Pixel Patch.
	//PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
}

void Ts_WoVarTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_bPass = 0;
	if (_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_pResponse[4] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_woVarResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWoVarTestFail);
	}
}

void Ts_WoVarTest::CleanUp()
{
	//_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}