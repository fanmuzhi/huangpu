#include "Ts_RetainMode.h"
#include "windows.h"


Ts_RetainMode::Ts_RetainMode(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_RetainMode::~Ts_RetainMode()
{
}

void Ts_RetainMode::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nGain = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = (float)1000;	//uA
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)0.1;	//uA
	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = 0;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nGain = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = (float)atof(listOfArgValue[1].c_str()) * 1000;
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)atof(listOfArgValue[2].c_str())  * 1000;
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = atoi(listOfArgValue[3].c_str());

	//load NAVPatch
	Syn_PatchInfo NAVPatch;
	if (!_pSyn_Dut->FindPatch("WoeSleepCurrentPatch", NAVPatch) || NULL == NAVPatch._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("WofPatchInfo Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(NAVPatch._pArrayBuf, NAVPatch._uiArraySize);
	
}


void Ts_RetainMode::Execute()
{
	uint32_t rc(0);
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

	//Poke appropriate registers.
	_pSyn_DutCtrl->FpPokeRegister(0x800003A0, 0x00FFFFFF);
	_pSyn_DutCtrl->FpPokeRegister(0x80000374, 0x00000012);

	//load wof2 cofigure
	uint8_t wof2sdk[] = {0x00,0x00,0x00,0x00,0x04,0x10,0x40,0x00,0xcc,0x2c,0x30,0xc8,0x00,0x00,
						 0x00,0x00,0xD0,0x07,0x08,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,0x02,
						 0x01,0x00 };
	_pSyn_DutCtrl->FpRunWOF2CFG(wof2sdk,32);

	//Load and execute the patch. The bin file is prefixed with a command ID. Do not load this ID in data block.
	//Syn_PatchInfo WofLowPowerBinPatchInfo;
	//if (!_pSyn_Dut->FindPatch("WofLowPowerBin", WofLowPowerBinPatchInfo) || NULL == WofLowPowerBinPatchInfo._pArrayBuf)
	//{
	//	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	//	ex.SetDescription("WofLowPowerBin Patch is NULL!");
	//	throw ex;
	//}

	//rc = _pSyn_DutCtrl->FpRunWOF2CFG(WofLowPowerBinPatchInfo._pArrayBuf, WofLowPowerBinPatchInfo._uiArraySize);
	//if (0 != rc)
	//{
	//	ex.SetError(rc);
	//	ex.SetDescription("Run Wof2 CFG Failed");
	//	throw ex;
	//}

	//enter sleep
	_pSyn_DutCtrl->FpTidleSet(0x03E8);

	////load ImgAcqPatch
	//Syn_PatchInfo ImgAcqPatchInfo;
	//if (_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	//{
	//	if (0 != ImgAcqPatchInfo._uiArraySize)
	//	{
	//		_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
	//	}
	//}

	////get print file
	//Syn_PatchInfo PrintFileInfo;
	//if (!_pSyn_Dut->FindPatch("PrintFile", PrintFileInfo))
	//{
	//	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	//	ex.SetDescription("PrintFile Patch is NULL!");
	//	throw ex;
	//	return;
	//}

	//uint16_t row = _pSyn_Dut->_RowNumber;
	//uint16_t column = _pSyn_Dut->_ColumnNumber;
	//if (0 == (row*column) % 64)
	//	row++;
	
	////read current
	////since it's high range, the spi_vcc current is not accurate.
	//uint32_t arrValue[2] = { 0, 0 };
	//rc = _pSyn_DutCtrl->FpGetImage(row,column, PrintFileInfo._uiArraySize, PrintFileInfo._pArrayBuf, arrValue, 2000);
	//if (0 != rc)
	//{
	//	ex.SetError(rc);
	//	ex.SetDescription("FpGetImage Failed");
	//	throw ex;
	//}

	//delay
	//::Sleep(_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nDelay);
	::Sleep(100);

	//read current
	uint32_t arrValue[2] = { 0, 0 };
	_pSynBridge->GetCurrentValues(arrValue, false);		//high gain

	float spivcc_current = ((float)(arrValue[0]) - (float)(_pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[2])) / 1000;	//uA
	float vcc_current = ((float)(arrValue[1]) - (float)(_pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[3])) / 1000;		//uA

	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_VCC_uA = vcc_current;
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_SPIVCC_uA = spivcc_current;

	_pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = true;
}


void Ts_RetainMode::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = true;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_VCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent)||
		(_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_nRetainModeCurrent_SPIVCC_uA > _pSyn_Dut->_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofWovarCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RetainModeCurrentTest", "Pass"));
	}
}


void Ts_RetainMode::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc(0);

	_pSyn_DutCtrl->PowerOff();
	rc = _pSyn_DutCtrl->PowerOn(_pSyn_Dut->_uiDutpwrVddh_mV, _pSyn_Dut->_uiDutpwrVdd_mV);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("PowerOn command failed!");
		throw ex;
		return;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
		return;
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_retainModeResults.m_elapsedtime);
}
