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
	_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.m_bExecuted = true;
}

void Ts_SNRTest::ProcessData()
{
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;
	int bIsBga = 0;

	float NOISE = 0.0, SNR = 0.0;

	int minrow, mincol, maxcol, maxrow, SIGNAL = 0;//;, s_ROW,s_COL;
	int i, j, k;//for 9 regions
	int M, N; //no of elements in row and columns of the segment window.
	int C0, C1, C2, R0, R1;
	int C[4], R[3];

	int all_zones_passed = 1;
	int overall_passed = 1;

	//debug
	for (i = 0; i < REGIONS; i++)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[i] = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[i] = 0;
	}

	// Get min/max cols and min/max rows from SNRInfo struct
	N = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols - (_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols + HEADER);
	M = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows;

	//ZONE DIVISION STARTS HERE
	C0 = N / 3;
	C1 = C0 + C0;
	C2 = N;
	C[0] = 0;
	C[1] = C0;
	C[2] = C1;
	C[3] = C2;
	//
	R0 = M / 2;
	R1 = R0 + R0;
	R[0] = 0;
	R[1] = R0;
	R[2] = R1;

	SNR_Fill_Log(_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults), numRows, numCols, _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numFrames);

	k = 0;
	for (i = 0; i<2; i++)//row
	{
		for (j = 0; j<3; j++)//col
		{
			if (k == 0)
			{
				mincol = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols + C[0] + HEADER;
				maxcol = C[1] + HEADER;
				minrow = R[0];
				maxrow = R[1];
			}
			if (k == 1)
			{
				mincol = C[1] + HEADER;
				maxcol = C[2] + HEADER;
				minrow = R[0];
				maxrow = R[1];
			}
			if (k == 2)
			{
				mincol = C[2] + HEADER;
				maxcol = C[3] + HEADER;
				minrow = R[0];
				maxrow = R[1];
			}
			if (k == 3)
			{
				mincol = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols + HEADER + C[0];
				maxcol = C[1] + HEADER;
				minrow = R[1];
				maxrow = R[2];
			}
			if (k == 4)
			{
				mincol = C[1] + HEADER;
				maxcol = C[2] + HEADER;
				minrow = R[1];
				maxrow = R[2];
			}
			if (k == 5)
			{
				mincol = C[2] + HEADER;
				maxcol = C[3] + HEADER;
				minrow = R[1];
				maxrow = R[2];
			}

			SIGNAL = get_signal_value(_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, &(_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo), minrow, maxrow, mincol, maxcol, k, &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults));
			NOISE = get_noise_value(_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, &(_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo), minrow, maxrow, mincol, maxcol, k, &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults));//,s_ROW,s_COL);
			SNR = SIGNAL / NOISE;

			_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] = SIGNAL;
			_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] = NOISE;
			_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] = SNR;

			SIGNAL = 0;
			NOISE = 0.0;
			SNR = 0.0;
			k += 1;

			if (k == 6)//if the overall window is looked at.
			{
				minrow = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows;
				maxrow = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows;
				mincol = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols + HEADER;
				maxcol = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols;

				SIGNAL = get_signal_value(_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, &(_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo), minrow, maxrow, mincol, maxcol, k, &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults));
				NOISE = get_noise_value(_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_finger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, &(_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo), minrow, maxrow, mincol, maxcol, k, &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults));//,s_ROW,s_COL);
				SNR = SIGNAL / NOISE;

				_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] = SIGNAL;
				_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] = NOISE;
				_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] = SNR;
			}
		}

	}

	//region 6 is the full picture.
	for (k = 0; k<7; k++)
	{
		if (!bIsBga)
		{
			if (k != 6)//zones
			{
				if (!((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_low && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_signal_high) &&
					(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low  && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high) &&
					(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_low    && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_snr_high))
					)
					all_zones_passed = 0;
			}
			else //overall
			{
				if (!((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_low && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_signal_high) &&
					(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low  && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high) &&
					(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_low    && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_snr_high))
					)
					overall_passed = 0;
			}
		}
		else
		{
			if (k != 6)//zones
			{
				if (!((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_low  && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_zone_noise_high)))
					all_zones_passed = 0;
			}
			else //overall			
			{
				if (!((_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] >= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_low  && _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[k] <= _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.limit_overall_noise_high)))
					overall_passed = 0;
			}
		}
	}

	_pSyn_Dut->_pSyn_DutTestResult->_snrResults.OTPVal_SNR = _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[REGIONS-1];
	_pSyn_Dut->_pSyn_DutTestResult->_snrResults.OTPVal_Noise = _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[REGIONS-1];
	_pSyn_Dut->_pSyn_DutTestResult->_snrResults.OTPVal_Signal = _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[REGIONS-1];

	if (all_zones_passed && overall_passed)
		_pSyn_Dut->_pSyn_DutTestResult->_snrResults.bPass = 1;
	else
		_pSyn_Dut->_pSyn_DutTestResult->_snrResults.bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sSnrFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SNRTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SNRTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.m_elapsedtime);

	//remove baseline
	for (int i = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows; i<_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows; i++)
	{
		for (int j = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols; j<_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols - HEADER; j++)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.arr_ImageFPSFrame.arr[i - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows][j - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols] = 
				   (uint8_t)(_pSyn_Dut->_pSyn_DutTestResult->_snrResults.NORM_AVGS[6].arr[i - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows][j - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols]);
		}
	}

	_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealRowNumber = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxRows - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinRows;
	_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.iRealColumnNumber = _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMaxCols - _pSyn_Dut->_pSyn_DutTestInfo->_snrInfo.numMinCols - HEADER;
}

