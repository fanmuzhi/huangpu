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
	Closed
};

struct Syn_SiteInfo
{
	SiteState		_siteState;			//states for site state machine
	std::string		_strErrorMessage;	//error message of current site.
	uint32_t		_uiSerialNumber;	//MPC04 serial number of current site.
	unsigned int	_uiSiteNumber;		//site number of current site.
};

class Syn_Site
{
public:

	/*need call Init function*/
	Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	~Syn_Site();

	/*site initiazlization*/
	uint32_t Init();

	/*fixed scriptID and scriptName in GUI. */
	uint32_t ExecuteScript(uint8_t scriptID);                              

	/*get number of test steps finished.*/
	uint32_t GetNumSteps(uint16_t numSteps);                               

	uint32_t GetTestInfo(Syn_DutTestInfo * &opTestInfo);
	
	/*get the test result of specified step number.*/
	uint32_t GetTestResult(Syn_DutTestResult * &opTestResult);

	/*get site infomation of siteNo, state, etc.*/
	void GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo);

	/*get the current state of site*/
	inline void GetState(SiteState &oSiteState){ oSiteState = _sitState; };

	/*Stop the test*/
	uint32_t Stop();

	/*close this site*/
	uint32_t Close();

	uint32_t GetRunTimeError(std::string &ostrErrorMessage)
	{ 
		ostrErrorMessage = _strErrorMessage;
		return _uiErrorFlag;
	};

	void RunScript(uint8_t scriptID);




	//static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	static bool RegisterLoggingConfig();

	void Run();						//main test entrance.
	void GetVersion();				//for debug.
	void ReadOTP();					//for debug.
	void Calibration();				//for debug
	void GetFingerprintImage();		//for debug
	void PowerOff();//debug

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };

	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };

	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oSysConfig){ oSysConfig = _SysConfig; };

	inline void SetSysConfig(Syn_SysConfig iSysConfig){ _SysConfig = iSysConfig; };


private:

	std::string _strConfigFilePath;

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_DutCtrl *_pSyn_DutCtrl;

	unsigned int _iSiteNumber;

	uint32_t _uiSerialNumber;

	Syn_SiteInfo		_siteInfo;

	SiteState _sitState;

	bool _stopFlag;

	bool GetTestScriptInfo(uint8_t scriptID, Syn_TestScript &oTestScriptInfo);
	uint32_t _uiErrorFlag;
	std::string _strErrorMessage;
};


#endif // SYN_SITE_H