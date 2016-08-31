#include "Ts_HuaweiImageQualityTest.h"

Ts_HuaweiImageQualityTest::Ts_HuaweiImageQualityTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_HuaweiImageQualityTest::~Ts_HuaweiImageQualityTest()
{
}

void Ts_HuaweiImageQualityTest::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._highLimit = 100;
	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._lowLimit = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._numSamples = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fCrop = 0.2;
	//_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fGrav = 0.2;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._highLimit = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._lowLimit = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._numSamples = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fCrop = std::stof(listOfArgValue[3].c_str());
	/*if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fGrav = std::stof(listOfArgValue[4].c_str());*/
}

void Ts_HuaweiImageQualityTest::Execute()
{
	Syn_Exception ex(0);

	int numMaxColsWithStim(0), numMinColsWithStim(0), numMaxRowsWithStim(0), numMinRowsWithStim(0);

	int iRowNumber = _pSyn_Dut->_RowNumber;
	int iColumnNumber = _pSyn_Dut->_ColumnNumber;
	numMaxColsWithStim = iColumnNumber - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim;
	numMinColsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim;
	numMaxRowsWithStim = iRowNumber - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim;;
	numMinRowsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim;

	uint16_t _uiNumRows = numMaxRowsWithStim - numMinRowsWithStim;
	uint16_t _uiNumCols = numMaxColsWithStim - numMinColsWithStim - HEADER;

	int NumberOfIamge(_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._numSamples);

	uint8_t *arrImageNoFinger = new uint8_t[_uiNumRows * _uiNumCols * NumberOfIamge];
	int k = 0;
	for (int i = 0; i < NumberOfIamge; i++)
	{
		for (int m = numMinRowsWithStim; m < numMaxRowsWithStim; m++)
		{
			for (int n = (numMinColsWithStim + HEADER); n < numMaxColsWithStim; n++)
			{
				arrImageNoFinger[k] = (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult).arr_nofinger[i].arr[m][n];
				k++;
			}
		}
	}

	uint8_t *arrImageFinger = new uint8_t[_uiNumRows * _uiNumCols * NumberOfIamge];
	k = 0;
	for (int i = 0; i < NumberOfIamge; i++)
	{
		//RemoveBaseline(
		//	&_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger[i],
		//	&_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_ImageFPSFrame,
		//	_uiNumRows,
		//	_uiNumCols
		//	);
		for (int m = numMinRowsWithStim; m < numMaxRowsWithStim; m++)
		{
			for (int n = (numMinColsWithStim + HEADER); n < numMaxColsWithStim; n++)
			{
				arrImageFinger[k] = (_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult).arr_finger[i].arr[m][n];
				k++;
			}
		}
	}

	//SNR
	float snrValue(0);
	int singalvalue(0);
	float noisevalue(0);
	try
	{
		snrValue = testSNR(arrImageNoFinger, arrImageFinger, _uiNumCols, _uiNumRows, NumberOfIamge, &singalvalue, &noisevalue, _pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fCrop);
	}
	catch (...)
	{
		ex.SetError(Syn_ExceptionCode::Syn_SystemError);
		ex.SetDescription("testSNR is failed!");
		throw ex;
		return;
	}

	//SNRUG
	/*float snrUGValue(0);
	try
	{
		snrUGValue = testSNRUG(arrImageNoFinger, arrImageFinger, _uiNumCols, _uiNumRows, NumberOfIamge);
	}
	catch (...)
	{
		ex.SetError(Syn_ExceptionCode::Syn_SystemError);
		ex.SetDescription("testSNRUG is failed!");
		throw ex;
		return;
	}

	//calchuaidian
	int calchuaidianValue(0);
	int num_of_bubble(0);
	try
	{
		uint8_t *ImageFinger = new uint8_t[_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealColumnNumber * _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealRowNumber];
		int t = 0;
		for (int m = 0; m < _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealRowNumber; m++)
		{
			for (int n = 0; n < _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealColumnNumber; n++)
			{
				ImageFinger[t] = _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_ImageFPSFrame.arr[m][n];
				t++;
			}
		}

		calchuaidianValue = calchuaidian(ImageFinger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealColumnNumber, _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealRowNumber, &num_of_bubble, _pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._fGrav);

		delete[] ImageFinger;
		ImageFinger = NULL;
	}
	catch (...)
	{
		ex.SetError(Syn_ExceptionCode::Syn_SystemError);
		ex.SetDescription("calchuaidian is failed!");
		throw ex;
		return;
	}*/

	_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.snr = snrValue;
	_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.singalValue = singalvalue;
	_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.nosieValue = noisevalue;
	//_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.snrUG = snrUGValue;
	//_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.calchuaidian = calchuaidianValue;

	delete[] arrImageNoFinger;
	arrImageNoFinger = NULL;

	delete[] arrImageFinger;
	arrImageFinger = NULL;

	//std::cout << "snrValue is " << snrValue << std::endl;
	_pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._bExecuted = true;
}

void Ts_HuaweiImageQualityTest::ProcessData()
{
	int bPass = 1;
	int highLimit = _pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._highLimit;
	int lowLimit = _pSyn_Dut->_pSyn_DutTestInfo->_huaweiIqTestInfo._lowLimit;
	int snr = _pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.snr;
	//int snrUG = _pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.snrUG;
	//int calchuaidian = _pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.calchuaidian;

	if (highLimit < snr || snr < lowLimit)
	{
		bPass = 0;
	}
	/*if (highLimit < snrUG || snrUG < lowLimit)
	{
		bPass = 0;
	}
	if (0 != calchuaidian)
	{
		bPass = 0;
	}*/

	if (bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults._bPass = true;
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("HuaweiImageQualityTest", "Pass"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults._bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sSnrFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("HuaweiImageQualityTest", "Fail"));
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_huaweiIqTestResults.m_elapsedtime);
}

void Ts_HuaweiImageQualityTest::CleanUp()
{
}