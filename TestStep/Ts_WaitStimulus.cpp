#include "Ts_WaitStimulus.h"

Ts_WaitStimulus::Ts_WaitStimulus(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_WaitStimulus::~Ts_WaitStimulus()
{
}

void Ts_WaitStimulus::SetUp()
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

	if (!_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		//ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		//ex.SetDescription("Calibrate has not executed!");
		//throw ex;
		return;
	}

	//power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	//_pSyn_DutCtrl->FpUnloadPatch();

	Syn_PatchInfo ImgAcqPatchInfo;
	if (_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		if (0 != ImgAcqPatchInfo._uiArraySize)
		{
			_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
		}
	}
}

void Ts_WaitStimulus::Execute()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		return;
	}

	uint16_t numRows = _pSyn_Dut->_RowNumber;
	uint16_t numCols = _pSyn_Dut->_ColumnNumber;

	FPSFrame *pFrame = new FPSFrame();
	GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, numRows, numCols);

	//image decode
	ImageDecode(&(_pSyn_Dut->_pSyn_DutTestResult->_WaitStimulusResults._ImagepFrame), pFrame, numRows, numCols, 1);

	//Trim
	int	numMaxColsWithStim = numCols - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithStim;
	int	numMinColsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithStim;
	int	numMaxRowsWithStim = numRows - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim;;
	int	numMinRowsWithStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim;
	int	numMaxColsWOStim = numCols - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;
	int	numMinColsWOStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;
	int	numMaxRowsWOStim = numRows - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int	numMinRowsWOStim = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;

	int numMaxCols = (numMaxColsWOStim < numMaxColsWithStim) ? numMaxColsWOStim : numMaxColsWithStim;
	int numMinCols = (numMinColsWOStim < numMinColsWithStim) ? numMinColsWithStim : numMinColsWOStim;
	int numMaxRows = (numMaxRowsWOStim < numMaxRowsWithStim) ? numMaxRowsWOStim : numMaxRowsWithStim;
	int numMinRows = (numMinRowsWOStim < numMinRowsWithStim) ? numMinRowsWithStim : numMinRowsWOStim;

	for (int i = numMinRows; i<numMaxRows; i++)
	{
		for (int j = numMinCols + HEADER; j<numMaxCols; j++)
		{
			int iTempValue(0);
			iTempValue = _pSyn_Dut->_pSyn_DutTestResult->_WaitStimulusResults._ImagepFrame.arr[i][j];
			_pSyn_Dut->_pSyn_DutTestResult->_WaitStimulusResults._ImagepFrame.arr[i - numMinRows][j - numMinCols - HEADER] = iTempValue;
		}
	}

	delete pFrame;
	pFrame = NULL;
}

void Ts_WaitStimulus::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		return;
	}

	_pSyn_Dut->_pSyn_DutTestResult->_WaitStimulusResults.iRealRowNumber = _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealRowNumber;
	_pSyn_Dut->_pSyn_DutTestResult->_WaitStimulusResults.iRealColumnNumber = _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealColumnNumber;
}

void Ts_WaitStimulus::CleanUp()
{
	PowerOff();
}