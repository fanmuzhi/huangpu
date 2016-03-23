#include "Ts_PixelPatchTest.h"

#include "windows.h"

Ts_PixelPatchTest::Ts_PixelPatchTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_PixelPatchTest::~Ts_PixelPatchTest()
{
}

void Ts_PixelPatchTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes = 3000;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nDelay_ms = 500;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());

	//Power On
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load OTPReadWritePatch
	Syn_PatchInfo PixelPatchInfo;
	if (!_pSyn_Dut->FindPatch("PixelPatch", PixelPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("PixelPatchInfo Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(PixelPatchInfo._pArrayBuf, PixelPatchInfo._uiArraySize);
	//::Sleep(1000);
}

void Ts_PixelPatchTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_bExecuted = true;
	
	//Get the response.
	_pSyn_DutCtrl->FpRunPatchTest(_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_pResponse, _pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes);

}

void Ts_PixelPatchTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass = 0;
	if (_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_pResponse[4] & 0x01)
		_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sPixPatchFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("PixelPatchTest", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("PixelPatchTest", "Pass"));
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_elapsedtime);
}

void Ts_PixelPatchTest::CleanUp()
{
	//Reset as work around for bug in Pixel Patch.
	//PowerOff();
	//PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	//_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}