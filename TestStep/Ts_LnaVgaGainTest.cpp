#include "Ts_LnaVgaGainTest.h"

Ts_LnaVgaGainTest::Ts_LnaVgaGainTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_LnaVgaGainTest::~Ts_LnaVgaGainTest()
{
}

void Ts_LnaVgaGainTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nNumResBytes = 2700;
	_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nDelay_ms = 2000;

	std::vector<std::string> listOfArgValue;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

}

void Ts_LnaVgaGainTest::Execute()
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	//load Patch
	Syn_PatchInfo LnaVgaGainTestInfo;
	if (!_pSyn_Dut->FindPatch("LnaVgaGainPatch", LnaVgaGainTestInfo) || NULL == LnaVgaGainTestInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("LnaVgaGain Patch is NULL!");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpLoadPatch(LnaVgaGainTestInfo._pArrayBuf, LnaVgaGainTestInfo._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Load LnaVgaGain Patch Failed");
		throw ex;
	}

	//Get the response.
	rc = _pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nNumResBytes, _pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_nDelay_ms);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Run LnaVgaGain Patch Test Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpUnloadPatch();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Unload LnaVgaGain Patch Failed");
		throw ex;
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_LnaVgaGainTestInfo.m_bExecuted = true;
}

void Ts_LnaVgaGainTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_pResponse[2] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sLnaVgaGainTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("LnaVgaGainTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("LnaVgaGainTest", "Pass"));


	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_LnaVgaGainTestResults.m_elapsedtime);
}

void Ts_LnaVgaGainTest::CleanUp()
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