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

//TestStep
#include "Syn_TestStep.h"
#include "Syn_TestStepFactory.h"

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

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	static bool RegisterLoggingConfig();


	void Run();				//main test entrance.

	void GetVersion();				//for debug.

	void ReadOTP();					//for debug.

	void Calibration();				//for debug

	void GetFingerprintImage();		//for debug
	void PowerOff();//debug


	void GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo);
	void GetTestInfo(Syn_DutTestInfo &oSyn_DutTestInfo);
	void GetTestResult(Syn_DutTestResult * &opSyn_DutTestResult);

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };

	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };

	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oSysConfig){ oSysConfig = _SysConfig; };

	inline void SetSysConfig(Syn_SysConfig iSysConfig){ _SysConfig = iSysConfig; };

private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_DutCtrl *_pSyn_DutCtrl;

	unsigned int _iSiteNumber;

	uint32_t _uiSerialNumber;

	Syn_SiteInfo		_siteInfo;
	Syn_DutTestInfo		_DutTestInfo;
	Syn_DutTestResult   *_pDutTestResult;

	//Syn_TestStep *_pTempTestStep;
};

#endif // SYN_SITE_H