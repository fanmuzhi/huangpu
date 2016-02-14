#pragma once

//Local:DutCtrl
#include "Syn_DutCtrl.h"
#include "Syn_Config.h" 

extern "C" {
#include "SYN_TestUtils.h"
}

//std
#include <string>
#include <vector>
using namespace std;

enum ProjectType {
	Viper1 = 0x1000,
	Viper2,
	Metallica
};

struct Syn_DutTestInfo
{
	//Syn_SiteInfo				_siteInfo;
	InitializationInfo			_initInfo;
	CalibrationInfo				_calibrationInfo;
	AcquireFPSInfo				_acquireFpsInfo;
	PeggedPixelsInfo			_peggedPixelsInfo;
	CurrentInfo					_currentInfo;
	OpensShortsInfo				_opensShortsInfo;
	FlooredPixelsInfo			_flooredPixelsInfo;
	DeltaPixelInfo				_deltaPixelsInfo;
	ConsecutivePixelsInfo		_consecutivePixelsInfo;
	PixelInfo					_pixelInfo;
	PixelPatchInfo				_pixelPatchInfo;
	SNRInfo						_snrInfo;
	WofTestInfo					_wofInfo;
	SCM_WofTestInfo				_SCM_wofInfo;//two SCM_WofTestInfo;with f and without f
	WoVarInfo					_woVarInfo;
	DRdyInfo					_DRdyInfo;
	UsbCommInfo					_usbCommInfo;
	SpiFlashInfo				_spiFlashInfo;
	BtnTestInfo				    _btnTestWithoutStimInfo;
	BtnTestInfo				    _btnTestWithStimInfo;
	SharpnessInfo				_SharpnessInfo;
	ReadDutAdcInfo				_ReadDutAdcInfo;
	ImperfectionsTestInfo       _imperfectionsTestInfo;
	RxStandardDevInfo			_RxStandardDevInfo;
	OscTrimInfo					_OscTrimInfo;
	SlowOscInfo					_SlowOscInfo;
	RAMTestInfo					_RAMTestInfo;
	RetainModeInfo				_retainModeInfo;
	WofLowPowerInfo				_wofLowPowerInfo;
	AdcBaseLineInfo				_adcBaselineInfo;
	SecurityStepInfo			_securityStepInfo;
	ProductIDTestInfo			_productIdTestInfo;
	AFETestInfo					_AFETestInfo;
	LEDTestInfo					_LEDTestInfo;
	SdkBaselineTestInfo		    _SdkBaselineInfo;
	SpiOwnershipInfo		    _SpiOwnershipInfo;
	OTPInfo						_otpInfo;
	GetVerInfo					_getVerInfo;
};

struct Syn_DutTestResult
{
	InitializationResults		_initResults;
	CalibrationResults			_calibrationResults;
	AcquireFPSResults			_acquireFpsResults;
	PeggedPixelsResults			_peggedPixelsResults;
	CurrentResults				_currentResults;
	OpensShortsResults			_opensShortsResults;
	FlooredPixelsResults		_flooredPixelsResults;
	DeltaPixelResults			_deltaPixelsResults;
	ConsecutivePixelsResults	_consecutivePixelsResults;
	PixelResults				_pixelResults;
	PixelPatchResults			_pixelPatchResults;
	SNRResults					_snrResults;
	WofTestResults				_wofResults;
	SCM_WofTestResults			_SCM_wofResults;
	WoVarResults				_woVarResults;
	DRdyResults					_DRdyResults;
	UsbCommResults				_usbCommResults;
	SpiFlashResults				_spiFlashResults;
	BtnTestResults			    _btnTestWithoutStimResults;
	BtnTestResults			    _btnTestWithStimResults;
	SharpnessResults			_SharpnessResults;
	ReadDutAdcResults			_ReadDutAdcResults;
	ImperfectionsTestResults    _imperfectionsTestResults;
	RxStandardDevResults		_RxStandardDevResults;
	OscTrimResults				_OscTrimResults;
	SlowOscResults				_SlowOscResults;
	RAMTestResults				_RAMTestResults;
	RetainModeResults			_retainModeResults;
	WofLowPowerResults			_wofLowPowerResults;
	SecurityStepResults			_securityStepResults;
	ProductIDTestResults		_productIdTestResults;
	AFETestResults				_AFETestResults;
	LEDTestResults				_LEDTestResults;
	SdkBaselineTestResults	    _SdkBaselineResults;
	SpiOwnershipResults			_SpiOwnershipResults;
};

class Syn_Dut
{
public:

	enum { kPgaCalTypeOneOffsetPerRow, kPgaCalTypeOneOffsetPerPixel };

	Syn_Dut();
	virtual ~Syn_Dut();

	//Gobal Function:Create a Dut Instance
	static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance);
	//static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance, uint32_t iSerialNumber, DutController iDutControllerType);

	//Set and Get DutCtrl
	bool SetDutCtrl(Syn_DutCtrl * ipSyn_DutCtrl);
	Syn_DutCtrl * GetDutCtrl();

	//virtual function
	virtual void GetProjectType(ProjectType &oProjectType) = 0;
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;

	virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult) = 0;

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

	//list all test itmes here
	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	bool ReadOTP(uint8_t* oarMS0, int iSize);

	bool Calibration(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

	bool CheckDUTexists();

	void CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults);

	void GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows);






	inline void SetPatchInfo(vector<Syn_PatchInfo> ilistOfPatchInfo){ _listOfPatchInfo = ilistOfPatchInfo; };



	//
	void InitDutTestInfo(Syn_SysConfig &sysConfig);

	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

	bool FindPatch(std::string patchName, Syn_PatchInfo &patchInfo);

	Syn_DutTestInfo			*_pSyn_DutTestInfo;

	Syn_DutTestResult		*_pSyn_DutTestResult;

	vector<Syn_PatchInfo>	_listOfPatchInfo;


private:

	//bool FindPatch(std::string patchName, Syn_PatchInfo &patchInfo);

protected:
	
	Syn_DutCtrl *_pSyn_DutCtrl;

	//vector<Syn_PatchInfo> _listOfPatchInfo;

};