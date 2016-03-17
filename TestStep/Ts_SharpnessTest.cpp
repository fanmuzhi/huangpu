#include "Ts_SharpnessTest.h"

Ts_SharpnessTest::Ts_SharpnessTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_SharpnessTest::~Ts_SharpnessTest()
{
}

void Ts_SharpnessTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.limit = 30;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize >= 1)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.limit = atoi(listOfArgValue[0].c_str());
	}
}


void Ts_SharpnessTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.m_bExecuted = true;

}

void Ts_SharpnessTest::ProcessData()
{
	int nTrimLeft = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int nTrimRight = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int nTrimTop = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int nTrimBottom = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	//call get_sensor_matrices in .c
	float nofinger[MAXROW][MAXCOL] = { 0 };
	float finger[MAXROW][MAXCOL] = { 0 };
	get_sensor_matrices(&(_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult), &(_pSyn_Dut->_pSyn_DutTestInfo->_snrInfo), &(_pSyn_Dut->_pSyn_DutTestResult->_snrResults), nofinger, finger, numRows, numCols);

	int height = numRows - (nTrimTop + nTrimBottom);
	int width = numCols - HEADER - (nTrimLeft + nTrimRight);

	int i, j;
	int M = height;
	int N = width;
	int C0 = N / 3;
	int C1 = C0 + C0;
	int C2 = N;
	int min, max;
	float measure;

	float zones[3] = { 0 };
	float tempImg[MAXROW][MAXCOL];
	
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.bPass = 1;

	//overall
	get_sharpness(M, N, finger, &measure, &(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults));
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[3] = measure;
	//zone 1
	for (i = 0; i<M; i++)
	{
		for (j = 0; j<C0; j++)
		{
			tempImg[i][j] = finger[i][j];
		}
	}
	get_sharpness(M, (C0 + 1), tempImg, &measure, &(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults));
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[0] = measure;
	//zone 2
	for (i = 0; i<M; i++)
	{
		for (j = C0; j<C1; j++)
		{
			tempImg[i][j - C0] = finger[i][j];
		}
	}
	get_sharpness(M, (C1 - C0 + 1), tempImg, &measure, &(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults));
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[1] = measure;
	//zone 3
	for (i = 0; i<M; i++)
	{
		for (j = C1; j<C2; j++)
		{
			tempImg[i][j - C1] = finger[i][j];
		}
	}
	get_sharpness(M, (C2 - C1 + 1), tempImg, &measure, &(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults));
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[2] = measure;

	zones[0] = _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[0];
	zones[1] = _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[1];
	zones[2] = _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[2];
	min = find_min(zones, 3);
	max = find_max(zones, 3);
	_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.percent = (float)abs((int)(((zones[max] - zones[min]) / _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[3]) * 100));

	if (_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.percent > _pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.limit)
		_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sSharpnessFail);
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.m_elapsedtime);

}

void Ts_SharpnessTest::CleanUp()
{
}