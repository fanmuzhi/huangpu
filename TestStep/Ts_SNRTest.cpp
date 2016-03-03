#include "Ts_SNRTest.h"

Ts_SNRTest::Ts_SNRTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_SNRTest::~Ts_SNRTest()
{
}

void Ts_SNRTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.m_bExecuted = false;

	int numMaxColsWithStim(0), numMinColsWithStim(0), numMaxRowsWithStim(0), numMinRowsWithStim(0);
	int numMaxColsWOStim(0), numMinColsWOStim(0), numMaxRowsWOStim(0), numMinRowsWOStim(0);

	uint16_t _uiNumRows = _pSyn_Dut->_RowNumber;
	uint16_t _uiNumCols = _pSyn_Dut->_ColumnNumber;
	numMaxColsWithStim = _uiNumCols - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim;
	numMinColsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim;
	numMaxRowsWithStim = _uiNumRows - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim;;
	numMinRowsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim;
	numMaxColsWOStim = _uiNumCols - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	numMinColsWOStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	numMaxRowsWOStim = _uiNumRows - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	numMinRowsWOStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;

	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numcols = _uiNumCols;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numrows = _uiNumRows;
	//Set defaults.
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numFrames = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols = 2;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols = 147;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows = 55;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.percent = 10;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_high = 500;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_low = 25;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_high = 500;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_low = 80;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high = 5;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_high = 500;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_low = 14;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_high = 500;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_low = 50;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high = 6;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low = 0;

	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols = (numMaxColsWOStim < numMaxColsWithStim) ? numMaxColsWOStim : numMaxColsWithStim;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols = (numMinColsWOStim < numMinColsWithStim) ? numMinColsWithStim : numMinColsWOStim;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows = (numMaxRowsWOStim < numMaxRowsWithStim) ? numMaxRowsWOStim : numMaxRowsWithStim;
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows = (numMinRowsWOStim < numMinRowsWithStim) ? numMinRowsWithStim : numMinRowsWOStim;

	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgFingerInfo.m_nNumImagesWithStimulus;//Image Count
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 18)
	{
		for (size_t t = 1; t <= 18 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.percent = atoi(listOfArgValue[5].c_str());
	////////////////////////////////////////////////
	//SNR LIMITS
	////////////////////////////////////////////////
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_high = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_low = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_high = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_low = atoi(listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low = atoi(listOfArgValue[11].c_str());
	if (0 != listOfArgValue[12].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_high = atoi(listOfArgValue[12].c_str());
	if (0 != listOfArgValue[13].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_low = atoi(listOfArgValue[13].c_str());
	if (0 != listOfArgValue[14].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_high = atoi(listOfArgValue[14].c_str());
	if (0 != listOfArgValue[15].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_low = atoi(listOfArgValue[15].c_str());
	if (0 != listOfArgValue[16].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high = atoi(listOfArgValue[16].c_str());
	if (0 != listOfArgValue[17].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low = atoi(listOfArgValue[17].c_str());
}


void Ts_SNRTest::Execute()
{

}

void Ts_SNRTest::ProcessData()
{
	//int nTrimLeft = 0;
	//int nTrimRight = 0;
	//int nTrimTop = 0;
	//int nTrimBottom = 0;
	//int numRows = _pSyn_Dut->_RowNumber;
	//int numCols = _pSyn_Dut->_ColumnNumber;

	//int i, j, k, temp = 0;
	//int numFrames, failRow, failCol, countRow, countCol, failLimit; //these are filled out by structure from .cpp file.
	//int temp_sum = 0;
	//int temp_val = 0;

	//int temp_pegged[MAXCOL] = { 0 };

	//countCol = 0;
	//countRow = 0;

	//_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 1;//set the device to pass to begin with.

	//numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames;
	//failCol = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_col;
	//failRow = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_row;
	//failLimit = _pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.fail_limits;//225
	////set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	//for (i = 0; i<numRows; i++)
	//{
	//	for (j = 0; j<numCols; j++)
	//	{
	//		for (k = 0; k<_pSyn_Dut->_pSyn_DutTestInfo->_peggedPixelsInfo.numFrames; k++)
	//		{
	//			temp_sum += (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus)[k].arr[i][j];
	//		}

	//		temp_val = temp_sum / numFrames;
	//		(_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] = temp_val;

	//		temp_sum = 0;
	//		temp_val = 0;
	//	}
	//}
	////scan through each row.
	//for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	//{
	//	for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	//	{
	//		if ((_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] >= failLimit)
	//		{
	//			countRow += 1;
	//			temp += 1;
	//		}
	//		if (temp > failRow)//fail the test if the count is greater than our expected limits.
	//			_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 0;
	//	}

	//	temp_pegged[i - nTrimTop] = countRow;
	//	countRow = 0;
	//	(_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_rows)[i - nTrimTop] = temp;
	//	temp = 0;
	//}

	////assign number of pegged 
	//_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_ROW = max_array(temp_pegged, numCols);

	////initialize temp_pegged;
	//for (i = 0; i<numCols; i++)
	//	temp_pegged[i] = 0;
	////scan through each col.
	//for (j = HEADER + nTrimLeft; j<numCols - nTrimRight; j++)
	//{
	//	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	//	{
	//		if ((_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.avg)[i][j] >= failLimit)
	//		{
	//			countCol += 1;
	//			temp += 1;
	//		}
	//		if (temp > failCol)//fail the test if the count is greater than our expected limits.
	//			_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 0;
	//	}

	//	temp_pegged[j - nTrimLeft - HEADER] = countCol;
	//	countCol = 0;
	//	_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_cols[j - nTrimLeft - HEADER] = temp;
	//	temp = 0;
	//}
	////assign number of pegged
	//_pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_COL = max_array(temp_pegged, numCols);

}

void Ts_SNRTest::CleanUp()
{
	PowerOff();
}