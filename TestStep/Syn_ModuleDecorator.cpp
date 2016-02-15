#include "Syn_ModuleDecorator.h"


Syn_ModuleDecorator::Syn_ModuleDecorator()
{
}


Syn_ModuleDecorator::~Syn_ModuleDecorator()
{
}

void Syn_ModuleDecorator::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols, Syn_DutCtrl * &pDutCtrl)
{
	if (NULL == pDutCtrl)
	{
		return;
	}

	int			nRows = nNumRows;
	int			nCols = nNumCols;

	if (((nRows * nCols) % 64) == 0)
		nRows++;

	uint8_t *pImgBuff = new uint8_t[nCols * nRows];

	//pDutCtrl->FpGetImage(pImgBuff, numCols*numRows);
	pDutCtrl->FpGetImage2(nRows, nCols, pImgBuff, pCalResults.m_nPrintPatchSize, pCalResults.m_pPrintPatch);


	for (int i = 0; i < (nRows * nCols); i++)
	{
		pFrame->arr[i / nCols][i%nCols] = pImgBuff[i];
	}

	//for (int i = 0; i < nRows; i++)
	for (int i = 0; i < nNumRows; i++)
	{
		std::string strTempRowValue;
		for (int j = HEADER; j < nNumCols; j++)		// HEADER defined the first 8 cols to ignore.
		{
			//strTempRowValue += to_string(pImgBuff[i*nRows + j]) + std::string(",");
			strTempRowValue += to_string(pFrame->arr[i][j]) + std::string(",");
		}
	}

	delete[] pImgBuff;
	pImgBuff = NULL;
}