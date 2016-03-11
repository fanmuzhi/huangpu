#include "Ts_PixelTest.h"


Ts_PixelTest::Ts_PixelTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_PixelTest::~Ts_PixelTest()
{
}

void Ts_PixelTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.numFrames = 30;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_row = 50;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_col = 50;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nTrim = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nCountFailedPixels = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nThresholdRange = 45;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 6)
	{
		for (size_t t = 1; t <= 6 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_row = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_col = atoi(listOfArgValue[2].c_str());
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nTrim = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nThresholdRange = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nCountFailedPixels = atoi(listOfArgValue[5].c_str());
}


void Ts_PixelTest::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.m_bExecuted = true;

}

void Ts_PixelTest::ProcessData()
{
	int nTrimLeft = 0;
	int nTrimRight = 0;
	int nTrimTop = 0;
	int nTrimBottom = 0;
	int numRows = _pSyn_Dut->_RowNumber;
	int numCols = _pSyn_Dut->_ColumnNumber;

	int   i, j, k;
	int   temp, numFrames;
	int   row_limit, col_limit;
	float span_row[MAXROW] = { 0 };
	float span_col[MAXCOL] = { 0 };
	float mean = 0.0, sum_deviation = 0.0;
	float mean_rows = 0.0;
	float mean_cols = 0.0;
	int   sizeRows = (numRows - nTrimBottom - 1 - 4) - (nTrimTop + 1);

	int nMaxPixel = 0;
	int nMinPixel = -1;

	int	nCountBadPixel = 0;
	row_limit = _pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_row;
	col_limit = _pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.fail_col;

	temp = 0, k = 0;
	//set result to pass before anything is done...
	_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.bPass = 1;
	numFrames = _pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.numFrames;

	for (i = nTrimTop; i<numRows - nTrimBottom; i++)
	{
		for (j = nTrimLeft + HEADER; j<numCols - nTrimRight; j++)
		{
			for (k = 0; k<numFrames; k++)
			{
				temp += _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger[k].arr[i][j];
			}
			_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.A[i - nTrimTop][j - nTrimLeft - HEADER] = (int)temp / numFrames;
			temp = 0;
		}
	}

	for (i = 0; i < numRows - (nTrimBottom + nTrimTop) - 1; i++)
	{
		for (j = 0; j < numCols - (HEADER + nTrimLeft + nTrimRight) - 1; j++)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.DA[i][j] = (_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.A[i + 1][j + 1] - _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.A[i + 1][j]) - (_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.A[i][j + 1] - _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.A[i][j]); //DA => difference image
		}
	}

	temp = 0;
	//process DA(difference image) according to Mike's requested hardcoded values of 8 from each edge
	//find max
	for (i = 8; i < numRows - (nTrimTop + nTrimBottom) - 8; i++)
	{
		for (j = 8; j < numCols - (HEADER + nTrimLeft + nTrimRight) - 8; j++)
		{
			temp = _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.DA[i][j];
			//count the bad pixels
			if ((temp >= _pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nThresholdRange) || (temp <= -(_pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nThresholdRange)))
				nCountBadPixel += 1;

			//find min and max in the 2d diff image
			if (temp <= nMinPixel)
				nMinPixel = temp;
			if (temp > nMaxPixel)
				nMaxPixel = temp;
		}
	}

	_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nMaxPixelValue = nMaxPixel;
	_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nMinPixelValue = nMinPixel;
	_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nCountAboveThreshold = nCountBadPixel;

	if (nCountBadPixel > _pSyn_Dut->_pSyn_DutTestInfo->_pixelInfo.nCountFailedPixels)
		_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.bPass = 0;

	if (!(_pSyn_Dut->_pSyn_DutTestResult->_pixelResults.bPass))
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sPixelUniformityFail);
	}
}

void Ts_PixelTest::CleanUp()
{
}