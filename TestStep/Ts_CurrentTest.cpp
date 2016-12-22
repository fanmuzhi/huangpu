#include "Ts_CurrentTest.h"

Ts_CurrentTest::Ts_CurrentTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_CurrentTest::~Ts_CurrentTest()
{
}

void Ts_CurrentTest::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain = 3;
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = (int)(50 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = (int)(5 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = (int)(1 * 1000);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = (int)(0.01 * 1000);
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 6)
	{
		for (size_t t = 1; t <= 6 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain = atoi(listOfArgValue[0].c_str()) % 4;//Gain range is 0-3.
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain = atoi(listOfArgValue[1].c_str()) % 4;//Gain range is 0-3.
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = (int)(stof(listOfArgValue[2]) * 1000);//(int)(_tstof((LPCTSTR)listOfArgValue[2].c_str()) * 1000);
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = (int)(stof(listOfArgValue[3]) * 1000);//(int)(_tstof((LPCTSTR)listOfArgValue[3].c_str()) * 1000);
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = (int)(stof(listOfArgValue[4]) * 1000);//(int)(_tstof((LPCTSTR)listOfArgValue[4].c_str()) * 1000);
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = (int)(stof(listOfArgValue[5]) * 1000);//(int)(_tstof((LPCTSTR)listOfArgValue[5].c_str()) * 1000);

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		if (0 != ImgAcqPatchInfo._uiArraySize)
		{
			_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
		}
	}
}

void Ts_CurrentTest::Execute()
{
	Syn_Exception ex(0);
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_bExecuted = true;
	_pSyn_DutCtrl->GetBridge(_pSynBridge);

	//get print file
	Syn_PatchInfo PrintFileInfo;
	if (!_pSyn_Dut->FindPatch("PrintFile", PrintFileInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("PrintFile Patch is NULL!");
		throw ex;
		return;
	}

	uint16_t row = _pSyn_Dut->_RowNumber;
	uint16_t column = _pSyn_Dut->_ColumnNumber;
	if (0 == (row*column) % 64)
		row++;
	
	//read current
	uint32_t arrValue[2] = { 0, 0 };
	uint32_t rc = _pSyn_DutCtrl->FpGetImage(row,column, PrintFileInfo._uiArraySize, PrintFileInfo._pArrayBuf, arrValue, 2000);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpGetImage Failed");
		throw ex;
	}


	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA = (arrValue[0] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[2])/1000;	//hign gain 1.8V
	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA = (arrValue[1] - _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo.m_arrAdcBaseLines[3])/1000;	//hign gain 3.3V

}

void Ts_CurrentTest::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 1;

	if ((_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA > _pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA)|| 
		(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA < _pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA))
		_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 0;

	if (!_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sImagingCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("CurrentTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("CurrentTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_elapsedtime);

}

void Ts_CurrentTest::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc = _pSyn_DutCtrl->FpReset();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Unload Patch Failed");
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