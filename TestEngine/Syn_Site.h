#ifndef SYN_SITE_H
#define SYN_SITE_H

//DutUtls
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Exception.h"

//TestStep
#include "Syn_TestStep.h"
#include "Syn_TestStepFactory.h"

//Reference inc
extern "C" {
#include "SYN_TestUtils.h"
};

//#include <windows.h> 

enum SiteState
{
	Idle = 0x8001,
	TestDataReady,
	Running,
	Error,
	Closed,
	NotConnected//add by Jerry:2016_02_24(for display Site State) maybe delete at end
};

class Syn_Site
{

public:

	//static function
	static uint32_t CreateSiteInstance(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const AdcBaseLineInfo &iADCInfo, Syn_Site * &opSiteInstance);

	/*need call Open function*/
	//Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	~Syn_Site();

	/*site open*/
	//uint32_t Init();
	uint32_t Open();

	/*fixed scriptID and scriptName in GUI. */
	uint32_t ExecuteScript(uint8_t scriptID);                              

	/*get number of test steps finished.*/
	uint32_t GetNumSteps(uint16_t numSteps);                               

	uint32_t GetTestInfo(Syn_DutTestInfo * &opTestInfo);
	
	/*get the test result of specified step number.*/
	uint32_t GetTestResult(Syn_DutTestResult * &opTestResult);

	/*Debug interface for each test step*/
	uint32_t ExecuteTestStep(std::string sTestStepName);

	/*get the current state of site*/
	inline void GetState(SiteState &oSiteState){ oSiteState = _siteState; };

	/*Stop the test*/
	uint32_t Stop();

	/*close this site*/
	uint32_t Close();

	uint32_t GetRunTimeError(std::string &ostrErrorMessage)
	{ 
		ostrErrorMessage = _strErrorMessage;
		return _uiErrorFlag;
	};

	static bool RegisterLoggingConfig();

	//void Run();						//main test entrance.
	//uint32_t ReadOTP();					//for debug.
	//void GetVersion();				//for debug.
	//void Calibration();				//for debug
	//void GetFingerprintImage();		//for debug
	//void PowerOff();//debug

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };
	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };
	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oConfig){ oConfig = _SysConfig; };


	//add by Jerry:2016_02_24(set NotConnected Site Status) maybe delete at end
	//inline void SetSiteNotConnected(){ _siteState = NotConnected; };

private:

	/*need call Open function*/
	Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	/*site initiazlization*/
	uint32_t Init();

	//variable
	std::string _strConfigFilePath;

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_DutCtrl *_pSyn_DutCtrl;

	//id
	unsigned int _iSiteNumber;
	uint32_t _uiSerialNumber;

	SiteState _siteState;

	bool _stopFlag;

	uint32_t _uiErrorFlag;
	std::string _strErrorMessage;

	AdcBaseLineInfo _ADCInfo;

private:

	void RunScript(uint8_t scriptID);
	
	bool GetTestScriptInfo(uint8_t scriptID, Syn_TestScript &oTestScriptInfo);
};


#endif // SYN_SITE_H