void Ts_SNRTest::CleanUp()
{
}

int Ts_SNRTest::get_signal_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol, int regions, SNRResults* results)//,int s_ROW, int s_COL)
{
	int p_percent, row, col, layer, i, S_ROW, S_COL, SIGNAL = 0, signalmin = 0, signalmax = 0, signalsum_min = 0, signalsum_max = 0;
	int temp_fing = 0, temp_nofing = 0;
	float mean = 0.0, tempsum_finger = 0.0, tempsum_nofinger = 0.0;
	float percent;


	int bins[HISTOGRAM] = { 0 };
	// float temp_mean[MAXC-MINC];
	float* pTemp_mean = (float*)malloc((info->numMaxCols - info->numMinCols)*sizeof(float));

	p_percent = info->percent;
	percent = (float)p_percent / 100;

	S_ROW = maxrow - minrow;
	S_COL = maxcol - mincol;

	//temp_mean =(float*) malloc(S_COL * sizeof(float *));

	for (row = minrow; row<maxrow; row++)
	{
		for (col = mincol; col<maxcol; col++)
		{
			for (layer = 0; layer<info->numFrames; layer++)
			{
				temp_fing += fingerdata[layer].arr[row][col];
			}
			tempsum_finger = (float)temp_fing / info->numFrames;
			//Truncate
			if (tempsum_finger > 255 || tempsum_finger < 0)
				tempsum_finger = tempsum_finger > 255 ? (float)255 : (float)0;

			for (layer = 0; layer<info->numFrames; layer++)
			{
				temp_nofing += nofingerdata[layer].arr[row][col];
			}
			tempsum_nofinger = (float)temp_nofing / info->numFrames;
			//Truncate
			if (tempsum_nofinger > 255 || tempsum_nofinger < 0)
				tempsum_nofinger = tempsum_nofinger > 255 ? (float)255 : (float)0;
			/*---BEGIN PIXEL_AVGS---*/
			results->PIXEL_AVGS[regions].arr[row - minrow][col - mincol] = tempsum_finger - tempsum_nofinger + 128;
			/*---ENDPIXEL_AVGS---*/
			pTemp_mean[col - mincol] = tempsum_finger - tempsum_nofinger + 128;//results->PIXEL_AVGS[regions].arr[row-minrow][col-mincol];//for calculating the norm_avgs
			//Truncate
			if (pTemp_mean[col - mincol] > 255 || pTemp_mean[col - mincol] < 0)
				pTemp_mean[col - mincol] = pTemp_mean[col - mincol] > 255 ? (float)255 : (float)0;

			tempsum_finger = 0.0;
			tempsum_nofinger = 0.0;
			temp_fing = 0;
			temp_nofing = 0;
		}

		for (i = 0; i<S_COL; i++)
		{
			results->NORM_AVGS[regions].arr[row - minrow][i] = results->PIXEL_AVGS[regions].arr[row - minrow][i] - find_mean(pTemp_mean, S_COL) + (float)128.00;
			//Truncate
			if (results->NORM_AVGS[regions].arr[row - minrow][i] > 255 || results->NORM_AVGS[regions].arr[row - minrow][i] < 0)
				results->NORM_AVGS[regions].arr[row - minrow][i] = results->NORM_AVGS[regions].arr[row - minrow][i] > 255 ? (float)255 : (float)0;
		}
	}

	find_bins(results, S_ROW, S_COL, bins, regions);


	signalmin = 0;
	signalsum_min = 0;
	for (i = 1; i<HISTOGRAM; i++)
	{
		if (signalsum_min == 0)
		{
			if (sum(bins, 0, i) >= 0.5*percent*sum(bins, 0, HISTOGRAM))
				signalmin = i;
			else
				signalmin = 0;
		}
		else
			signalmin = 0;

		signalsum_min += signalmin;
	}

	signalmax = 0;
	signalsum_max = 0;
	for (i = 1; i<HISTOGRAM; i++)
	{
		if (signalsum_max == 0)
		{
			if (sum(bins, 0, i) >= (1 - 0.5*percent)*sum(bins, 0, HISTOGRAM))
				signalmax = i;
			else
				signalmax = 0;
		}
		else
			signalmax = 0;

		signalsum_max += signalmax;

	}
	SIGNAL = signalsum_max - signalsum_min;

	free(pTemp_mean);
	pTemp_mean = NULL;

	return SIGNAL;

}

