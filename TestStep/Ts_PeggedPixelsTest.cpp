#include "Ts_PeggedPixelsTest.h"

Ts_PeggedPixelsTest::Ts_PeggedPixelsTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_PeggedPixelsTest::~Ts_PeggedPixelsTest()
{
}

void Ts_PeggedPixelsTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row = 9;
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col = 28;
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits = 255;
	ParseTestStepArgs(_strArgs, listOfArgValue);

	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;//atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
}


void Ts_PeggedPixelsTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.m_bExecuted = true;

}

void Ts_PeggedPixelsTest::ProcessData()
{
	int nTrimLeft = 0;
	int nTrimRight = 0;
	int nTrimTop = 0;
	int nTrimBottom = 0; 
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int i, j, k, temp = 0;
	int numFrames, failRow, failCol, countRow, countCol, failLimit; //these are filled out by structure from .cpp file.
	int temp_sum = 0;
	int temp_val = 0;

	int temp_pegged[MAXCOL] = { 0 };

	countCol = 0;
	countRow = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 1;//set the device to pass to begin with.

	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames;
	failCol = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col;
	failRow = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row;
	failLimit = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits;//225
	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames; k++)
			{
				temp_sum += (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus)[k].arr[i][j];
			}

			temp_val = temp_sum / numFrames;
			(_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] = temp_val;

			temp_sum = 0;
			temp_val = 0;
		}
	}
	//scan through each row.
	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	{
		for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] >= failLimit)
			{
				countRow += 1;
				temp += 1;
			}
			if (temp > failRow)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 0;
		}

		temp_pegged[i - nTrimTop] = countRow;
		countRow = 0;
		(_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_rows)[i - nTrimTop] = temp;
		temp = 0;
	}

	//assign number of pegged 
	_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_ROW = max_array(temp_pegged, numCols);

	//initialize temp_pegged;
	for (i = 0; i<numCols; i++)
		temp_pegged[i] = 0;
	//scan through each col.
	for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	{
		for (i = nTrimTop; i<numRows - nTrimBottom; i++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] >= failLimit)
			{
				countCol += 1;
				temp += 1;
			}
			if (temp > failCol)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 0;
		}

		temp_pegged[j - nTrimLeft - HEADER] = countCol;
		countCol = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_cols[j - nTrimLeft - HEADER] = temp;
		temp = 0;
	}
	//assign number of pegged
	_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_COL = max_array(temp_pegged, numCols);

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sPeggedPixFail);
	}

}

void Ts_PeggedPixelsTest::CleanUp()
{
}