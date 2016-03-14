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
	for (i = 0; i<10; i++)
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
	}

}

void Ts_SNRTest::CleanUp()
{
}