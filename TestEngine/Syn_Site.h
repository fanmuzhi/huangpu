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

struct Syn_ADCBaseLineInfo
{
	uint32_t	_arAdcBaseLines[4][4];
	uint32_t	_nVdd;
	uint32_t	_nVio;
	uint32_t	_nVled;
	uint32_t	_nVddh;
};

struct Syn_TestResults
{
	std::string sTestName;
	std::string sResult;
	//std::string sTestStepName;
	//bool bPass;

	double snrValue[7];

	std::vector<std::string> listOfBinCodes;

	uint8_t arrImageNoFinger[200][200];
	int imageNoFingerRows;
	int imageNoFingerCols;

	uint8_t arrImageFinger[200][200];
	int imageFingerRows;
	int imageFingerCols;

	//Get ver info
	std::string sSensorSerialNumber;
	uint32_t   buildtime;        /* Unix-style build time, in seconds   *//*  from 1/1/1970 12:00 AM GMT         */
	uint32_t   buildnum;         /* build number                        */
	uint8_t    vmajor;           /* major version                       */
	uint8_t    vminor;           /* minor version                       */
	uint8_t    target;           /* target, e.g. VCSFW_TARGET_ROM       */
	uint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
	uint8_t    siliconrev;       /* silicon revision                    */
	uint8_t    formalrel;        /* boolean: non-zero -> formal release */
	uint8_t    platform;         /* Platform (PCB) revision             */
	uint8_t    patch;            /* patch level                         */
	uint8_t    serial_number[6]; /* 48-bit Serial Number                */
	uint8_t    security[2];      /* bytes 0 and 1 of OTP                */
	uint32_t   patchsig;         /* opaque patch signature              */
	uint8_t    iface;            /* interface type, see below           */
	uint8_t    otpsig[3];        /* OTP Patch Signature                 */
	uint16_t   otpspare1;        /* spare space                         */
	uint8_t    reserved;         /* reserved byte                       */
	uint8_t    device_type;      /* device type                         */

	//OTP content
	uint8_t arrBootSector0[64];
	uint8_t arrBootSector1[64];

	uint8_t arrMainSector0[2048];
	uint8_t arrMainSector1[2048];
};

class Syn_Site
{

public:

	enum SiteState{	Idle = 0x8001,TestDataReady,Running,Error,Closed };

	//static function
	static uint32_t CreateSiteInstance(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const AdcBaseLineInfo &iADCInfo, Syn_Site * &opSiteInstance);

	//
	static uint32_t CreateSiteInstance(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const Syn_ADCBaseLineInfo &iADCBaseLineInfo, Syn_Site * &opSiteInstance);

	/*need call Open function*/
	//Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath);

	~Syn_Site();

	/*site open*/
	//uint32_t Init();
	uint32_t Open();

	/*fixed scriptID and scriptName in GUI. */
	uint32_t ExecuteScript(uint8_t scriptID);                              

	/*get number of test steps finished.*/
	//uint32_t GetNumSteps(uint16_t numSteps);                               

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

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };
	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };
	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oConfig){ oConfig = _SysConfig; };



	//just for important results
	uint32_t GetResults(Syn_TestResults * &opTestResults);

	bool GetPassResult(std::string sTestStepName);

	void GetSensorSerialNumber(std::string &osSensorSerialNumber);

	void GetTestStepList(std::vector<std::string> &olistOfTestStepName);

	void Write_Log(std::string sFolderPath);

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