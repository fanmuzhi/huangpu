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

	_starttime = time(NULL);
	//_starttime = clock();
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

void Syn_FingerprintTest::BurnToOTP(long nRecType, uint8_t* pSrc, int numBytes)
{
	int		timeout(2000), nSize(0);
	uint8_t arOutBuf[MS0_SIZE] = { 0 };

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


void Syn_FingerprintTest::SCM_WofTestExecute()
{
	int			timeout, timeout2;
	uint8_t		pStatus[4];
	uint8_t		pResult[2];
	Syn_PatchInfo ScmWofPatchInfo, Cmd1ScmWofPlotInfo, Cmd2ScmWofBinInfo, Cmd3SweepTagInfo, WofCmd2Info;
	_pSyn_Dut->FindPatch("ScmWofPatch", ScmWofPatchInfo);
	_pSyn_Dut->FindPatch("Cmd1ScmWofPlot", Cmd1ScmWofPlotInfo);
	_pSyn_Dut->FindPatch("Cmd2ScmWofBin", Cmd2ScmWofBinInfo);
	_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagInfo);
	_pSyn_Dut->FindPatch("WofCmd2", WofCmd2Info);

	if (NULL == ScmWofPatchInfo._pArrayBuf || NULL == Cmd1ScmWofPlotInfo._pArrayBuf || NULL == Cmd2ScmWofBinInfo._pArrayBuf || NULL == Cmd3SweepTagInfo._pArrayBuf || NULL == WofCmd2Info._pArrayBuf)
		return;

	uint8_t*    pWofCmd2Gains = WofCmd2Info._pArrayBuf;
	uint8_t*	pWofCmd1 = Cmd1ScmWofPlotInfo._pArrayBuf;
	int			nCmd1Size = Cmd1ScmWofPlotInfo._uiArraySize;
	uint8_t*	pWofCmd2 = Cmd2ScmWofBinInfo._pArrayBuf;
	int			nCmd2Size = Cmd2ScmWofBinInfo._uiArraySize;
	uint8_t*	pWofCmd3 = Cmd3SweepTagInfo._pArrayBuf;
	int			nCmd3Size = Cmd3SweepTagInfo._uiArraySize;
	uint8_t		pGetPrintMerged[3000] = { 0 };
	int			nGetPrintMergedSize = nCmd2Size + nCmd3Size;

	int			nVCC = (int)_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nVCC * 1000;

	if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount == 0)
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	else
		PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, nVCC, nVCC, true);

	//Get gains to use out of the Cmd2 file.
	uint8_t arGainIdx[MAX_THRESHOLD_GAINS] = { 0x1E, 0x22, 0x26, 0x0C, 0x10, 0x14 };
	uint8_t arGainMergeIdx[MAX_THRESHOLD_GAINS] = { 0x1E, 0x22, 0x26, 0x0B, 0x0F, 0x13 };
	for (int i = 0; i<MAX_THRESHOLD_GAINS; i++)
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arStartStopThreshGains[i] = pWofCmd2Gains[arGainIdx[i]];

	//Modify Cmd2 
	//Write cmd2. 
	uint8_t* pTempBuf = new uint8_t[nCmd2Size];
	memcpy(pTempBuf, pWofCmd2, nCmd2Size);
	pTempBuf[0x1E] = 0;
	pTempBuf[0x23] = 0;
	pTempBuf[0x26] = 0xDF;
	pTempBuf[0xB] = 0x80;
	pTempBuf[0xF] = 0;
	pTempBuf[0x13] = 0;

	//Merge GetPrint
	for (int i = 0; i<MAX_THRESHOLD_GAINS; i++)
		pWofCmd3[arGainMergeIdx[i]] = _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arStartStopThreshGains[i];
	pGetPrintMerged[0] = 0x70;
	pGetPrintMerged[1] = 0;
	pGetPrintMerged[2] = 0;
	pGetPrintMerged[3] = 0;
	memcpy(&pGetPrintMerged[4], pTempBuf, nCmd2Size);
	memcpy(&pGetPrintMerged[nCmd2Size + 4], pWofCmd3, nCmd3Size);

	pGetPrintMerged[nCmd2Size + 4 + 11] = 100;
	pGetPrintMerged[nCmd2Size + 4 + 15] = 100;
	pGetPrintMerged[nCmd2Size + 4 + 19] = 200;
	pGetPrintMerged[nCmd2Size + 4 + 38] = 63;

	delete[] pTempBuf;

	_pSyn_DutCtrl->FpUnloadPatch();
	//Download patch.
	_pSyn_DutCtrl->FpLoadPatch(ScmWofPatchInfo._pArrayBuf, ScmWofPatchInfo._uiArraySize);
	//GetDutCtrl().FpLoadPatch(GetSysConfig().GetPtr("ScmWofPatch"), GetSysConfig().GetSize("ScmWofPatch"));

	//Write cmd1.
	_pSyn_DutCtrl->FpWrite(1, pWofCmd1[0], &pWofCmd1[1], nCmd1Size - 1);

	//Wait for cmd1 to complete.
	timeout = 1000;
	_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
	while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
	{
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF status never complete.");
		throw(ex);
	}
	//Check error code returned by cmd1.
	timeout = 1000;
	_pSyn_DutCtrl->FpRead(1, 0xFF, pResult, sizeof(pResult));
	while (timeout && !((pResult[0] != 0xFF) || (pResult[1] != 0xFF)))
	{
		_pSyn_DutCtrl->FpRead(1, 0x01, pResult, sizeof(pResult));
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF: Command 1 never complete.");
		throw(ex);
	}

	//write cmd2 with GetPrintMerged
	_pSyn_DutCtrl->FpWrite(1, 0x02, pGetPrintMerged, nGetPrintMergedSize + 4);

	//Wait for cmd2 to complete.
	timeout = 1000;
	_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
	while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
	{
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF status never complete.");
		throw(ex);
	}
	//Check error code returned by cmd2.
	timeout = 1000;
	_pSyn_DutCtrl->FpRead(1, 0xFF, pResult, sizeof(pResult));
	while (timeout && !((pResult[0] != 0xFF) || (pResult[1] != 0xFF)))
	{
		_pSyn_DutCtrl->FpRead(1, 0x01, pResult, sizeof(pResult));
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF: Command 2 never complete.");
		throw(ex);
	}

	//Execute command to read SCM WOF data.
	_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);

	//Wait for command complete
	timeout = 1000;
	_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
	while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
	{
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		timeout--;

		if (timeout % 44 == 0)
			_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);
	}

	_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);

	//	if (timeout == 0)
	//	{
	//		ExTestStep ex(0);
	//		ex.SetDescription("SCM WOF: Status never complete.");
	//		throw(ex);
	//	}

	//Get response data.
	timeout2 = 1000;
	_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);
	while (timeout2 && !((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[0] == 0x00) && (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[1] == 0x00)))
	{
		//Try retrieving response data again.
		_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);

		//Wait for response to be ready.
		timeout = 1000;
		_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
		while (timeout && !((pStatus[0] == 0x03) && (pStatus[1] == 0x00) && (pStatus[2] == 0x00) && (pStatus[3] == 0x30)))
		{
			_pSyn_DutCtrl->FpGetStatus(pStatus, sizeof(pStatus));
			timeout--;

			if (timeout % 10 == 0)
				_pSyn_DutCtrl->FpWrite(1, 0xFA, (uint8_t*)0, 0);
		}

		//		if (timeout == 0)
		//		{
		//			ExTestStep ex(0);
		//			ex.SetDescription("SCM WOF status never complete.");
		//			throw(ex);
		//		}

		//Check error code.
		timeout = 1000;
		_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);
		while (timeout && !((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[0] == 0x02) || ((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[0] == 0x00) && (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[1] == 0x00))))
		{
			_pSyn_DutCtrl->FpRead(1, 0xFF, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData, _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes);
			timeout--;
		}
		if (timeout == 0)
		{
			Syn_Exception ex(0);
			ex.SetDescription("SCM WOF: Command 1 never complete.");
			throw(ex);
		}
		timeout2--;
	}
	if (timeout2 == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("SCM WOF: Cannot get plot.");
		throw(ex);
	}

	//Organize bits from the response of the sensor
	for (int i = MAX_THRESHOLD_GAINS; i < _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes; i++)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] = ((_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] == 0) || (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i] == 2)) ? 1 : 0;
	}
}

