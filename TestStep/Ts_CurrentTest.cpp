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
	_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_bExecuted = true;

	FPSFrame * pFrame = new FPSFrame;
	uint32_t arCurrentVals[NUM_CURRENT_VALUES] = { 0 };
	uint32_t arCurrentSums[NUM_CURRENT_VALUES] = { 0 };
	//CalibrationResults* pCalResults = &site.m_calibrationResults;

	//site.SetPercentDone(0);

	try
	{
		//Get the current readings when DUT is actively acquiring an image.
		for (int i = 0; i<NUM_CURRENT_DRAW_READINGS; i++)
		{
			//Each call to GetFingerprintImage returns a current draw reading that
			//is generated by taking two readings and returning the average.
			//GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, _pSyn_Dut->_RowNumber, _pSyn_Dut->_ColumnNumber);
			GetFingerprintImageForCurrentTest(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, _pSyn_Dut->_RowNumber, _pSyn_Dut->_ColumnNumber, arCurrentVals, _pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain);
			for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
				arCurrentSums[curIdx] += arCurrentVals[curIdx];

			//site.SetPercentDone(((i * 50) / NUM_CURRENT_DRAW_READINGS));
		}
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC4] = arCurrentSums[ADC4] / NUM_CURRENT_DRAW_READINGS;

		//cleanup
		memset(arCurrentVals, 0, sizeof(arCurrentVals));
		memset(arCurrentSums, 0, sizeof(arCurrentSums));

		//Get the current readings when DUT is actively acquiring an image.
		for (int i = 0; i<NUM_CURRENT_DRAW_READINGS; i++)
		{
			//Each call to GetFingerprintImage returns a current draw reading that
			//is generated by taking two readings and returning the average.																					
			//GetFingerprintImage(site.m_calibrationResults, pFrame, GetSysConfig().GetNumRows(), GetSysConfig().GetNumCols(), arCurrentVals, site.m_currentInfo.m_nHighGain);
			//GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, _pSyn_Dut->_RowNumber, _pSyn_Dut->_ColumnNumber);
			GetFingerprintImageForCurrentTest(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, _pSyn_Dut->_RowNumber, _pSyn_Dut->_ColumnNumber, arCurrentVals, _pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain);
			for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
				arCurrentSums[curIdx] += arCurrentVals[curIdx];

			//site.SetPercentDone((50 + ((i * 50) / NUM_CURRENT_DRAW_READINGS)));
		}
		
		_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC1] = arCurrentSums[ADC1] / NUM_CURRENT_DRAW_READINGS;
	}
	catch (Syn_Exception ex)
	{
		delete pFrame;
		throw ex;
	}

	//site.SetPercentDone(100);

	delete pFrame;
}

void Ts_CurrentTest::ProcessData()
{
	int arGains[4] = { 1, 10, 100, 1000 };
	unsigned long int	current_uA = 0;
	//AdcBaseLineInfo	ablInfo = _pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_ablInfo;
	AdcBaseLineInfo	ablInfo = _pSyn_Dut->_pSyn_DutTestInfo->_adcBaselineInfo;

	if (_pSyn_Dut->_uiDutpwrVdd_mV != ablInfo.m_nVdd || _pSyn_Dut->_uiDutpwrVio_mV != ablInfo.m_nVio || _pSyn_Dut->_uiDutpwrVled_mV != ablInfo.m_nVled || _pSyn_Dut->_uiDutpwrVddh_mV != ablInfo.m_nVddh)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sVoltageMismatch);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("CurrentTest", "Fail"));
		return;
	}

	//actCurrent_uA = ((3/4096) * ADCCounts * (1000000/gain) (uint32_t)
	/*current_uA = (_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC4] > (int)ablInfo.m_arAdcBaseLines[ADC4][_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain]) ?
		(_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC4] - ablInfo.m_arAdcBaseLines[ADC4][_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain]) : 0;
	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA = (3 * current_uA * (1000000 / arGains[_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nLowGain])) / 4096;

	current_uA = (_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC1] > (int)ablInfo.m_arAdcBaseLines[ADC1][_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain]) ?
		(_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_arImageAcqCurrentVals[ADC1] - ablInfo.m_arAdcBaseLines[ADC1][_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain]) : 0;
	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA = (3 * current_uA * (1000000 / arGains[_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nHighGain])) / 4096;

	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nSleepAnaCurrent_uA = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nSleepDigCurrent_uA = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 1;
	if ((_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA > (_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA)) ||
		(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA < (_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA)))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 0;
	}

	if ((_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA >(_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA)) ||
		(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA < (_pSyn_Dut->_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA)))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass = 0;
	}

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sImagingCurrentFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("CurrentTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("CurrentTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_elapsedtime);*/
}

void Ts_CurrentTest::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}