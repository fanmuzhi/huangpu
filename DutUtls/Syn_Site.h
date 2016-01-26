#ifndef SYN_SITE_H
#define SYN_SITE_H

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"

//std
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

//Reference inc
extern "C" {
#include "SYN_TestUtils.h"
};

enum Syn_TestState{TestReady = 0xAEF0,TestRunning,TestOK,TestError,TestFailed};

struct Syn_SiteInfo
{
	Syn_TestState	_TestState;			//states for site state machine
	std::string		_strErrorMessage;	//error message of current site.
	uint32_t		_uiSerialNumber;	//MPC04 serial number of current site.
	unsigned int	_uiSiteNumber;		//site number of current site.
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

	/*InitializationResults		_initResults;
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
	SpiOwnershipResults			_SpiOwnershipResults;*/
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

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	static bool RegisterLoggingConfig();


	void Run();				//main test entrance.

	void GetVersion();		//for debug.

	void ReadOTP();			//for debug.

	void GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo);
	void GetTestInfo(Syn_DutTestInfo &oSyn_DutTestInfo);
	void GetTestResult(Syn_DutTestResult * &opSyn_DutTestResult);

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };

	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };

	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oSysConfig){ oSysConfig = _SysConfig; };

	inline void SetSysConfig(Syn_SysConfig iSysConfig){ _SysConfig = iSysConfig; };


protected:		
	//parse args test
	void InitDutTestInfo();
	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	unsigned int _iSiteNumber;

	uint32_t _uiSerialNumber;

	Syn_SiteInfo		_siteInfo;
	Syn_DutTestInfo		_DutTestInfo;
	//Syn_DutTestResult   _DutTestResult;

	//Syn_DutTestInfo		*_pDutTestInfo;
	Syn_DutTestResult   *_pDutTestResult;

};

#endif // SYN_SITE_H