float Ts_SNRTest::get_noise_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol, int regions, SNRResults* results)//,int s_ROW, int s_COL)
{
	int row, col, layer, i, S_ROW, S_COL;
	float mean = 0.0, sum_deviation = 0.0, tempsum_nofinger = 0.0;


	short temp[MAXFRAMES] = { 0 };

	S_ROW = maxrow - minrow;
	S_COL = maxcol - mincol;

	for (row = minrow; row<maxrow; row++)
	{
		for (col = mincol; col<maxcol; col++)
		{
			for (layer = 0; layer<info->numFrames; layer++)
			{
				*(temp + layer) = (short)nofingerdata[layer].arr[row][col];
				mean += *(temp + layer);
			}

			tempsum_nofinger = mean / info->numFrames;
			mean = mean / info->numFrames;
			for (i = 0; i<info->numFrames; ++i)
				sum_deviation += (*(temp + i) - mean)*(*(temp + i) - mean);
			//NUMFRAMES from info structure.
			results->STD[regions].arr[row - minrow][col - mincol] = (float)sqrt(sum_deviation / (info->numFrames - 1));

			sum_deviation = 0.0;
			mean = 0.0;

		}
	}

	return find_noise(results, S_ROW, S_COL, regions);
}


float Ts_SNRTest::find_noise(SNRResults * pRes, int S_ROW, int S_COL, int regions)
{
	int row, col;
	float temp = 0.0;

	for (row = 0; row<S_ROW; row++)
	{
		for (col = 0; col<S_COL; col++)
		{
			temp += pRes->STD[regions].arr[row][col];
		}
	}

	return temp / (S_ROW*S_COL);
}

void Ts_SNRTest::find_bins(SNRResults *pRes, int S_ROW, int S_COL, int bins[HISTOGRAM], int regions)
{
	int row, col, minlim, maxlim, i;
	int count = 0;
	float temp;

	for (i = 0; i<HISTOGRAM; i++)
	{
		if (i == 0)
		{
			minlim = -1000;
			maxlim = 0;
		}
		else if (i == HISTOGRAM - 1)
		{
			minlim = i;
			maxlim = 1000;
		}
		else
		{
			minlim = i;
			maxlim = i + 1;
		}

		for (row = 0; row<S_ROW; row++)
		{
			for (col = 0; col<S_COL; col++)
			{
				temp = pRes->NORM_AVGS[regions].arr[row][col];

				if (temp >= minlim && temp<maxlim)
					count += 1;
			}
		}

		bins[i] = count;
		count = 0;
	}
}

int Ts_SNRTest::sum(int arr[HISTOGRAM], int firstindex, int lastindex)
{
	int i;
	int tempsum = 0;

	for (i = firstindex; i<lastindex; i++)
	{
		tempsum += arr[i];
	}

	return tempsum;
}

