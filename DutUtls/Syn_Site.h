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

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	void Run();

	void GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo);

	inline void SetSiteNumber(unsigned int iSiteNumber){ _iSiteNumber = iSiteNumber; };

	inline void GetSiteNumber(unsigned int &oSiteNumber){ oSiteNumber = _iSiteNumber; };

	inline void GetSerialNumber(uint32_t &oSerialNumber){ oSerialNumber = _uiSerialNumber; };

	inline void GetSysConfig(Syn_SysConfig &oSysConfig){ oSysConfig = _SysConfig; };

	inline void SetSysConfig(Syn_SysConfig iSysConfig){ _SysConfig = iSysConfig; };


	//xml test
	static bool ParseSysConfigXML(std::string strConfigFilePath, Syn_SysConfig &oSyn_SysConfig);

	static void ConvertAsciiToBinary(const std::string strAsciiValue, uint8_t *pDst, int nDstSize);


private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_OTPTestInfo _OTPTestInfo;

	unsigned int _iSiteNumber;

	uint32_t _uiSerialNumber;
};

#endif // SYN_SITE_H
