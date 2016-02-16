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

enum ProjectType
{
	Viper1 = 0x1001,
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

	static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance);

	//function
	void InitData(Syn_SysConfig &sysConfig);

	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

	bool FindPatch(std::string patchName, Syn_PatchInfo &patchInfo);

	inline void SetPatchInfo(vector<Syn_PatchInfo> ilistOfPatchInfo){ _listOfPatchInfo = ilistOfPatchInfo; };

	//variable
	Syn_DutTestInfo			*_pSyn_DutTestInfo;

	Syn_DutTestResult		*_pSyn_DutTestResult;

	vector<Syn_PatchInfo>	_listOfPatchInfo;

	uint16_t		_uiDutpwrVdd_mV;//DutpwrVdd_mV
	uint16_t		_uiDutpwrVio_mV;//DutpwrVio_mV
	uint16_t		_uiDutpwrVled_mV;//DutpwrVled_mV
	uint16_t		_uiDutpwrVddh_mV;//DutpwrVddh_mV

	uint16_t		_RowNumber;//NumRows
	uint16_t		_ColumnNumber;//NumCols

	ProjectType		_eProjectType;

};