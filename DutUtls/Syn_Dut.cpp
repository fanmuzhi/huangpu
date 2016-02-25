//windows api
//#include "windows.h"

//Local
#include "Syn_Dut.h"
#include "Syn_Exception.h"
#include "Syn_Vcsfw.h"

//std
#include <iostream>

//third-party
//#include "easylogging++.h"

Syn_Dut::Syn_Dut()
:_pSyn_DutTestInfo(NULL)
, _pSyn_DutTestResult(NULL)
{
}

Syn_Dut::~Syn_Dut()
{
	if (NULL != _pSyn_DutTestInfo)
	{
		delete _pSyn_DutTestInfo;
		_pSyn_DutTestInfo = NULL;
	}

	if (NULL != _pSyn_DutTestResult)
	{
		delete _pSyn_DutTestResult;
		_pSyn_DutTestResult = NULL;
	}
}

bool Syn_Dut::CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance)
{
	//opSyn_DutInstance = NULL;

	opSyn_DutInstance = new Syn_Dut();
	opSyn_DutInstance->_eProjectType = iType;

	return true;
}

void Syn_Dut::InitData(Syn_SysConfig &sysConfig)
{
	bool rc(true);

	_uiDutpwrVdd_mV = sysConfig._uiDutpwrVdd_mV;
	_uiDutpwrVio_mV = sysConfig._uiDutpwrVio_mV;
	_uiDutpwrVled_mV = sysConfig._uiDutpwrVled_mV;
	_uiDutpwrVddh_mV = sysConfig._uiDutpwrVddh_mV;

	_RowNumber = sysConfig._uiNumRows;
	_ColumnNumber = sysConfig._uiNumCols;

	SetPatchInfo(sysConfig._listPatchInfo);

	if (NULL == _pSyn_DutTestInfo)
	{
		_pSyn_DutTestInfo = new Syn_DutTestInfo();
	}

	if (NULL == _pSyn_DutTestResult)
	{
		_pSyn_DutTestResult = new Syn_DutTestResult();
	}
	_pSyn_DutTestResult->_arSerialNum[DUT_SER_NUM_SIZE] = {0};
	_pSyn_DutTestResult->_binCodes.clear();

	std::string strArgsValue("");
	std::vector<std::string> listOfArgValue;

	//OTPCheckInfo
	int arrIncrement[4] = {3, 1, -1, -3};	//swap the value
	for (auto i = 0; i < BS0_SIZE; i++)
	{
		_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i] = sysConfig._arrUserSpecifiedBS0[i + arrIncrement[i % 4]];
		_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS1[i] = sysConfig._arrUserSpecifiedBS1[i + arrIncrement[i % 4]];
	}
	_pSyn_DutTestInfo->_otpCheckInfo._bExecuted = false;


	//InitializationStep
	_pSyn_DutTestInfo->_initInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_initInfo.m_bBga = 0;
	_pSyn_DutTestInfo->_initInfo.m_nFlexId = 0x08F3;
	_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim = 1;
	_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim = 1;
	_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim = 1;
	_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim = 1;
	_pSyn_DutTestInfo->_initInfo.m_nBuildNum = 154;
	_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim = 0;
	_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim = 0;
	_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim = 0;
	_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim = 0;
	_pSyn_DutTestInfo->_initInfo.m_nMaxFileSize_MB = 100;
	_pSyn_DutTestInfo->_initInfo.m_bVerboseMode = 0;
	_pSyn_DutTestInfo->_initInfo.m_nProductId = 0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("InitializationStep"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 14)
		{
			for (size_t t = 1; t <= 14 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_initInfo.m_bBga = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_initInfo.m_nFlexId = strtoul(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_initInfo.m_nBuildNum = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_pSyn_DutTestInfo->_initInfo.m_nMaxFileSize_MB = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_pSyn_DutTestInfo->_initInfo.m_bVerboseMode = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_pSyn_DutTestInfo->_initInfo.m_nProductId = strtoul(listOfArgValue[13].c_str(), NULL, 0);
	}

	//Calibrate
	_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx = 1028;
	_pSyn_DutTestInfo->_calibrationInfo.m_nPgaIdx = 1140;
	_pSyn_DutTestInfo->_calibrationInfo.m_bForceCal = 0;
	_pSyn_DutTestInfo->_calibrationInfo.m_nNumImagesToDiscard = 20;
	_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetLow = 108;
	_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetHigh = 148;
	_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitLow = 108;
	_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitHigh = 148;
	_pSyn_DutTestInfo->_calibrationInfo.m_nCalType = 0;
	_pSyn_DutTestInfo->_calibrationInfo.m_nPgaOffsetRatio = (float)0.3;
	_pSyn_DutTestInfo->_calibrationInfo.m_nNumPgaSamples = 4;
	_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit = 90;
	_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset = 0;
	_pSyn_DutTestInfo->_calibrationInfo.m_bPgaFineTuning = 0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("Calibrate"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 14)
		{
			for (size_t t = 1; t <= 14 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nPgaIdx = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_bForceCal = atoi(listOfArgValue[2].c_str()) ? 1 : 0;
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nNumImagesToDiscard = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetLow = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetHigh = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitLow = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitHigh = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nCalType = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nPgaOffsetRatio = stof(listOfArgValue[9]);//(float)_tstof((LPCTSTR)listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nNumPgaSamples = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_pSyn_DutTestInfo->_calibrationInfo.m_bPgaFineTuning = atoi(listOfArgValue[13].c_str());
	}

	//AcquireFPSSamples
	_pSyn_DutTestInfo->_acquireFpsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesToDiscard = 20;
	_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus = 30;
	_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus = 30;
	if (sysConfig.GetSyn_TestStepInfo(std::string("AcquireFPSSamples"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesToDiscard = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus = atoi(listOfArgValue[2].c_str());

		if (_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus > MAXFRAMES)
		{
			Syn_Exception ex(0);
			ex.SetDescription("The maximum frames of images without the stimulus has been exceeded.");
			throw(ex);
		}
		if (_pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus > MAXFRAMES)
		{
			Syn_Exception ex(0);
			ex.SetDescription("The maxinum frames of images with the stimulus has been exceeded.");
			throw(ex);
		}
	}

	//PeggedPixelsTest
	_pSyn_DutTestInfo->_peggedPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames = 30;
	_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row = 9;
	_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col = 28;
	_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits = 255;
	if (sysConfig.GetSyn_TestStepInfo(std::string("PeggedPixelsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus;//atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//CurrentTest
	_pSyn_DutTestInfo->_currentInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_currentInfo.m_nLowGain = 2;
	_pSyn_DutTestInfo->_currentInfo.m_nHighGain = 3;
	_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = (int)(50 * 1000);
	_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = (int)(5 * 1000);
	_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = (int)(1 * 1000);
	_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = (int)(0.01 * 1000);
	if (sysConfig.GetSyn_TestStepInfo(std::string("CurrentTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 6)
		{
			for (size_t t = 1; t <= 6 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_currentInfo.m_nLowGain = atoi(listOfArgValue[0].c_str()) % 4;			//Gain range is 0-3.
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_currentInfo.m_nHighGain = atoi(listOfArgValue[1].c_str()) % 4;		//Gain range is 0-3.
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMax_uA = stoi(listOfArgValue[2]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[2].c_str()) * 1000);
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMax_uA = stoi(listOfArgValue[3]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[3].c_str()) * 1000);
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_currentInfo.m_nImageAcqAnaMin_uA = stoi(listOfArgValue[4]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[4].c_str()) * 1000);
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_currentInfo.m_nImageAcqDigMin_uA = stoi(listOfArgValue[5]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[5].c_str()) * 1000);
	}

	//OpensShortsTest
	_pSyn_DutTestInfo->_opensShortsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_opensShortsInfo.m_nNumResBytes = 2056;
	_pSyn_DutTestInfo->_opensShortsInfo.m_nDelay_ms = 500;
	if (sysConfig.GetSyn_TestStepInfo(std::string("OpensShortsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_opensShortsInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_opensShortsInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//FlooredPixelsTest
	_pSyn_DutTestInfo->_flooredPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_flooredPixelsInfo.numFrames = 30;
	_pSyn_DutTestInfo->_flooredPixelsInfo.fail_row = 9;
	_pSyn_DutTestInfo->_flooredPixelsInfo.fail_col = 28;
	_pSyn_DutTestInfo->_flooredPixelsInfo.fail_limits = 25;
	if (sysConfig.GetSyn_TestStepInfo(std::string("FlooredPixelsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_pSyn_DutTestInfo->_flooredPixelsInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus;
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_flooredPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_flooredPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_flooredPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//DeltaPixelTest
	_pSyn_DutTestInfo->_deltaPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_deltaPixelsInfo.numFrames = 30;
	_pSyn_DutTestInfo->_deltaPixelsInfo.fail_row = 0;
	_pSyn_DutTestInfo->_deltaPixelsInfo.fail_col = 0;
	_pSyn_DutTestInfo->_deltaPixelsInfo.fail_limits = 10;
	if (sysConfig.GetSyn_TestStepInfo(std::string("DeltaPixelTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_deltaPixelsInfo.numFrames = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_deltaPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_deltaPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_deltaPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//ConsecutivePixels
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_peggedThreshold = 225;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_flooredThreshold = 25;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = 4;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_cutEdge = 7;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.numFrames = 30;
	if (sysConfig.GetSyn_TestStepInfo(std::string("ConsecutivePixels"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();//ilistSize is 5??????????????
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_consecutivePixelsInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_consecutivePixelsInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_consecutivePixelsInfo.m_cutEdge = atoi(listOfArgValue[3].c_str());

		_pSyn_DutTestInfo->_consecutivePixelsInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus;
	}

	//PixelTest
	_pSyn_DutTestInfo->_pixelInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_pixelInfo.numFrames = 30;
	_pSyn_DutTestInfo->_pixelInfo.fail_row = 50;
	_pSyn_DutTestInfo->_pixelInfo.fail_col = 50;
	_pSyn_DutTestInfo->_pixelInfo.nTrim = 0;
	_pSyn_DutTestInfo->_pixelInfo.nCountFailedPixels = 0;
	_pSyn_DutTestInfo->_pixelInfo.nThresholdRange = 45;
	if (sysConfig.GetSyn_TestStepInfo(std::string("PixelTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 6)
		{
			for (size_t t = 1; t <= 6 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_pSyn_DutTestInfo->_pixelInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithoutStimulus;
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_pixelInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_pixelInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_pixelInfo.nTrim = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_pixelInfo.nThresholdRange = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_pixelInfo.nCountFailedPixels = atoi(listOfArgValue[5].c_str());
	}

	//PixelPatchTest
	_pSyn_DutTestInfo->_pixelPatchInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes = 3000;
	_pSyn_DutTestInfo->_pixelPatchInfo.m_nDelay_ms = 500;
	if (sysConfig.GetSyn_TestStepInfo(std::string("PixelPatchTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_pixelPatchInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//SNRTest
	_pSyn_DutTestInfo->_snrInfo.m_bExecuted = false;

	int numMaxColsWithStim(0), numMinColsWithStim(0), numMaxRowsWithStim(0), numMinRowsWithStim(0);
	int numMaxColsWOStim(0), numMinColsWOStim(0), numMaxRowsWOStim(0), numMinRowsWOStim(0);

	numMaxColsWithStim = sysConfig._uiNumCols - _pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim;
	numMinColsWithStim = _pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim;
	numMaxRowsWithStim = sysConfig._uiNumRows - _pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim;;
	numMinRowsWithStim = _pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim;
	numMaxColsWOStim = sysConfig._uiNumCols - _pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	numMinColsWOStim = _pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	numMaxRowsWOStim = sysConfig._uiNumRows - _pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	numMinRowsWOStim = _pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;

	_pSyn_DutTestInfo->_snrInfo.numcols = sysConfig._uiNumCols;
	_pSyn_DutTestInfo->_snrInfo.numrows = sysConfig._uiNumRows;
	//Set defaults.
	_pSyn_DutTestInfo->_snrInfo.numFrames = 30;
	_pSyn_DutTestInfo->_snrInfo.numMinCols = 2;
	_pSyn_DutTestInfo->_snrInfo.numMaxCols = 147;
	_pSyn_DutTestInfo->_snrInfo.numMinRows = 1;
	_pSyn_DutTestInfo->_snrInfo.numMaxRows = 55;
	_pSyn_DutTestInfo->_snrInfo.percent = 10;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_high = 500;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_low = 25;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_high = 500;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_low = 80;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high = 5;
	_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low = 0;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_high = 500;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_low = 14;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_high = 500;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_low = 50;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high = 6;
	_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low = 0;

	_pSyn_DutTestInfo->_snrInfo.numMaxCols = (numMaxColsWOStim < numMaxColsWithStim) ? numMaxColsWOStim : numMaxColsWithStim;
	_pSyn_DutTestInfo->_snrInfo.numMinCols = (numMinColsWOStim < numMinColsWithStim) ? numMinColsWithStim : numMinColsWOStim;
	_pSyn_DutTestInfo->_snrInfo.numMaxRows = (numMaxRowsWOStim < numMaxRowsWithStim) ? numMaxRowsWOStim : numMaxRowsWithStim;
	_pSyn_DutTestInfo->_snrInfo.numMinRows = (numMinRowsWOStim < numMinRowsWithStim) ? numMinRowsWithStim : numMinRowsWOStim;

	_pSyn_DutTestInfo->_snrInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus;//Image Count
	if (sysConfig.GetSyn_TestStepInfo(std::string("SNRTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 18)
		{
			for (size_t t = 1; t <= 18 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_snrInfo.percent = atoi(listOfArgValue[5].c_str());
		////////////////////////////////////////////////
		//SNR LIMITS
		////////////////////////////////////////////////
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_high = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_low = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_high = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_low = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_high = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_low = atoi(listOfArgValue[13].c_str());
		if (0 != listOfArgValue[14].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_high = atoi(listOfArgValue[14].c_str());
		if (0 != listOfArgValue[15].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_low = atoi(listOfArgValue[15].c_str());
		if (0 != listOfArgValue[16].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high = atoi(listOfArgValue[16].c_str());
		if (0 != listOfArgValue[17].length())
			_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low = atoi(listOfArgValue[17].c_str());
	}

	//WakeOnFinger
	_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_DutTestInfo->_wofInfo.m_bExecutedWithStimulus = false;
	_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus = 0;
	_pSyn_DutTestInfo->_wofInfo.m_nDelta_100 = 7;
	_pSyn_DutTestInfo->_wofInfo.m_nMinTriggerThreshold = 25;
	_pSyn_DutTestInfo->_wofInfo.m_nMaxTriggerThreshold = 60;
	_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes = 2000;
	_pSyn_DutTestInfo->_wofInfo.m_nNumNoiseSamples = 10;
	_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetHigh = 3;
	_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetLow = 3;
	_pSyn_DutTestInfo->_wofInfo.m_nStdDevThreshold = (float)0.9;
	_pSyn_DutTestInfo->_wofInfo.m_nDelta_200 = 20;
	_pSyn_DutTestInfo->_wofInfo.m_nDelta_200_3p7 = 13;
	_pSyn_DutTestInfo->_wofInfo.m_nVCC = (float)3.7;
	if (sysConfig.GetSyn_TestStepInfo(std::string("WakeOnFinger"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 18)
		{
			for (size_t t = 1; t <= 18 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_wofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_wofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_pSyn_DutTestInfo->_wofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
		if (0 != listOfArgValue[9].length())
			_pSyn_DutTestInfo->_wofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_pSyn_DutTestInfo->_wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_pSyn_DutTestInfo->_wofInfo.m_nVCC = stof(listOfArgValue[11]);
	}


	//SCM_WOFWithoutStimulus

	//SCM_WOFWithStimulus


	//WoVarTest
	_pSyn_DutTestInfo->_woVarInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_woVarInfo.m_nNumResBytes = 1000;
	_pSyn_DutTestInfo->_woVarInfo.m_nDelay_ms = 500;
	if (sysConfig.GetSyn_TestStepInfo(std::string("WoVarTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_woVarInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_woVarInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//DRdyTest
	_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_DRdyInfo.m_portId = 1;
	_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = 0x10;
	if (sysConfig.GetSyn_TestStepInfo(std::string("DRdyTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_DRdyInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
	}

	//UsbCommTest
	_pSyn_DutTestInfo->_usbCommInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedVid = 0x0000;
	_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedPid = 0x0000;
	_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedMaximRev = 0x13;
	_pSyn_DutTestInfo->_usbCommInfo.m_nSlaveSelPin = 17;
	_pSyn_DutTestInfo->_usbCommInfo.m_nMisoPin = 19;
	_pSyn_DutTestInfo->_usbCommInfo.m_nMosiPin = 21;
	_pSyn_DutTestInfo->_usbCommInfo.m_nSclkPin = 23;
	if (sysConfig.GetSyn_TestStepInfo(std::string("UsbCommTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 7)
		{
			for (size_t t = 1; t <= 7 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedVid = strtol(listOfArgValue[0].c_str(), NULL, 0);
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedPid = strtol(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nExpectedMaximRev = strtol(listOfArgValue[2].c_str(), NULL, 0);
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nSlaveSelPin = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nMisoPin = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nMosiPin = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_usbCommInfo.m_nSclkPin = atoi(listOfArgValue[6].c_str());
	}

	//SpiFlashRam
	_pSyn_DutTestInfo->_spiFlashInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_spiFlashInfo.m_nNumResBytes = 1024;
	/*if (_SysConfig.GetSyn_TestStepInfo(std::string("SpiFlashRam"), strArgsValue))
	{
	ParseTestStepArgs(strArgsValue, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize >= 1)
	{
	_pSyn_DutTestInfo->m_spiFlashInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
	}
	}*/

	//ButtonTestWithoutStimulus
	_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId = 8;
	_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk = 0x10;
	_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_expectedState = 1;
	if (sysConfig.GetSyn_TestStepInfo(std::string("ButtonTestWithoutStimulus"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_expectedState = atoi(listOfArgValue[2].c_str());
	}

	//ButtonTestWithStimulus
	_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId = 8;
	_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk = 0x10;
	_pSyn_DutTestInfo->_btnTestWithStimInfo.m_expectedState = 0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("ButtonTestWithStimulus"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_btnTestWithStimInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_btnTestWithStimInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_btnTestWithStimInfo.m_expectedState = atoi(listOfArgValue[2].c_str());
	}

	//SharpnessTest
	_pSyn_DutTestInfo->_SharpnessInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SharpnessInfo.limit = 30;
	if (sysConfig.GetSyn_TestStepInfo(std::string("SharpnessTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize >= 1)
		{
			_pSyn_DutTestInfo->_SharpnessInfo.limit = atoi(listOfArgValue[0].c_str());
		}
	}

	//ReadDutAdc
	_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted = false;

	//Imperfections
	_pSyn_DutTestInfo->_imperfectionsTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_imperfectionsTestInfo.m_peggedThreshold = 138;
	_pSyn_DutTestInfo->_imperfectionsTestInfo.m_flooredThreshold = 118;
	_pSyn_DutTestInfo->_imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = 10;
	_pSyn_DutTestInfo->_imperfectionsTestInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus;
	if (sysConfig.GetSyn_TestStepInfo(std::string("Imperfections"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_imperfectionsTestInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_imperfectionsTestInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
	}

	//RxStandardDev
	_pSyn_DutTestInfo->_RxStandardDevInfo.m_bExecuted = false;
	//_pSyn_DutTestInfo->m_RxStandardDevInfo.numFrames = 30;
	_pSyn_DutTestInfo->_RxStandardDevInfo.limit = 80;
	_pSyn_DutTestInfo->_RxStandardDevInfo.numFrames = _pSyn_DutTestInfo->_acquireFpsInfo.m_nNumImagesWithStimulus;
	if (sysConfig.GetSyn_TestStepInfo(std::string("RxStandardDev"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize >= 1)
		{
			_pSyn_DutTestInfo->_RxStandardDevInfo.limit = atoi(listOfArgValue[0].c_str());
		}
	}

	//OscTrim
	_pSyn_DutTestInfo->_OscTrimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_OscTrimInfo.nLowerLimit_Hz = 122;
	_pSyn_DutTestInfo->_OscTrimInfo.nUpperLimit_Hz = 123;
	_pSyn_DutTestInfo->_OscTrimInfo.nInitialTrim = 0x380;
	_pSyn_DutTestInfo->_OscTrimInfo.m_OscTrimDefault = 0x0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("OscTrim"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_OscTrimInfo.nLowerLimit_Hz = strtoul(listOfArgValue[0].c_str(), NULL, 0);
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_OscTrimInfo.nUpperLimit_Hz = strtoul(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_OscTrimInfo.nInitialTrim = strtoul(listOfArgValue[2].c_str(), NULL, 0);
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_OscTrimInfo.m_OscTrimDefault = strtoul(listOfArgValue[3].c_str(), NULL, 0);
	}

	//SlowOsc
	_pSyn_DutTestInfo->_SlowOscInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SlowOscInfo.nLowerLimit_Hz = 475000;
	_pSyn_DutTestInfo->_SlowOscInfo.nUpperLimit_Hz = 501000;
	_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultTrim = 0x0;
	_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultBias = 0x0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("SlowOsc"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_SlowOscInfo.nLowerLimit_Hz = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_SlowOscInfo.nUpperLimit_Hz = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultTrim = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_SlowOscInfo.m_nDefaultBias = atoi(listOfArgValue[3].c_str());
	}

	//RAMTest
	_pSyn_DutTestInfo->_RAMTestInfo.m_bExecuted = false;
	//Set defaults.

	//RetainModeCurrentTest
	_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_retainModeInfo.m_nGain = 2;
	_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = 1;
	_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = (float)0.1;
	_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = 0;
	if (sysConfig.GetSyn_TestStepInfo(std::string("RetainModeCurrentTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_retainModeInfo.m_nGain = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_retainModeInfo.m_nMaxCurrent = stof(listOfArgValue[1]);
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_retainModeInfo.m_nMinCurrent = stof(listOfArgValue[2]);
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_retainModeInfo.m_nDelay = atoi(listOfArgValue[3].c_str());
	}

	//WOF-LowPower
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain = 2;
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA = 50;
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA = 0;
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms = 200;
	if (sysConfig.GetSyn_TestStepInfo(std::string("WOF-LowPower"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_wofLowPowerInfo.m_nGain = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMaxCurrent_uA = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_wofLowPowerInfo.m_nMinCurrent_uA = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_wofLowPowerInfo.m_nDelay_ms = atoi(listOfArgValue[3].c_str());
	}

	//AdcBaseLineInfo????????????????????????????????????????

	//Security
	_pSyn_DutTestInfo->_securityStepInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_securityStepInfo.m_bEngineering = 0;
	_pSyn_DutTestInfo->_securityStepInfo.m_bProduction = 0;
	_pSyn_DutTestInfo->_securityStepInfo.m_bUsbMode = 0;
	_pSyn_DutTestInfo->_securityStepInfo.m_bSpiOwnership = 0;
	_pSyn_DutTestInfo->_securityStepInfo.m_nNumSpiOwnershipRuns = 5;
	if (sysConfig.GetSyn_TestStepInfo(std::string("Security"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 5)
		{
			for (size_t t = 1; t <= 5 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_securityStepInfo.m_bEngineering = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_securityStepInfo.m_bProduction = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_securityStepInfo.m_bUsbMode = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_securityStepInfo.m_bSpiOwnership = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_securityStepInfo.m_nNumSpiOwnershipRuns = atoi(listOfArgValue[4].c_str());
	}

	//ProductIdTest
	_pSyn_DutTestInfo->_productIdTestInfo.m_bExecuted = false;

	//AFETest
	_pSyn_DutTestInfo->_AFETestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_AFETestInfo.m_nNumResBytes = 2056;
	_pSyn_DutTestInfo->_AFETestInfo.m_nDelay_ms = 500;
	if (sysConfig.GetSyn_TestStepInfo(std::string("AFETest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_AFETestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_AFETestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//LEDTest
	_pSyn_DutTestInfo->_LEDTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO1 = 0x02;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO2 = 0x04;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO3 = 0x08;
	memset(_pSyn_DutTestInfo->_LEDTestInfo.m_sUserMsg, NULL, MAX_LED_TEST_MSG_LEN);
	_pSyn_DutTestInfo->_LEDTestInfo.m_nMpcGpioPin = 22;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nRegisterAddress = 0x80001600;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nPokeValue = 0x04;
	_pSyn_DutTestInfo->_LEDTestInfo.m_nTimeout = 5000;
	if (sysConfig.GetSyn_TestStepInfo(std::string("LEDTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 8)
		{
			for (size_t t = 1; t <= 8 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO1 = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO2 = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nGPIO3 = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			memcpy(_pSyn_DutTestInfo->_LEDTestInfo.m_sUserMsg, listOfArgValue[3].c_str(), listOfArgValue[3].length());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nMpcGpioPin = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nRegisterAddress = strtoul(listOfArgValue[5].c_str(), NULL, 0);
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nPokeValue = strtoul(listOfArgValue[6].c_str(), NULL, 0);
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_LEDTestInfo.m_nTimeout = atoi(listOfArgValue[7].c_str());
	}

	//SDKBaselineTest
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimTop = 0;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimBottom = 0;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimLeft = 0;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimRight = 0;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nRightShiftBits = 3;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nStrideWidth = 3;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nMaxSumAllowed = 250;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_nPercentFail = 25;
	if (sysConfig.GetSyn_TestStepInfo(std::string("SDKBaselineTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 8)
		{
			for (size_t t = 1; t <= 8 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimTop = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimBottom = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[2].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimLeft = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[3].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nTrimRight = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[4].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nRightShiftBits = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[5].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nStrideWidth = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[6].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nMaxSumAllowed = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[7].length())
			_pSyn_DutTestInfo->_SdkBaselineInfo.m_nPercentFail = atoi(listOfArgValue[0].c_str());
	}
}

bool Syn_Dut::ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol)
{
	olistOfArgValue.clear();

	unsigned int iLength = strArgsValue.length();
	if (0 == iLength)
	{
		return false;
	}

	std::string strTempValue = strArgsValue;
	while (true)
	{
		size_t SymbolPosition = strTempValue.find(strSymbol);
		if (SymbolPosition == std::string::npos)
		{
			if (0 != strTempValue.length())
			{
				olistOfArgValue.push_back(strTempValue);
			}
			break;
		}

		std::string strArgValue = strTempValue.substr(0, SymbolPosition);
		olistOfArgValue.push_back(strArgValue);
		strTempValue = strTempValue.substr(SymbolPosition + 1, strTempValue.length() - SymbolPosition);
		//strTempValue = strTempValue.substr(SymbolPosition + strSymbol.length(), strTempValue.length() - SymbolPosition-strSymbol.length());
	}

	return true;
}

bool Syn_Dut::FindPatch(std::string patchName, Syn_PatchInfo &patchInfo)
{
	bool IsExists(false);
	for (size_t i = 1; i <= _listOfPatchInfo.size(); i++)
	{
		if (patchName == _listOfPatchInfo[i - 1]._strXepatchName)
		{
			patchInfo = _listOfPatchInfo[i - 1];
			IsExists = true;
			break;
		}
	}
	return IsExists;
}