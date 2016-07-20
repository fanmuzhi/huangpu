#pragma once

//DutUtls
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Exception.h"
#include "Syn_TestUtils.h"

//TestStep
#include "Syn_TestStep.h"
#include "Syn_TestStepFactory.h"

//stat
#include <sys/stat.h>

class Syn_Site
{

public:

	enum SiteState{ Idle = 0x8001, TestDataReady, Running, Error, Closed };

	enum ExcuteType{ All = 0x6001, Setup, Excute, ProcessData, Cleanup};

	//static function
	static uint32_t CreateSiteInstance(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const AdcBaseLineInfo &iADCInfo, Syn_Site * &opSiteInstance);

	/*need call Open function*/
	//Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	~Syn_Site();

	/*site open*/
	uint32_t Open();

	uint32_t GetTestInfo(Syn_DutTestInfo * &opTestInfo);

	/*get the test result of specified step number.*/
	uint32_t GetTestResult(Syn_DutTestResult * &opTestResult);

	/*Debug interface for each test step*/
	uint32_t ExecuteTestStep(std::string sTestName, ExcuteType Type = Syn_Site::All);

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

	//static bool RegisterLoggingConfig();

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };
	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };
	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oConfig){ oConfig = _SysConfig; };

	void GetTestStepList(std::vector<std::string> &olistOfTestStepName);

	bool WriteLog(std::string sFolderPath, std::string sFileName = "");

private:

	/*need call Open function*/
	Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	/*site initiazlization*/
	uint32_t Init();

	bool FileExists(const std::string& name);

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
};