void Syn_FingerprintTest::SCM_WofTestProcessData()
{
	int			i;
	int			nTgrIdx;
	uint8_t		m_arGains[2] = { 0 };
	int			nRecSize = (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nNumResponseBytes - 6) / 2;
	int			bTgrFound = 0;
	int			nDelta;
	uint8_t*	pBuf;

	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;	//Assume true.

	if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_bWithStimulus == 0)  // If NO Stimulus
	{
		//Find the trigger index from gain of 100.
		nTgrIdx = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i + 6] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 0;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = nTgrIdx - 1;

		//Get the trigger index for the second gain.
		nTgrIdx = 0;
		bTgrFound = 0;
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[i + 6 + nRecSize] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = nTgrIdx - 1;

		//If invalid threshold from gain of 100.
		if ((nTgrIdx < _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMinTriggerThreshold) || (nTgrIdx >= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nMaxTriggerThreshold))
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 1;
	}
	else if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount == 0)	//With stimulus at 3.3V.
	{
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 1;
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 1;

		//Get trg index at gain 100.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = nTgrIdx - 1;

		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 - _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100;
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_100)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 0;
			nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 - _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200;
			if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200)
			{
				_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 0;
				_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
			}
		}
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nWithStimCount++;
	}
	else //With stimulus at 3.7V.
	{
		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &_pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_arWofData[6 + nRecSize];
		for (i = 0; (i<nRecSize) && (bTgrFound == 0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}

		// To save log for 3.7v pass
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200_3p7 = nTgrIdx - 1;

		nDelta = _pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 - (nTgrIdx - 1);
		if (nDelta <= _pSyn_Dut->_pSyn_DutTestInfo->_SCM_wofInfo.m_nDelta_200_3p7)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 0;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
		}
	}
}

