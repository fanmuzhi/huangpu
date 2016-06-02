//windows api
//#include "windows.h"

//Local
#include "Syn_Dut.h"
#include "Syn_Exception.h"
#include "Syn_Vcsfw.h"

//std
#include <iostream>

//third-party
//#include "easylogging++.h"

Syn_Dut::Syn_Dut()
:_pSyn_DutTestInfo(NULL)
, _pSyn_DutTestResult(NULL)
{
}

Syn_Dut::~Syn_Dut()
{
	if (NULL != _pSyn_DutTestInfo)
	{
		delete _pSyn_DutTestInfo;
		_pSyn_DutTestInfo = NULL;
	}

	if (NULL != _pSyn_DutTestResult)
	{
		delete _pSyn_DutTestResult;
		_pSyn_DutTestResult = NULL;
	}

	_listOfPatchInfo.clear();
}

bool Syn_Dut::CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance)
{
	//opSyn_DutInstance = NULL;

	opSyn_DutInstance = new Syn_Dut();
	opSyn_DutInstance->_eProjectType = iType;

	return true;
}

void Syn_Dut::InitData(Syn_SysConfig &sysConfig, AdcBaseLineInfo &iAdcBaseLineInfo)
{
	bool rc(true);

	_uiDutpwrVdd_mV = sysConfig._uiDutpwrVdd_mV;
	_uiDutpwrVio_mV = sysConfig._uiDutpwrVio_mV;
	_uiDutpwrVled_mV = sysConfig._uiDutpwrVled_mV;
	_uiDutpwrVddh_mV = sysConfig._uiDutpwrVddh_mV;

	_RowNumber = sysConfig._uiNumRows;
	_ColumnNumber = sysConfig._uiNumCols;

	SetPatchInfo(sysConfig._listPatchInfo);

	if (NULL == _pSyn_DutTestInfo)
	{
		_pSyn_DutTestInfo = new Syn_DutTestInfo();
		_pSyn_DutTestInfo->_adcBaselineInfo = iAdcBaseLineInfo;
	}

	if (NULL == _pSyn_DutTestResult)
	{
		_pSyn_DutTestResult = new Syn_DutTestResult();
	}
	//_pSyn_DutTestResult->_sSensorSerialNumber.clear();
	_pSyn_DutTestResult->_binCodes.clear();

	int arrIncrement[4] = {3, 1, -1, -3};	//swap the value
	for (auto i = 0; i < BS0_SIZE; i++)
	{
		_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[i] = sysConfig._arrUserSpecifiedBS0[i + arrIncrement[i % 4]];
		_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS1[i] = sysConfig._arrUserSpecifiedBS1[i + arrIncrement[i % 4]];
	}

	//Init info
	_pSyn_DutTestInfo->_initInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_acqImgNoFingerInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_acqImgFingerInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_peggedPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_currentInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_opensShortsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_flooredPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_deltaPixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_consecutivePixelsInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_pixelInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_pixelPatchInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_snrInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithStimulus = false;
	_pSyn_DutTestInfo->_z0WofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithStimulus = false;
	_pSyn_DutTestInfo->_z1WofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_DutTestInfo->_TopSCM_wofInfo.m_bExecutedWithStimulus = false;
	_pSyn_DutTestInfo->_TopSCM_wofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_DutTestInfo->_BottomSCM_wofInfo.m_bExecutedWithStimulus = false;
	_pSyn_DutTestInfo->_BottomSCM_wofInfo.m_bExecutedWithoutStimulus = false;
	_pSyn_DutTestInfo->_woVarInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_usbCommInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_spiFlashInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_btnTestWithoutStimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_btnTestWithStimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SharpnessInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_imperfectionsTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_RxStandardDevInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_OscTrimInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SlowOscInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_RAMTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_retainModeInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_wofLowPowerInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_adcBaselineInfo.m_bExecuted = true;
	_pSyn_DutTestInfo->_securityStepInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_productIdTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_AFETestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_LEDTestInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SdkBaselineInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_SpiOwnershipInfo.m_bExecuted = false;
	_pSyn_DutTestInfo->_otpCheckInfo._bExecuted = false;
	_pSyn_DutTestInfo->_huaweiIqTestInfo._bExecuted = false;

	_pSyn_DutTestResult->_initResults.m_bPass = 1;
	_pSyn_DutTestResult->_calibrationResults.m_bPass = 1;
	//_pSyn_DutTestResult->_acqImgNoFingerResult.m_bPass = 1;
	//_pSyn_DutTestResult->_acqImgFingerResult.m_bPass = 1;
	_pSyn_DutTestResult->_peggedPixelsResults.m_bPass = 1;
	_pSyn_DutTestResult->_currentResults.bPass = 1;
	_pSyn_DutTestResult->_opensShortsResults.m_bPass = 1;
	_pSyn_DutTestResult->_flooredPixelsResults.m_bPass = 1;
	_pSyn_DutTestResult->_deltaPixelsResults.m_bPass = 1;
	_pSyn_DutTestResult->_consecutivePixelsResults.m_bPass = 1;
	_pSyn_DutTestResult->_pixelResults.bPass = 1;
	_pSyn_DutTestResult->_pixelPatchResults.m_bPass = 1;
	_pSyn_DutTestResult->_snrResults.bPass = 1;
	_pSyn_DutTestResult->_z0WofResults.m_bPass = 1;
	_pSyn_DutTestResult->_z1WofResults.m_bPass = 1;
	_pSyn_DutTestResult->_TopSCM_wofResults.m_bPass = 1;
	_pSyn_DutTestResult->_BottomSCM_wofResults.m_bPass = 1;
	_pSyn_DutTestResult->_woVarResults.m_bPass = 1;
	_pSyn_DutTestResult->_DRdyResults.m_bPass = 1;
	_pSyn_DutTestResult->_usbCommResults.m_bPass = 1;
	_pSyn_DutTestResult->_spiFlashResults.m_bPass = 1;
	_pSyn_DutTestResult->_btnTestWithoutStimResults.m_bPass = 1;
	_pSyn_DutTestResult->_btnTestWithStimResults.m_bPass = 1;
	_pSyn_DutTestResult->_SharpnessResults.bPass = 1;
	_pSyn_DutTestResult->_ReadDutAdcResults.m_bPass = 1;
	_pSyn_DutTestResult->_imperfectionsTestResults.m_bPass = 1;
	_pSyn_DutTestResult->_RxStandardDevResults.m_bPass = 1;
	_pSyn_DutTestResult->_OscTrimResults.m_bPass = 1;
	_pSyn_DutTestResult->_SlowOscResults.m_bPass = 1;
	_pSyn_DutTestResult->_RAMTestResults.bPass = 1;
	_pSyn_DutTestResult->_retainModeResults.m_bPass = 1;
	_pSyn_DutTestResult->_wofLowPowerResults.m_bPass = 1;
	_pSyn_DutTestResult->_securityStepResults.m_bPass = 1;
	_pSyn_DutTestResult->_productIdTestResults.m_bPass = 1;
	_pSyn_DutTestResult->_AFETestResults.m_bPass = 1;
	_pSyn_DutTestResult->_LEDTestResults.m_bPass = 1;
	_pSyn_DutTestResult->_SdkBaselineResults.m_bPass = 1;
	_pSyn_DutTestResult->_SpiOwnershipResults.m_bPass = 1;
	_pSyn_DutTestResult->_otpCheckResult._bPass = 1;

	//time ms
	_pSyn_DutTestResult->_z0WofResults.m_elapsedtime = 0;
	_pSyn_DutTestResult->_z1WofResults.m_elapsedtime = 0;
	_pSyn_DutTestResult->_TopSCM_wofResults.m_elapsedtime = 0;
	_pSyn_DutTestResult->_BottomSCM_wofResults.m_elapsedtime = 0;

}


bool Syn_Dut::FindPatch(std::string patchName, Syn_PatchInfo &patchInfo)
{
	bool IsExists(false);
	for (size_t i = 1; i <= _listOfPatchInfo.size(); i++)
	{
		if (patchName == _listOfPatchInfo[i - 1]._strXepatchName)
		{
			patchInfo = _listOfPatchInfo[i - 1];
			IsExists = true;
			break;
		}
	}
	return IsExists;
}