#include "Ts_OpensShortsTest.h"

Ts_OpensShortsTest::Ts_OpensShortsTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_OpensShortsTest::~Ts_OpensShortsTest()
{
}

void Ts_OpensShortsTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_nNumResBytes = 2056;
	_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_nDelay_ms = 500;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

	_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_bExecuted = false;
}

void Ts_OpensShortsTest::Execute()
{
	uint32_t rc(0);
	Syn_Exception ex(0);

	//load OpenShortPatch
	Syn_PatchInfo OpensShortsPatchInfo;
	if (!_pSyn_Dut->FindPatch("OpensShortsPatch", OpensShortsPatchInfo) || NULL == OpensShortsPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OpensShortsPatch Patch is NULL!");
		throw ex;
		return;
	}

	_pSyn_DutCtrl->FpLoadPatch(OpensShortsPatchInfo._pArrayBuf, OpensShortsPatchInfo._uiArraySize);

	//Get the response.
	rc = _pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_nNumResBytes);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Run OpenShorts Patch Test Failed");
		return;
		throw ex;
	}

	_pSyn_DutCtrl->FpUnloadPatch();

	_pSyn_Dut->_pSyn_DutTestInfo->_opensShortsInfo.m_bExecuted = true;
}

void Ts_OpensShortsTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_bPass = 0;

	if (_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_pResponse[2] & 0x00000001)
		_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_bPass = 1;
	//else
	//	_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass = 0;
	if (_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_pResponse[2] & 0x00000002)
		_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sOpensShortsFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OpensShortsTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OpensShortsTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_opensShortsResults.m_elapsedtime);
}

void Ts_OpensShortsTest::CleanUp()
{
	_pSyn_DutCtrl->FpReset();
}