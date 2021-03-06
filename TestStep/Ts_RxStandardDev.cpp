#include "Ts_RxStandardDev.h"

Ts_RxStandardDev::Ts_RxStandardDev(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_RxStandardDev::~Ts_RxStandardDev()
{
}

void Ts_RxStandardDev::SetUp()
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
	
	_pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.m_bExecuted = false;
	//_pSyn_Dut->_pSyn_DutTestInfo->m_RxStandardDevInfo.numFrames = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.limit = 80;
	_pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgFingerInfo.m_nNumImagesWithStimulus;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize >= 1)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.limit = atoi(listOfArgValue[0].c_str());
	}
}


void Ts_RxStandardDev::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.m_bExecuted = true;
}

void Ts_RxStandardDev::ProcessData()
{
	int nTrimLeft = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int nTrimRight = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int nTrimTop = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int nTrimBottom = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int row, temp_sum, i, j, k;
	float mean, temp_val;
	int numFrames;
	float fingerdata[MAXROW][MAXCOL] = { 0 };

	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.numFrames;
	_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.m_bPass = 1;
	_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.max_percent = 0;

	//set up the finger avg matrix that is obtained by taking the arithmetic mean of 30 frames of finger data.
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp_sum += (_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger[k]).arr[i][j];
			}

			temp_val = (float)(temp_sum / numFrames);
			fingerdata[i][j] = temp_val;

			temp_sum = 0;
			temp_val = 0;
		}
	}

	for (row = nTrimTop; row<numRows - nTrimBottom; row++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.Rx_STDs[row] = std_dev(&fingerdata[row][nTrimLeft + HEADER], numCols - HEADER - nTrimRight - nTrimLeft);
	}

	mean = find_mean(&(_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.Rx_STDs[nTrimTop]), numRows - (nTrimTop + nTrimBottom));

	for (row = nTrimTop; row<numRows - nTrimBottom; row++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.percent[row - nTrimTop] = (float)abs((int)(((_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.Rx_STDs[row] - mean) / (mean)) * 100));
		if (_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.percent[row - nTrimTop] > _pSyn_Dut->_pSyn_DutTestInfo->_RxStandardDevInfo.limit)
			_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.m_bPass = 0;

		// SS ask to present max percent data in the log
		_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.max_percent = (_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.percent[row - nTrimTop] > _pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.max_percent) ? _pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.percent[row - nTrimTop] : _pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.max_percent;
	}

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sStdDevOfRxFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RxStandardDev", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("RxStandardDev", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.m_elapsedtime);

}

void Ts_RxStandardDev::CleanUp()
{
}

float Ts_RxStandardDev::find_mean(float* arr, int S_COL)
{
	int i;
	float tempsum = 0.0;
	for (i = 0; i<S_COL; i++)
	{
		tempsum += arr[i];
	}

	return tempsum / S_COL;
}

float Ts_RxStandardDev::std_dev(float pData[], int size)
{
	float mean = 0.0, sum_deviation = 0.0;
	int i;
	for (i = 0; i<size; ++i)
	{
		mean += pData[i];
	}
	mean = mean / size;
	for (i = 0; i<size; ++i)
		sum_deviation += (pData[i] - mean)*(pData[i] - mean);


	return (float)sqrt(sum_deviation / size);
}