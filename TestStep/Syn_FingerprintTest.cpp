//Local
#include "Syn_FingerprintTest.h"
#include "Syn_MetallicaModule.h"
#include "Syn_ViperModule.h"

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
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;
			
		case Viper2:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;

		default:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;		
	}
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


void Syn_FingerprintTest::CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->CalculateLnaOffsetsBinarySearch(pFrame, pLnaValues, nNumRows, nNumCols, CalInfo, CalResults);
}


void Syn_FingerprintTest::GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetRowAverages(pFrame, nColBegin, nColEnd, pAverages, nNumRows);
}


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
