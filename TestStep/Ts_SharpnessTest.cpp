#include "Ts_SharpnessTest.h"

//sharpness test kernels
static int m_kernelSharpnessGx[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
static int m_kernelSharpnessGy[3][3] = { { 1, 0, -1 }, { 2, 0, -2 }, { 1, 0, -1 } };

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
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SharpnessTest", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("SharpnessTest", "Pass"));

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.m_elapsedtime);

}

void Ts_SharpnessTest::CleanUp()
{
}

void Ts_SharpnessTest::get_sensor_matrices(AcqImgNoFingerResult* pFrames, SNRInfo *pInfo, SNRResults* pResult, float nofinger[MAXROW][MAXCOL], float finger[MAXROW][MAXCOL], int numRows, int numCols)
{
	int i, j, k;
	int temp, numFrames;
	numFrames = pInfo->numFrames;

	temp = 0;
	//note that nofinger is printed in raw form.
	for (i = 0; i<numRows; i++)
	{
		for (j = 0; j<numCols; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp += pFrames->arr_nofinger[k].arr[i][j];
			}
			//nofinger avg:
			nofinger[i][j] = (float)temp / numFrames;
			temp = 0;
		}
	}
	//finger avg:
	temp = 0;
	for (i = pInfo->numMinRows; i<pInfo->numMaxRows; i++)
	{
		for (j = pInfo->numMinCols; j<pInfo->numMaxCols - HEADER; j++)
		{
			finger[i - pInfo->numMinRows][j - pInfo->numMinCols] = pResult->NORM_AVGS[6].arr[i - pInfo->numMinRows][j - pInfo->numMinCols];
		}
	}
}

void Ts_SharpnessTest::get_sharpness(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessResults* pSharpness)
{
	int i, j;
	int m, n;
	double x, y;
	int tempSum = 0;
	int sum;
	int sharpImage[MAXROW][MAXCOL];

	int tempImg[3][3];
	int tempGx[3][3];
	int tempGy[3][3];

	int product[MAXROW][MAXCOL];

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			//==================
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempImg[m][n] = (int)pImg[i + m][j + n];
				}
			}
			//==================
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempGx[m][n] = tempImg[m][n] * m_kernelSharpnessGx[m][n];
				}
			}
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempSum += tempGx[m][n];
				}
			}
			x = tempSum;
			tempSum = 0;
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempGy[m][n] = tempImg[m][n] * m_kernelSharpnessGy[m][n];
				}
			}
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempSum += tempGy[m][n];
				}
			}
			y = tempSum;
			tempSum = 0;
			sum = (int)((x*x) + (y*y));
			sum = (int)sqrt((long double)sum);
			sharpImage[i][j] = sum;
			sum = 0;
		}
	}

	gradient(height, width, sharpImage, pSharpness->Gx, pSharpness->Gy, pSharpness);
	for (i = 0; i<height - 2; i++)
	{
		for (j = 0; j<width - 2; j++)
		{
			product[i][j] = (pSharpness->Gx[i][j] * pSharpness->Gx[i][j]) + (pSharpness->Gy[i][j] * pSharpness->Gy[i][j]);
			product[i][j] = (int)sqrt((long double)product[i][j]);
		}
	}
	sum = 0;
	for (i = 0; i<height - 3; i++)
	{
		for (j = 0; j<width - 4; j++)
		{
			sum += product[i][j];
		}
	}
	//overall zone
	*sharpnessMeasure = (float)(sum / ((height - 2)*(width - 4)));
}

int Ts_SharpnessTest::find_min(float* pArray, int size)
{
	int min, i, loc = 0;
	min = (int)pArray[0];
	for (i = 1; i<size; i++)
	{
		if (pArray[i] < min)
		{
			min = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}
//returns location of max
int Ts_SharpnessTest::find_max(float* pArray, int size)
{
	int max, i, loc = 0;
	max = (int)pArray[0];
	for (i = 1; i<size; i++)
	{
		if (pArray[i] > max)
		{
			max = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}

void Ts_SharpnessTest::gradient(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int Gx[MAXROW][MAXCOL], int Gy[MAXROW][MAXCOL], SharpnessResults* pSharpness)
{
	int n, p;
	int k, ndim, i;
	int row, col;
	ndim = 2;

	for (k = 0; k<ndim; k++)
	{
		//56x144
		if (k == 0)
		{
			n = numRowsImg;
			p = numColsImg;
			for (i = 0; i<numRowsImg; i++)
				pSharpness->h[i] = i;
			//Take forward differences on left and right edges
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[0][i] = pImg[1][i] - pImg[0][i];
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[n][i] = pImg[n][i] - pImg[n - 1][i];

			for (row = 2; row<n; row++)
			{
				for (col = 0; col<p; col++)
				{
					pSharpness->g[row - 1][col] = (pImg[row][col] - pImg[row - 2][col]) / 2;
				}
			}
			//Obtain Gy
			for (row = 0; row<numRowsImg - 2; row++)
			{
				for (col = 0; col<numColsImg; col++)
				{
					Gy[row][col] = pSharpness->g[row][col];
				}
			}

			//init. g[][] to zero
			for (row = 0; row<MAXROW; row++)
			{
				for (col = 0; col<MAXCOL; col++)
				{
					pSharpness->g[row][col] = 0;
				}
			}
		}
		//144x56
		else
		{
			n = numColsImg;
			p = numRowsImg;
			//transpose image 
			for (row = 0; row<numColsImg; row++)
			{
				for (col = 0; col<numRowsImg; col++)
				{
					pSharpness->pImg_t[row][col] = pImg[col][row];
				}
			}
			for (i = 0; i<numColsImg; i++)
				pSharpness->h[i] = i;
			//Take forward differences on left and right edges
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[0][i] = pSharpness->pImg_t[1][i] - pSharpness->pImg_t[0][i];
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[n][i] = pSharpness->pImg_t[n][i] - pSharpness->pImg_t[n - 1][i];

			for (row = 2; row<n; row++)
			{
				for (col = 0; col<p; col++)
				{
					pSharpness->g[row - 1][col] = (pSharpness->pImg_t[row][col] - pSharpness->pImg_t[row - 2][col]) / 2;
				}
			}
			//transpose image and Obtain Gx			
			for (row = 0; row<p; row++)
			{
				for (col = 0; col<n; col++)
				{
					Gx[row][col] = pSharpness->g[col][row];
				}
			}
		}
	}

}