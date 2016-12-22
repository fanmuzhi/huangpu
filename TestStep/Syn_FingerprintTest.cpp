//Local
#include "Syn_FingerprintTest.h"

//windows
//#include "windows.h" 

Syn_FingerprintTest::Syn_FingerprintTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_TestStep(strName, strArgs, pDutCtrl, pDut)
{
#ifdef _DEBUG
	if (strName != "WaitStimulus")
	{
		LOG(DEBUG) << "============================== " << strName << " ==============================";
	}
#endif

	pDutCtrl->GetBridge(_pSynBridge);

	_startTime = GetTickCount64();
}


Syn_FingerprintTest::~Syn_FingerprintTest()
{
}

void Syn_FingerprintTest::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

	int	 nRows = nNumRows;
	int	 nCols = nNumCols;

	if (((nRows * nCols) % 64) == 0)
		nRows++;

	uint8_t *pImgBuff = new uint8_t[nCols * nRows];

	uint32_t rc = _pSyn_DutCtrl->FpGetImage2(nRows, nCols, pImgBuff, pCalResults.m_nPrintPatchSize, pCalResults.m_pPrintPatch);
	if (0 != rc)
	{
		Syn_Exception ex(rc);
		ex.SetDescription("FpGetImage2 is failed!");
		throw ex;
		return;
	}

	for (int i = 0; i < (nRows * nCols); i++)
	{
		pFrame->arr[i / nCols][i%nCols] = pImgBuff[i];
	}

	delete[] pImgBuff;
	pImgBuff = NULL;
	
	return;
}

void Syn_FingerprintTest::GetFingerprintImageForCurrentTest(CalibrationResults& pCalResults, FPSFrame* pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain)
{
	/*if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetFingerprintImageForCurrentTest(pCalResults, pFrame, nNumRows, nNumCols, pCurrentDrawVals, nGain);*/
}

void Syn_FingerprintTest::ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames)
{
	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

	for (int i = 0; i < nNumFrames; i++)
	{
		_pSyn_DutCtrl->ImageDecode(pDecodeFrame[i].arr, pEncodeFrame[i].arr, nNumRow, nNumCol);
	}
}

bool Syn_FingerprintTest::ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol)
{
	olistOfArgValue.clear();

	unsigned int iLength = strArgsValue.length();
	if (0 == iLength)
	{
		return false;
	}

	std::string strTempValue = strArgsValue;
	while (true)
	{
		size_t SymbolPosition = strTempValue.find(strSymbol);
		if (SymbolPosition == std::string::npos)
		{
			if (0 != strTempValue.length())
			{
				olistOfArgValue.push_back(strTempValue);
			}
			break;
		}

		std::string strArgValue = strTempValue.substr(0, SymbolPosition);
		olistOfArgValue.push_back(strArgValue);
		strTempValue = strTempValue.substr(SymbolPosition + 1, strTempValue.length() - SymbolPosition);
		//strTempValue = strTempValue.substr(SymbolPosition + strSymbol.length(), strTempValue.length() - SymbolPosition-strSymbol.length());
	}

	return true;
}

void Syn_FingerprintTest::ComputeRunningTime(double &ioRunningTime)
{
	_finishTime = GetTickCount64();
	ioRunningTime = (double)(_finishTime - _startTime);
}

int Syn_FingerprintTest::max_array(int a[], int num_elements)
{
	int i, max = -32000;
	for (i = 0; i<num_elements; i++)
	{
		if (a[i]>max)
		{
			max = a[i];
		}
	}
	return(max);
}

void Syn_FingerprintTest::RemoveBaseline(FPSFrame *pImgFingerArr, FPSFrame *pImgNoFingerArr, int nNumRow, int nNumCol)
{
	float tempSum = 0;
	float tempMean[MAXROW] = {0};
	//FPSFrame tempFrame;

	for (int row = 0; row < nNumRow; row++)
	{
		for (int col = 0; col < nNumCol; col++)
		{
			//pImgFingerArr->arr[row][col] = pImgFingerArr->arr[row][col] -pImgNoFingerArr->arr[row][col] + 128;
			int temp = pImgFingerArr->arr[row][col] -pImgNoFingerArr->arr[row][col] + 128;
			temp = temp > 255 ? 255 : temp;
			temp = temp < 0 ? 0 : temp;
			pImgFingerArr->arr[row][col] = temp;
			tempSum += pImgFingerArr->arr[row][col];
			//tempSum += temp;
		}

		tempMean[row] = (float)(tempSum / nNumCol);
		tempSum = 0;
	}

	for (int row = 0; row < nNumRow; row++)
	{
		for (int col = 0; col < nNumCol; col++)
		{
			int temp = (pImgFingerArr->arr[row][col] - tempMean[row] + 128);
			temp = temp > 255 ? 255 : temp;
			temp = temp < 0 ? 0 : temp;
			pImgFingerArr->arr[row][col] = temp;
		}
	}

}