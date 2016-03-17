#include "Ts_ConsecutivePixels.h"

Ts_ConsecutivePixels::Ts_ConsecutivePixels(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_ConsecutivePixels::~Ts_ConsecutivePixels()
{
}

void Ts_ConsecutivePixels::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_peggedThreshold = 225;
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_flooredThreshold = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = 4;
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_cutEdge = 7;
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.numFrames = 30;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();//ilistSize is 5??????????????
	if (ilistSize < 4)
	{
		for (size_t t = 1; t <= 4 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_cutEdge = atoi(listOfArgValue[3].c_str());

	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;
}


void Ts_ConsecutivePixels::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_bExecuted = true;

}

void Ts_ConsecutivePixels::ProcessData()
{
	int nTrimLeft = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int nTrimRight = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int nTrimTop = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int nTrimBottom = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int numFrames, i, j, k, temp = 0;
	int failRowPegged, failRowFloored, countRow, countCol, failLimit; //these are filled out by structure from .cpp file.
	int temp_sum = 0;
	int temp_val = 0;
	int temp_consecutive[MAXCOL];
	int temp_col[MAXCOL];
	int temp_row[MAXROW];
	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.numFrames;

	//initialize temp_consecutive
	for (i = 0; i<numCols; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<numCols; i++)
		temp_col[i] = 0;
	for (i = 0; i<numRows; i++)
		temp_row[i] = 0;

	countRow = 0;
	countCol = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 1;//set the device to pass to begin with.

	//eliminate=info->m_cutEdge;

	failRowPegged = _pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_peggedThreshold;
	failRowFloored = _pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_flooredThreshold;
	failLimit = _pSyn_Dut->_pSyn_DutTestInfo->_consecutivePixelsInfo.m_maxAdjacentPixelsAllowed;//4

	//initialize the structs.
	for (i = 0; i<numCols - HEADER - (nTrimLeft + nTrimRight); i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_cols[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_cols[i] = 0;
	}
	for (i = 0; i<numRows - (nTrimTop + nTrimBottom); i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_rows[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_rows[i] = 0;
	}

	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp_sum += _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus[k].arr[i][j];
			}

			temp_val = temp_sum / numFrames;
			_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.avg[i][j] = temp_val;

			temp_sum = 0;
			temp_val = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	temp = 0;
	//scan through each row.
	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	{
		for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.avg[i][j] >= failRowPegged))
			{
				temp++;
				temp_consecutive[j - HEADER - nTrimLeft] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[j - HEADER - nTrimLeft] = temp;
			}

			if (temp >= failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 0;
		}
		temp_row[i - nTrimTop] = max_array(&temp_consecutive[0], numCols - HEADER - (nTrimLeft + nTrimRight));

		temp = 0;
	}


	//fill struct for logging purposes
	for (i = 0; i<numRows - (nTrimTop + nTrimBottom); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_rows[i] = temp_row[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for (i = 0; i<numCols; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<numCols - HEADER; i++)
		temp_col[i] = 0;
	for (i = 0; i<numRows; i++)
		temp_row[i] = 0;
	temp = 0;
	//scan through each row.
	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	{
		for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.avg[i][j] <= failRowFloored))
			{
				temp++;
				temp_consecutive[j - HEADER - nTrimLeft] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[j - HEADER - nTrimLeft] = temp;
			}

			if (temp >= failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 0;
		}
		temp_row[i - nTrimTop] = max_array(&temp_consecutive[0], numCols - HEADER - (nTrimLeft + nTrimRight));
		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<numRows - (nTrimTop + nTrimBottom); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_rows[i] = temp_row[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for (i = 0; i<numCols; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<numCols - HEADER; i++)
		temp_col[i] = 0;
	for (i = 0; i<numRows; i++)
		temp_row[i] = 0;

	temp = 0;
	//scan through each col. floored cols
	for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	{
		for (i = nTrimTop; i<numRows - nTrimBottom; i++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.avg[i][j] <= failRowFloored))
			{
				temp++;
				temp_consecutive[i - nTrimTop] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[i - nTrimTop] = temp;
			}

			if (temp >= failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 0;
		}
		temp_col[j - HEADER - nTrimLeft] = max_array(&temp_consecutive[0], numRows - (nTrimTop + nTrimBottom));
		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<numCols - HEADER - (nTrimLeft + nTrimRight); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_cols[i] = temp_col[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for (i = 0; i<numCols; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<numCols - HEADER; i++)
		temp_col[i] = 0;
	for (i = 0; i<numRows; i++)
		temp_row[i] = 0;
	temp = 0;
	//scan through each col.
	for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	{
		for (i = nTrimTop; i<numRows - nTrimBottom; i++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.avg[i][j] >= failRowPegged))
			{
				temp++;
				temp_consecutive[i - nTrimTop] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[i - nTrimTop] = temp;
			}

			if (temp >= failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 0;
		}

		temp_col[j - HEADER - nTrimLeft] = max_array(&temp_consecutive[0], numRows - (nTrimTop + nTrimBottom));
		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<numCols - HEADER - (nTrimLeft + nTrimRight); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_cols[i] = temp_col[i];

	_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.pegged_ROW = max_array(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_rows, numRows - (nTrimTop + nTrimBottom));
	_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.floored_ROW = max_array(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_rows, numRows - (nTrimTop + nTrimBottom));
	_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.floored_COL = max_array(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_cols, numCols - HEADER - (nTrimLeft + nTrimRight));
	_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.pegged_COL = max_array(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_cols, numCols - HEADER - (nTrimLeft + nTrimRight));

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sConsecutivePeggedPixFail);
		//_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sConsecutiveFlooredPixFail);
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_elapsedtime);
}

void Ts_ConsecutivePixels::CleanUp()
{
}