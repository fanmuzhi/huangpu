#include "Ts_FinalizationStep.h"

#include <stdio.h>
#include <io.h>

Ts_FinalizationStep::Ts_FinalizationStep(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_FinalizationStep::~Ts_FinalizationStep()
{
}

void Ts_FinalizationStep::SetUp()
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
}

void Ts_FinalizationStep::Execute()
{
	if (0 == _pSyn_Dut->_pSyn_DutTestResult->_binCodes.size())
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sPass);
		_pSyn_Dut->b_Pass = true;
	}
	else
	{
		_pSyn_Dut->b_Pass = false;
	}
}

void Ts_FinalizationStep::ProcessData()
{
	//WriteLog();
}

void Ts_FinalizationStep::CleanUp()
{
	PowerOff();
}

//void Ts_FinalizationStep::WriteLog()
//{
//	/*for (int i = 0; i < DUT_SER_NUM_SIZE; i++)
//	{
//		std::string strTempSerialNumber = to_string(_pSyn_Dut->_pSyn_DutTestResult->_arSerialNum[i]);
//		strTempSerialNumber.
//	}*/
//	/*std::vector<std::string> lsitOfFileName;
//	long handle;
//	struct _finddata_t fileinfo;
//	handle = _findfirst("*.csv", &fileinfo);
//	if (-1 == handle)
//		return;
//	while (!_findnext(handle, &fileinfo))
//	{
//		lsitOfFileName.push_back(fileinfo.name);
//	}
//	_findclose(handle);   */                                 
//
//	std::string stringFilePath("MTLog_Site1.csv");
//	FILE *pFile = fopen(stringFilePath.c_str(), "a");
//	if (NULL == pFile)
//	{
//		return;
//	}
//
//	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");
//	fprintf(pFile, "MTLog_Site_\n");
//
//	//Put in part number.
//	fprintf(pFile, "Part Number,%s\n", "DebugVersion");
//
//	//fprintf(pFile, "ConfigFile,%s\n", _pSyn_Dut->_pSyn_DutTestInfo-);
//	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");
//	
//	fprintf(pFile, "\n---------------------\n");
//	const time_t t = time(NULL);
//	struct tm* current_time = localtime(&t);
//	fprintf(pFile, "Run %d,%s\n", "", asctime(current_time));
//
//	//InitlizationStep
//	fprintf(pFile, "Initialization, %s,%s ms\n", _pSyn_Dut->_pSyn_DutTestResult->_initResults.m_bPass?"Pass":"Fail", 0);
//
//	//Pixel Patch
//	fprintf(pFile, "\nPixel Patch, %s,%s ms\n", _pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_bPass?"Pass":"Fail", 0);
//	fprintf(pFile, ",,,");
//	for (int i = 0; i < (_pSyn_Dut->_pSyn_DutTestInfo->_pixelPatchInfo.m_nNumResBytes) / 4;i++)
//		fprintf(pFile, "%d,", *((uint32_t*)&_pSyn_Dut->_pSyn_DutTestResult->_pixelPatchResults.m_pResponse[i * 4]));
//	fprintf(pFile, "\n");
//
//	//Cablication
//	fprintf(pFile, "\nCalibration, %s,%d ms\n", _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass ? "Pass" : "Fail", 0);
//	// Stage1 LNA values from print patch
//	fprintf(pFile, ",,,Stage1");
//	for (int i = 0; i < _pSyn_Dut->_RowNumber; i++)
//	{
//		fprintf(pFile, ",%02X", _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch[i + _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx]);
//	}
//	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType == 0)
//	{
//		fprintf(pFile, "\n,,,Stage2");
//		for (int i = 0; i < _pSyn_Dut->_RowNumber; i++)
//		{
//			fprintf(pFile, ",%02X", _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets[i]);
//		}
//		fprintf(pFile, "\n");
//	}
//	else if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType == 1)
//	{
//		fprintf(pFile, "\n,,,Stage2 Used");
//		for (int i = 0; i<(_pSyn_Dut->_RowNumber) * (_pSyn_Dut->_ColumnNumber - 8); i++)
//		{
//			fprintf(pFile, ",%02X", _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets[i]);
//		}
//		fprintf(pFile, "\n");
//	}
//	else
//	{
//		fprintf(pFile, ",,,Stage2 Used,N/A\n");
//	}
//	// Stage2 OTP values	
//	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType == 1)
//	{
//		if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPGA_OOPP_count != 0)
//		{
//			fprintf(pFile, ",,,Stage2 OTP");
//			for (int i = 0; i< (NUM_PGA_OOPP_OTP_ROWS * (_pSyn_Dut->_ColumnNumber - 8)); i++)
//			{
//				fprintf(pFile, ",%02X", _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPGAOtpArray[i]);
//			}
//			
//			fprintf(pFile, "\n,,,Stage2 Variance Score,N/A\n");
//		}
//		else
//		{
//			fprintf(pFile, ",,,Stage2 OTP,N/A\n");
//			fprintf(pFile, ",,,Stage2 Variance Score,N/A\n");
//		}
//	}
//	fprintf(pFile, ",,,FlexId,%04X\n", _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nFlexId);
//
//	//Pegged Pixels Test
//	fprintf(pFile, "\nPegged Pixels Test,%s,%d ms,Rows,", _pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.m_bPass ? "Pass" : "Fail",0);
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_rows[i]);
//
//	fprintf(pFile, "\n,,,Columns,");
//	for (int i = 0; i<_pSyn_Dut->_ColumnNumber - HEADER - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_peggedPixelsResults.pegged_pixel_cols[i]);
//	fprintf(pFile, "\n");
//
//	//Floored Pixels Test
//	fprintf(pFile, "\nFloored Pixels Test,%s,%d ms,Rows,", _pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.m_bPass ? "Pass" : "Fail", "0ms");
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_pixel_rows[i]);
//
//	fprintf(pFile, "\n,,,Columns,");
//	for (int i = 0; i<_pSyn_Dut->_ColumnNumber - HEADER - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_flooredPixelsResults.floored_pixel_cols[i]);
//	fprintf(pFile, "\n");
//
//	//DRdy Test
//	fprintf(pFile, "\nDRdy Test,%s,%d ms\n", _pSyn_Dut->_pSyn_DutTestResult->_DRdyResults.m_bPass ? "Pass" : "Fail", 0);
//
//	//Consecutive Pixels Test
//	fprintf(pFile, "\nConsecutive Pixels Test,%s,%d ms,Pegged Rows,", _pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass ? "Pass" : "Fail", "0ms");
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_rows[i]);
//
//	fprintf(pFile, "\n,,,Floored Rows,");
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_rows[i]);
//
//	fprintf(pFile, "\n,,,Pegged Columns,");
//	for (int i = 0; i<_pSyn_Dut->_ColumnNumber - HEADER - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_pegged_cols[i]);
//
//	fprintf(pFile, "\n,,,Floored Columns,");
//	for (int i = 0; i<_pSyn_Dut->_ColumnNumber - HEADER - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_consecutivePixelsResults.consecutive_floored_cols[i]);
//	fprintf(pFile, "\n");
//
//	//Current Test
//	fprintf(pFile, "\nCurrent Test,%s,%d ms,", _pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass ? "Pass" : "Fail", 0);
//	//If the test was successful.
//	if (_pSyn_Dut->_pSyn_DutTestResult->_currentResults.bPass != 0)
//	{
//		fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA )/ 1000);
//		fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
//	}
//	else
//	{
//		fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqDigCurrent_uA) / 1000);
//		fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(_pSyn_Dut->_pSyn_DutTestResult->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
//	}
//
//	//SNR Test
//	fprintf(pFile, "\nSNR Test,%s,%d ms,", _pSyn_Dut->_pSyn_DutTestResult->_snrResults.bPass ? "Pass" : "Fail", 0);
//	fprintf(pFile, "Signal_Z1,Noise_Z1,SNR_Z1,Signal_Z2,Noise_Z2,SNR_Z2,Signal_Z3,Noise_Z3,SNR_Z3,Signal_Z4,Noise_Z4,SNR_Z4,Signal_Z5,Noise_Z5,SNR_Z5,Signal_Z6,Noise_Z6,SNR_Z6,Signal_OVERALL,Noise_OVERALL,SNR_OVERALL\n");
//	fprintf(pFile, ",,,");
//	for (int i = 0; i<REGIONS; i++)
//		fprintf(pFile, "%d,%f,%f,", _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SIGNAL[i], _pSyn_Dut->_pSyn_DutTestResult->_snrResults.NOISE[i], _pSyn_Dut->_pSyn_DutTestResult->_snrResults.SNR[i]);
//	fprintf(pFile, "\n");
//
//	//Pixel Uniformity Test
//	fprintf(pFile, "\nPixel Uniformity Test,%s,%d ms,", _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.bPass ? "Pass" : "Fail", 0);
//	fprintf(pFile, "Minimum Pixel,Maximum Pixel,Failing Pixel Count\n");
//	fprintf(pFile, ",,,%d,%d,%d,", _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nMinPixelValue, _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nMaxPixelValue, _pSyn_Dut->_pSyn_DutTestResult->_pixelResults.nCountAboveThreshold);
//	fprintf(pFile, "\n");
//
//	//Sharpness Test
//	fprintf(pFile, "\nSharpness Test,%s,%d ms\n", _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.bPass ? "Pass" : "Fail", 0);
//	fprintf(pFile, ",,,Variation(%%),Zone1,Zone2,Zone3,Overall\n");
//	fprintf(pFile, ",,,%f,%d,%d,%d,%d", _pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.percent, (int)_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[0], (int)_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[1], (int)_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[2], (int)_pSyn_Dut->_pSyn_DutTestResult->_SharpnessResults.SHARPNESS[3]);
//	fprintf(pFile, "\n");
//
//	//RxStandardDev Test
//	fprintf(pFile, "\nRxStandardDev Test,%s,%d ms", _pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.m_bPass ? "Pass" : "Fail", 0);
//	fprintf(pFile, "\n,,,Percent:,");
//	for (int i = 0; i<(_pSyn_Dut->_RowNumber - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim); i++)
//		fprintf(pFile, "%f,", _pSyn_Dut->_pSyn_DutTestResult->_RxStandardDevResults.percent[i]);
//	fprintf(pFile, "\n");
//
//	//Imperfections Test
//	fprintf(pFile, "\nImperfections Test,%s,%d ms,Along Rows,", _pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass ? "Pass" : "Fail",0);
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_rows[i]);
//	fprintf(pFile, "\n,,,Along Columns,");
//	for (int i = 0; i<_pSyn_Dut->_ColumnNumber - HEADER - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); i++)
//		fprintf(pFile, "%d,", _pSyn_Dut->_pSyn_DutTestResult->_imperfectionsTestResults.consecutive_pegged_cols[i]);
//	fprintf(pFile, "\n");
//
//	//Average No Finger & Average Finger
//	int numFrames = 30;
//	float temp = 0.0;
//	fprintf(pFile, "\nAverage No Finger\n");
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim); i++)
//	{
//		for (int j = 0; j<_pSyn_Dut->_ColumnNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); j++)
//		{
//			for (int k = 0; k<numFrames; k++)
//			{
//				temp += _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.m_arImagesWithoutStimulus[k].arr[i][j];
//			}
//			fprintf(pFile, "%.0f,", temp / numFrames);
//			temp = 0.0;
//		}
//		fprintf(pFile, "\n");
//	}
//	temp = 0.0;
//	fprintf(pFile, "\nAverage Finger\n");
//	for (int i = 0; i<_pSyn_Dut->_RowNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithStim); i++)
//	{
//		for (int j = 0; j<_pSyn_Dut->_ColumnNumber - (_pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim + _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim); j++)
//		{
//			for (int k = 0; k<numFrames; k++)
//			{
//				temp += _pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult.m_arImagesWithStimulus[k].arr[i][j];
//			}
//			fprintf(pFile, "%.0f,", temp / numFrames);
//			temp = 0.0;
//		}
//		fprintf(pFile, "\n");
//	}
//
//	fprintf(pFile, "\n,Bin Codes");
//	for (size_t i = 1; i <= _pSyn_Dut->_pSyn_DutTestResult->_binCodes.size(); i++)
//	{
//		fprintf(pFile, ",%s", (_pSyn_Dut->_pSyn_DutTestResult->_binCodes[i-1]).c_str());
//	}
//
//	fclose(pFile);
//}