void Ts_SNRTest::SNR_Fill_Log(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRResults* pResults, int numRows, int numCols, int numFrames)
{
	int i, j, k, l;
	int min, max;
	float mean = 0.0, sum_deviation = 0.0;
	int temp = 0;
	int span[MAXFRAMES] = { 0 };

	//nofinger avg
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp += nofingerdata[k].arr[i][j];
			}

			pResults->AVG_NOFINGER[i][j] = (float)temp / numFrames;
			//Truncate
			if (pResults->AVG_NOFINGER[i][j] > 255 || pResults->AVG_NOFINGER[i][j] < 0)
				pResults->AVG_NOFINGER[i][j] = pResults->AVG_NOFINGER[i][j] > 255 ? (float)255 : (float)0;
			temp = 0;
		}
	}

	//finger avg
	temp = 0;
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp += fingerdata[k].arr[i][j];
			}

			pResults->AVG_FINGER[i][j] = (float)temp / numFrames;
			//Truncate
			if (pResults->AVG_FINGER[i][j] > 255 || pResults->AVG_FINGER[i][j] < 0)
				pResults->AVG_FINGER[i][j] = pResults->AVG_FINGER[i][j] > 255 ? (float)255 : (float)0;
			temp = 0;
		}
	}

	//nofinger span
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				span[k] = nofingerdata[k].arr[i][j];
			}
			get_span(span, &min, &max, numFrames);
			pResults->SPAN_NOFINGER[i][j] = (float)max - min;
			//Truncate
			if (pResults->SPAN_NOFINGER[i][j] > 255 || pResults->SPAN_NOFINGER[i][j] < 0)
				pResults->SPAN_NOFINGER[i][j] = pResults->SPAN_NOFINGER[i][j] > 255 ? (float)255 : (float)0;
			max = 0;
			min = 0;
		}
	}
	//finger span	
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				span[k] = fingerdata[k].arr[i][j];
			}
			get_span(span, &min, &max, numFrames);
			pResults->SPAN_FINGER[i][j] = (float)max - min;
			//Truncate
			if (pResults->SPAN_FINGER[i][j] > 255 || pResults->SPAN_FINGER[i][j] < 0)
				pResults->SPAN_FINGER[i][j] = pResults->SPAN_FINGER[i][j] > 255 ? (float)255 : (float)0;
			max = 0;
			min = 0;
		}
	}


	//nofinger std
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				*(span + k) = nofingerdata[k].arr[i][j];
				mean += *(span + k);
			}
			mean = mean / numFrames;
			for (l = 0; l<numFrames; ++l)
				sum_deviation += (*(span + l) - mean)*(*(span + l) - mean);
			//printf("%d\tstd_row\n",col-mincol);			
			pResults->STD_NOFINGER[i][j] = (float)sqrt(sum_deviation / (numFrames - 1));
			//Truncate
			if (pResults->STD_NOFINGER[i][j] > 255 || pResults->STD_NOFINGER[i][j] < 0)
				pResults->STD_NOFINGER[i][j] = pResults->STD_NOFINGER[i][j] > 255 ? (float)255 : (float)0;
			sum_deviation = 0.0;
			mean = 0.0;
		}
	}

	//finger std
	sum_deviation = 0.0;
	mean = 0.0;
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				*(span + k) = fingerdata[k].arr[i][j];
				mean += *(span + k);
			}
			mean = mean / numFrames;
			for (l = 0; l<numFrames; ++l)
				sum_deviation += (*(span + l) - mean)*(*(span + l) - mean);
			//printf("%d\tstd_row\n",col-mincol);			
			pResults->STD_FINGER[i][j] = (float)sqrt(sum_deviation / (numFrames - 1));
			//Truncate
			if (pResults->STD_FINGER[i][j] > 255 || pResults->STD_FINGER[i][j] < 0)
				pResults->STD_FINGER[i][j] = pResults->STD_FINGER[i][j] > 255 ? (float)255 : (float)0;
			sum_deviation = 0.0;
			mean = 0.0;
		}
	}
}

float Ts_SNRTest::find_mean(float* arr, int S_COL)
{
	int i;
	float tempsum = 0.0;
	for (i = 0; i<S_COL; i++)
	{
		tempsum += arr[i];
	}

	return tempsum / S_COL;
}

void Ts_SNRTest::get_span(int span[MAXFRAMES], int *min, int *max, int numFrames)
{
	int i;
	*max = -3200, *min = 3200;
	for (i = 0; i<numFrames; i++)
	{
		if (span[i]> *max)
		{
			*max = span[i];
		}
		else if (span[i] < *min)
		{
			*min = span[i];
		}
	}
}