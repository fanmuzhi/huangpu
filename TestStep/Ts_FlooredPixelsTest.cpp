#include "Ts_FlooredPixelsTest.h"

Ts_FlooredPixelsTest::Ts_FlooredPixelsTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_FlooredPixelsTest::~Ts_FlooredPixelsTest()
{
}

void Ts_FlooredPixelsTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.numFrames = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_row = 9;
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_col = 28;
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_limits = 25;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
}

void Ts_FlooredPixelsTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.m_bExecuted = true;

}

void Ts_FlooredPixelsTest::ProcessData()
{
	int nTrimLeft = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int nTrimRight = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int nTrimTop = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int nTrimBottom = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int i, j, k, temp = 0;
	int numFrames, failRow, failCol, countRow, countCol, failLimit, nFailCount; //these are filled out by structure from .cpp file.
	int temp_sum = 0;
	int temp_val = 0;

	countCol = 0;
	countRow = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_bPass = 1;//set the device to pass to begin with.

	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.numFrames;
	failCol = _pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_col;
	failRow = _pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_row;
	failLimit = _pSyn_Dut->_pSyn_DutTestInfo->_flooredPixelsInfo.fail_limits;
	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
				temp_sum += (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus[k]).arr[i][j];

			temp_val = temp_sum / numFrames;
			_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.avg[i][j] = temp_val;

			temp_sum = 0;
			temp_val = 0;
		}
	}

	//scan through each row.
	_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_ROW = 0;
	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	{
		nFailCount = 0;
		for (j = HEADER + nTrimLeft; j<numCols - (HEADER + nTrimLeft + nTrimRight); j++)
		{
			if (_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.avg[i][j] < failLimit)
				nFailCount += 1;

			if (nFailCount > failRow)
				_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_bPass = 0;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_pixel_rows[i - nTrimTop] = nFailCount;
		if (nFailCount > _pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_ROW)
			_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_ROW = nFailCount;
	}

	//scan through each col.
	_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_COL = 0;
	for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	{
		nFailCount = 0;
		for (i = nTrimTop; i<numRows - (nTrimTop + nTrimBottom); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.avg[i][j] < failLimit)
				nFailCount++;

			if (temp > failCol)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_bPass = 0;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_pixel_cols[j - (HEADER + nTrimLeft)] = nFailCount;
		if (nFailCount > _pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_COL)
			_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_COL = nFailCount;
	}

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sFlooredPixFail);
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_elapsedtime);

}

void Ts_FlooredPixelsTest::CleanUp()
{
}