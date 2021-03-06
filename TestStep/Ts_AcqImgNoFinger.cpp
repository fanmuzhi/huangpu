#include "Ts_AcqImgNoFinger.h"


Ts_AcqImgNoFinger::Ts_AcqImgNoFinger(string &strName, string &strArgs,  Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_AcqImgNoFinger::~Ts_AcqImgNoFinger()
{
}


void Ts_AcqImgNoFinger::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesToDiscard = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus = 30;

	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesToDiscard= atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus= atoi(listOfArgValue[1].c_str());

	//power on
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load ImgAcqPatch
	/*Syn_PatchInfo ImgAcqPatchInfo;
	if (!_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("ImageAcqPatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);*/

	Syn_PatchInfo ImgAcqPatchInfo;
	if (_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		if (0 != ImgAcqPatchInfo._uiArraySize)
		{
			_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
		}
	}
}


void Ts_AcqImgNoFinger::Execute()
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

	//CheckDUTexists();
	
	uint16_t numRows		= _pSyn_Dut->_RowNumber;
	uint16_t numCols		= _pSyn_Dut->_ColumnNumber;
	int nNumImgsToDiscard	= _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesToDiscard;
	int nNumframes			= _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;

	//image to discard
	for (int i = 0; i < nNumImgsToDiscard; i++)
	{
		FPSFrame *pFrame = new FPSFrame();
		GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, numRows, numCols);

		delete pFrame;
		pFrame = NULL;
	}

	//acqire user-specified number of images
	for (int i = 0; i < nNumframes; i++)
	{
		GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, &_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus[i], numRows, numCols);
	}


	_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_bExecuted = true;
}


void Ts_AcqImgNoFinger::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep AcqImgNoFinger is not executed!");
		throw ex;
	}

	uint16_t numRows		= _pSyn_Dut->_RowNumber;
	uint16_t numCols		= _pSyn_Dut->_ColumnNumber;
	int nNumframes			= _pSyn_Dut->_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_nNumImagesWithoutStimulus;

	//image decode
	ImageDecode(_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger, _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus, numRows, numCols, nNumframes);

	//caculate avg	
	int temp = 0;
	for (int i=0; i<numRows; i++)
	{
		for(int j=0; j<numCols; j++)
		{
			for(int k=0; k<nNumframes; k++)
			{
				temp+=_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_nofinger[k].arr[i][j];
			}

			int result = (float) temp/nNumframes;
			if (result > 255 || result <0)
				result = result > 255 ? (float)255 : (float)0;

			_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j] = result;

			temp=0;
		}
	}

	//adjust
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
			iTempValue = _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j];
			_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i - numMinRows][j - numMinCols - HEADER] = iTempValue;

		}
	}

	_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealRowNumber = numMaxRows - numMinRows;
	_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealColumnNumber = numMaxCols - numMinCols - HEADER;

	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("AcqImgNoFinger", "Pass"));
}


void Ts_AcqImgNoFinger::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}