void Syn_FingerprintTest::GetOtpScmWofData(uint8_t pOtpData[MS0_SIZE], int OtpScmWofDataCount)
{
	uint32_t    extTagData = 0;

	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bValid = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200_3p7V = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200_3p7 = 0;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = 0;

	/*Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
	Syn_Exception ex(0);
	ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
	ex.SetDescription("Wof:OtpReadWritePatch Patch is null!");
	throw ex;
	return;
	}

	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);*/


	//if (GetMS0RecordData(TAG_CAL, EXT_TAG_WOF_BOT, pOtpData, MS0_SIZE, site) != 0)
	if (0 != OtpScmWofDataCount)
	{
		extTagData = *((uint32_t*)pOtpData);
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = (uint8_t)((extTagData & 0x00FF0000) >> 16);
		_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim = (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim / 100) - 1;
		if (_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nRecIdxWithoutStim == 0)	//gain 100
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain100 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_100 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_100 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
		else
		{
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPassAtGain200 = 1;
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithoutStim_200 = (uint8_t)(extTagData & 0x000000FF);
			_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_nTriggerIdxWithStim_200 = (uint8_t)((extTagData & 0x0000FF00) >> 8);
		}
	}

	// OTP data is real gain. The write_logfile() function multiply 100 after increasing 1.
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bValid = 1;
	_pSyn_Dut->_pSyn_DutTestResult->_SCM_wofResults.m_bPass = 1;
}


void Syn_FingerprintTest::ComputeRunningTime(double &ioRunningTime)
{
	_finishtime = time(NULL);
	ioRunningTime = difftime(_finishtime, _starttime)*1000;

	//_finishtime = clock();
	//ioRunningTime = (double)(_finishtime - _starttime) / CLOCKS_PER_SEC;
}