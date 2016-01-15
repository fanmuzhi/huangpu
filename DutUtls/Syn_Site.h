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

struct Syn_OTPTestInfo
{
	uint8_t _BootSector0Array[BS0_SIZE];
	uint8_t _BootSector1Array[BS1_SIZE];
	
	uint8_t _MainSector0Array[MS1_SIZE];//MS0_SIZE
	uint8_t _MainSector1Array[MS1_SIZE];

	Syn_TestState _TestState;

	std::string _strErrorMessage;

	uint32_t _uiSerialNumber;
	unsigned int _uiSiteNumber;

};

struct Syn_DutTestInfo
{
	InitializationInfo			m_initInfo;
	CalibrationInfo				m_calibrationInfo;
	AcquireFPSInfo				m_acquireFpsInfo;
	PeggedPixelsInfo			m_peggedPixelsInfo;
	CurrentInfo					m_currentInfo;
	OpensShortsInfo				m_opensShortsInfo;
	FlooredPixelsInfo			m_flooredPixelsInfo;
	DeltaPixelInfo				m_deltaPixelsInfo;
	ConsecutivePixelsInfo		m_consecutivePixelsInfo;
	PixelInfo					m_pixelInfo;
	PixelPatchInfo				m_pixelPatchInfo;
	SNRInfo						m_snrInfo;
	WofTestInfo					m_wofInfo;
};

struct Syn_DutTestResultInfo
{
	//InitializationInfo			m_initInfo;
	InitializationResults		m_initResults;
	//CalibrationInfo				m_calibrationInfo;
	CalibrationResults			m_calibrationResults;
	/*AcquireFPSInfo				m_acquireFpsInfo;
	AcquireFPSResults			m_acquireFpsResults;
	PeggedPixelsInfo			m_peggedPixelsInfo;
	PeggedPixelsResults			m_peggedPixelsResults;
	CurrentInfo					m_currentInfo;
	CurrentResults				m_currentResults;
	OpensShortsInfo				m_opensShortsInfo;
	OpensShortsResults			m_opensShortsResults;
	FlooredPixelsInfo			m_flooredPixelsInfo;
	FlooredPixelsResults		m_flooredPixelsResults;
	DeltaPixelInfo				m_deltaPixelsInfo;
	DeltaPixelResults			m_deltaPixelsResults;
	ConsecutivePixelsInfo		m_consecutivePixelsInfo;
	ConsecutivePixelsResults	m_consecutivePixelsResults;
	PixelInfo					m_pixelInfo;
	PixelResults				m_pixelResults;
	PixelPatchInfo				m_pixelPatchInfo;
	PixelPatchResults			m_pixelPatchResults;
	SNRInfo						m_snrInfo;
	SNRResults					m_snrResults;
	WofTestInfo					m_wofInfo;
	WofTestResults				m_wofResults;
	SCM_WofTestInfo				m_SCM_wofInfo;
	SCM_WofTestResults			m_SCM_wofResults;
	WoVarInfo					m_woVarInfo;
	WoVarResults				m_woVarResults;
	DRdyInfo					m_DRdyInfo;
	DRdyResults					m_DRdyResults;
	UsbCommInfo					m_usbCommInfo;
	UsbCommResults				m_usbCommResults;
	SpiFlashInfo				m_spiFlashInfo;
	SpiFlashResults				m_spiFlashResults;
	BtnTestInfo				    m_btnTestWithoutStimInfo;
	BtnTestResults			    m_btnTestWithoutStimResults;
	BtnTestInfo				    m_btnTestWithStimInfo;
	BtnTestResults			    m_btnTestWithStimResults;
	SharpnessInfo				m_SharpnessInfo;
	SharpnessResults			m_SharpnessResults;
	ReadDutAdcInfo				m_ReadDutAdcInfo;
	ReadDutAdcResults			m_ReadDutAdcResults;
	ImperfectionsTestInfo       m_imperfectionsTestInfo;
	ImperfectionsTestResults    m_imperfectionsTestResults;
	RxStandardDevInfo			m_RxStandardDevInfo;
	RxStandardDevResults		m_RxStandardDevResults;
	OscTrimInfo					m_OscTrimInfo;
	OscTrimResults				m_OscTrimResults;
	SlowOscInfo					m_SlowOscInfo;
	SlowOscResults				m_SlowOscResults;
	RAMTestInfo					m_RAMTestInfo;
	RAMTestResults				m_RAMTestResults;
	RetainModeInfo				m_retainModeInfo;
	RetainModeResults			m_retainModeResults;
	WofLowPowerInfo				m_wofLowPowerInfo;
	WofLowPowerResults			m_wofLowPowerResults;
	AdcBaseLineInfo				m_adcBaselineInfo;
	SecurityStepInfo			m_securityStepInfo;
	SecurityStepResults			m_securityStepResults;
	ProductIDTestInfo			m_productIdTestInfo;
	ProductIDTestResults		m_productIdTestResults;
	AFETestInfo					m_AFETestInfo;
	AFETestResults				m_AFETestResults;
	LEDTestInfo					m_LEDTestInfo;
	LEDTestResults				m_LEDTestResults;
	SdkBaselineTestInfo		    m_SdkBaselineInfo;
	SdkBaselineTestResults	    m_SdkBaselineResults;
	SpiOwnershipInfo		    m_SpiOwnershipInfo;
	SpiOwnershipResults			m_SpiOwnershipResults;*/
};

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	void Run();

	void GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo);

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };

	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };

	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oSysConfig){ oSysConfig = _SysConfig; };

	inline void SetSysConfig(Syn_SysConfig iSysConfig){ _SysConfig = iSysConfig; };


protected:

	//function

	//parse args test
	void InitDutTestInfo();

	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_OTPTestInfo _OTPTestInfo;

	unsigned int _iSiteNumber;

	uint32_t _uiSerialNumber;

	Syn_DutTestInfo			_DutTestInfo;
	Syn_DutTestResultInfo	_DutTestResultInfo;
};

#endif // SYN_SITE_H
