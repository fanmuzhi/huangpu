//Local
#include "Syn_FingerprintTest.h"
#include "Syn_MetallicaModule.h"
#include "Syn_ViperModule.h"
#include "Syn_Viper1Module.h"
#include "Syn_Viper2Module.h"

//windows
//#include "windows.h" 

Syn_FingerprintTest::Syn_FingerprintTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_TestStep(strName, strArgs, pDutCtrl, pDut)
, _pSyn_Module(NULL)
{
	switch (_pSyn_Dut->_eProjectType)
	{
		case Metallica:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;

		case Viper1:
			_pSyn_Module = new Syn_Viper1Module();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;
			
		case Viper2:
			_pSyn_Module = new Syn_Viper2Module();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;

		default:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;		
	}

	//_starttime = time(NULL);
	_starttime = GetTickCount();
}


Syn_FingerprintTest::~Syn_FingerprintTest()
{
	if (NULL != _pSyn_Module)
	{
		delete _pSyn_Module;
		_pSyn_Module = NULL;
	}
}


void Syn_FingerprintTest::PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->PowerOn(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh, bDisableSleep);
}


void Syn_FingerprintTest::PowerOff()
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->PowerOff();
}


void Syn_FingerprintTest::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->CopyToPrintPatch(pSrc, pPrintPatch, nNumBytes, nPatchIdx);
}


bool Syn_FingerprintTest::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	if (NULL == _pSyn_Module)
	{
		return false;
	}

	return _pSyn_Module->CalculatePgaOffsets_OOPP(numCols, numRows, calInfo, calResult);
}


void Syn_FingerprintTest::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetFingerprintImage(pCalResults, pFrame, nNumRows, nNumCols);
}

void Syn_FingerprintTest::GetFingerprintImageForCurrentTest(CalibrationResults& pCalResults, FPSFrame* pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetFingerprintImageForCurrentTest(pCalResults, pFrame, nNumRows, nNumCols, pCurrentDrawVals, nGain);
}

void Syn_FingerprintTest::CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->CalculateLnaOffsetsBinarySearch(pFrame, pLnaValues, nNumRows, nNumCols, CalInfo, CalResults);
}


/*
void Syn_FingerprintTest::GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetRowAverages(pFrame, nColBegin, nColEnd, pAverages, nNumRows);
}
*/


bool Syn_FingerprintTest::CheckDUTexists()
{
	if (NULL == _pSyn_Module)
	{
		return false;
	}

	return _pSyn_Module->CheckDUTexists();
}


void Syn_FingerprintTest::ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	return _pSyn_Module->ImageDecode(pDecodeFrame, pEncodeFrame, nNumRow, nNumCol, nNumFrames);
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

void Syn_FingerprintTest::BurnToOTP(long nRecType, uint8_t* pSrc, int numBytes)
{
	int		timeout(2000), nSize(0);
	uint8_t arOutBuf[MS0_SIZE + MS1_SIZE] = { 0 };

	arOutBuf[4] = 0x11;
	arOutBuf[12] = (uint8_t)(nRecType & 0xFF);
	arOutBuf[15] = (uint8_t)((nRecType >> 24) & 0xFF);
	arOutBuf[18] = 0x0E;

	if ((nRecType == EXT_TAG_PGA_OOPR) || (nRecType == EXT_TAG_PGA_OOPP) ||(nRecType == EXT_TAG_LNA) || (nRecType == EXT_TAG_WOF_BOT) || (nRecType == EXT_TAG_SCM_WOF_BOT))
	{
		//PGA and LNA records have an extra 4 bytes (0x00000007).
		arOutBuf[24] = 0x07;
		arOutBuf[16] = numBytes + 4;
		arOutBuf[17] = (numBytes + 4) >> 8;
		memcpy(&arOutBuf[28], pSrc, numBytes);
		nSize = numBytes + 28;
	}
	else //All other records.
	{
		arOutBuf[16] = numBytes;
		arOutBuf[17] = numBytes >> 8;
		memcpy(&arOutBuf[24], pSrc, numBytes);
		nSize = numBytes + 24;
	}

	uint8_t pStatus[4] = { 0 };
	uint8_t arExtendedTag[2] = { 0 };
	uint8_t arNumBytes[2] = { 0 };

	_pSyn_DutCtrl->FpOtpRomTagWrite(arOutBuf, nSize);

}


void Syn_FingerprintTest::ComputeRunningTime(double &ioRunningTime)
{
	//_finishtime = time(NULL);
	//ioRunningTime = difftime(_finishtime, _starttime)*1000;

	_finishtime = GetTickCount();
	ioRunningTime = (double)(_finishtime - _starttime);
}