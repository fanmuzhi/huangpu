#include "Ts_DynOffCorrDacTest.h"

Ts_DynOffCorrDacTest::Ts_DynOffCorrDacTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_DynOffCorrDacTest::~Ts_DynOffCorrDacTest()
{
}

void Ts_DynOffCorrDacTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_nNumResBytes = 1200;
	_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_nDelay_ms = 2000;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

}

void Ts_DynOffCorrDacTest::Execute()
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	//load Patch
	Syn_PatchInfo DynOffCorrDacTestInfo;
	if (!_pSyn_Dut->FindPatch("DynOffCorrDacPatch", DynOffCorrDacTestInfo) || NULL == DynOffCorrDacTestInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("DynOffCorrDac Patch is NULL!");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpLoadPatch(DynOffCorrDacTestInfo._pArrayBuf, DynOffCorrDacTestInfo._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Load DynOffCorrDac Patch Failed");
		throw ex;
	}

	//Get the response.
	rc = _pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_nNumResBytes);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Run DynOffCorrDac Patch Test Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpUnloadPatch();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Unload DynOffCorrDac Patch Failed");
		throw ex;
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_DynOffCorrDacTestInfo.m_bExecuted = true;
}

void Ts_DynOffCorrDacTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_pResponse[2] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sDynOffCorrDacTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DynOffCorrDacTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("DynOffCorrDacTest", "Pass"));


	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_DynOffCorrDacTestResults.m_elapsedtime);
}

void Ts_DynOffCorrDacTest::CleanUp()
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
}