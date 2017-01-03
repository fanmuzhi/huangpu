#include "Ts_RAMTest.h"


Ts_RAMTest::Ts_RAMTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_RAMTest::~Ts_RAMTest()
{
}

void Ts_RAMTest::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_RAMTestInfo.m_bExecuted = false;

}

void Ts_RAMTest::Execute()
{
	this->LoadRAMPatch("CacheDataRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam,14);

	this->LoadRAMPatch("CacheTagRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam, 14);

	this->LoadRAMPatch("CachInstDataRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam, 14);

	this->LoadRAMPatch("CachInstTagRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam, 14);

	this->LoadRAMPatch("ScmAndMainRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam, 14);

	_pSyn_Dut->_pSyn_DutTestInfo->_RAMTestInfo.m_bExecuted = true;
}

void Ts_RAMTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPass = 1;
	
	if ((_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam[2] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam[3] == 0) &&
		(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam[4] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam[5] == 0))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheDataRam = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheDataRam = 0;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam[2] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam[3] == 0) &&
		(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam[4] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam[5] == 0))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheTagRam = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheTagRam = 0;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam[2] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam[3] == 0) &&
		(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam[4] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam[5] == 0))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstDataRam = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstDataRam = 0;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam[2] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam[3] == 0) &&
		(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam[4] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam[5] == 0))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstTagRam = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstTagRam = 0;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam[2] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam[3] == 0) &&
		(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam[4] == 0) && (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam[5] == 0))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassScmAndMainRam = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassScmAndMainRam = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheDataRam) || !(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCacheTagRam) || !(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstDataRam) ||
		!(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassCachInstTagRam) || !(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPassScmAndMainRam))
		_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPass = 0;


	if (_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.bPass == 0)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sRAMTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RAMTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RAMTest", "Pass"));

}

void Ts_RAMTest::CleanUp()
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

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_elapsedtime);
}

void Ts_RAMTest::LoadRAMPatch(std::string sPatchName, uint8_t* pPatchResults, uint16_t nSize)
{
	uint32_t rc(0);
	Syn_Exception ex(0);
	if (0 == sPatchName.size())
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("RAMPatch name is NULL!");
		throw ex;
	}

	Syn_PatchInfo RamPatchInfo;
	if (!_pSyn_Dut->FindPatch(sPatchName, RamPatchInfo) || NULL == RamPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription(sPatchName+" Patch is NULL!");
		throw ex;
	}
	rc = _pSyn_DutCtrl->FpLoadPatch(RamPatchInfo._pArrayBuf, RamPatchInfo._uiArraySize, pPatchResults, nSize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription(sPatchName + " RAM Patch Load Failed");
		throw ex;
	}
	//Maybe not need to unload patch
	//rc = _pSyn_DutCtrl->FpGetPatchInfo((uint8_t*)&_pSyn_Dut->_pSyn_DutTestInfo->_getPatchInfo, PATCHINFO_SIZE);
	//rc = _pSyn_DutCtrl->FpUnloadPatch();
}