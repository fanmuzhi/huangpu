#include "Ts_RAMTest.h"


Ts_RAMTest::Ts_RAMTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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

	//Power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	//_pSyn_DutCtrl->FpUnloadPatch();

	//load ImgAcqPatch
	/*Syn_PatchInfo OpensShortsPatchInfo;
	if (!_pSyn_Dut->FindPatch("OpensShortsPatch", OpensShortsPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OpensShortsPatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(OpensShortsPatchInfo._pArrayBuf, OpensShortsPatchInfo._uiArraySize);*/
}

void Ts_RAMTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_RAMTestInfo.m_bExecuted = true;

	this->LoadRAMPatch("CacheDataRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheDataRam,14);

	this->LoadRAMPatch("CacheTagRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCacheTagRam, 14);

	this->LoadRAMPatch("CachInstDataRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstDataRam, 14);

	this->LoadRAMPatch("CachInstTagRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseCachInstTagRam, 14);

	this->LoadRAMPatch("ScmAndMainRam", _pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_pResponseScmAndMainRam, 14);

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
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

	//ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_RAMTestResults.m_elapsedtime);
}

void Ts_RAMTest::CleanUp()
{
	PowerOff();
}

void Ts_RAMTest::LoadRAMPatch(std::string sPatchName, uint8_t* pPatchResults, uint16_t nSize)
{
	Syn_Exception ex(0);
	if (0 == sPatchName.size())
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("RAMPatch name is NULL!");
		throw ex;
		return;
	}

	_pSyn_DutCtrl->FpUnloadPatch();

	Syn_PatchInfo RamPatchInfo;
	if (!_pSyn_Dut->FindPatch(sPatchName, RamPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription(sPatchName+" Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(RamPatchInfo._pArrayBuf, RamPatchInfo._uiArraySize);

	_pSyn_DutCtrl->FpRunPatchTest(pPatchResults, nSize);

}