#include "Ts_FlexIdTest.h"

Ts_FlexIdTest::Ts_FlexIdTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_FlexIdTest::~Ts_FlexIdTest()
{
}

void Ts_FlexIdTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_nNumResBytes = 48;
	_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_nDelay_ms = 500;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

}

void Ts_FlexIdTest::Execute()
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	//load Patch
	Syn_PatchInfo FlexIdTestInfo;
	if (!_pSyn_Dut->FindPatch("FlexIdPatch", FlexIdTestInfo) || NULL == FlexIdTestInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("FlexId Patch is NULL!");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpLoadPatch(FlexIdTestInfo._pArrayBuf, FlexIdTestInfo._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Load FlexId Patch Failed");
		throw ex;
	}

	//Get the response.
	rc = _pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_nNumResBytes);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Run FlexId Patch Test Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpUnloadPatch();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Unload FlexId Patch Failed");
		throw ex;
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_FlexIdTestInfo.m_bExecuted = true;
}

void Ts_FlexIdTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_pResponse[2] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sFlexIdTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("FlexIdTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("FlexIdTest", "Pass"));


	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_FlexIdTestResults.m_elapsedtime);
}

void Ts_FlexIdTest::CleanUp()
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