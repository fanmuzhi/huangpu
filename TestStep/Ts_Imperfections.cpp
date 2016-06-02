#include "Ts_Imperfections.h"

Ts_Imperfections::Ts_Imperfections(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_Imperfections::~Ts_Imperfections()
{
}

void Ts_Imperfections::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_peggedThreshold = 138;
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_flooredThreshold = 118;
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgFingerInfo.m_nNumImagesWithStimulus;
	
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 3)
	{
		for (size_t t = 1; t <= 3 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
}


void Ts_Imperfections::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_bExecuted = true;

}

void Ts_Imperfections::ProcessData()
{
	int nTrimLeft = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int nTrimRight = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int nTrimTop = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int nTrimBottom = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int numFrames, i, j, temp = 0;
	int failRowPegged, failRowFloored, countRow, countCol, failLimit; //these are filled out by structure from .cpp file.
	int temp_sum = 0;
	int temp_val = 0;
	int temp_consecutive[MAXCOL] = { 0 };
	int temp_col[MAXCOL] = { 0 };
	int temp_row[MAXROW] = { 0 };
	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.numFrames;

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
	_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass = 1;//set the device to pass to begin with.

	//eliminate=info->m_cutEdge;

	failRowPegged = _pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_peggedThreshold;
	failRowFloored = _pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_flooredThreshold;
	failLimit = _pSyn_Dut->_pSyn_DutTestInfo->_imperfectionsTestInfo.m_maxAdjacentPixelsAllowed;

	//initialize the structs.
	for (i = 0; i<numCols - HEADER - (nTrimLeft + nTrimRight); i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_floored_cols[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_cols[i] = 0;
	}
	for (i = 0; i<numRows - (nTrimTop + nTrimBottom); i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_floored_rows[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_rows[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	temp = 0;
	//scan through each row.
	for (i = 0; i<numRows - nTrimBottom - nTrimTop; i++)
	{
		for (j = 0; j<numCols - nTrimRight - nTrimLeft - HEADER; j++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NORM_AVGS[6].arr[i][j] >= failRowFloored) && (_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NORM_AVGS[6].arr[i][j] <= failRowPegged))
			{
				temp++;
				temp_consecutive[j] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[j] = temp;
			}

			if (temp > failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass = 0;
		}
		temp_row[i] = max_array(&temp_consecutive[0], numCols - HEADER - (nTrimLeft + nTrimRight));

		temp = 0;
	}


	//fill struct for logging purposes
	for (i = 0; i<numRows - (nTrimTop + nTrimBottom); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_rows[i] = temp_row[i];
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
	for (j = 0; j<numCols - nTrimRight - nTrimLeft - HEADER; j++)
	{
		for (i = 0; i<numRows - nTrimBottom - nTrimTop; i++)
		{
			if ((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NORM_AVGS[6].arr[i][j] >= failRowFloored) && (_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NORM_AVGS[6].arr[i][j] <= failRowPegged))
			{
				temp++;
				temp_consecutive[i] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[i] = temp;
			}

			if (temp > failLimit)//fail the test if the count is greater than our expected limits.
				_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass = 0;
		}

		temp_col[j] = max_array(&temp_consecutive[0], numRows - (nTrimTop + nTrimBottom));
		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<numCols - HEADER - (nTrimLeft + nTrimRight); i++)
		_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_cols[i] = temp_col[i];

	_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.pegged_ROW = max_array(_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_rows, numRows - (nTrimTop + nTrimBottom));
	_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.pegged_COL = max_array(_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_cols, numCols - HEADER - (nTrimLeft + nTrimRight));

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sImperfectionsFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("Imperfections", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("Imperfections", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_elapsedtime);
}

void Ts_Imperfections::CleanUp()
{
}