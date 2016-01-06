#pragma once

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"

//std
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

struct Syn_OTPTestInfo
{
	uint8_t _BootSector0Array[BS0_SIZE];
	uint8_t _BootSector1Array[BS1_SIZE];
	
	uint8_t _MainSector0Array[MS1_SIZE];//MS0_SIZE
	uint8_t _MainSector1Array[MS1_SIZE];
};

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	//void Run(uint8_t* arMS0, int iSize);
	void Run();

	void GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo);


	//thread Test
	bool TestGetValue(std::string &strTime);
	void TestSet();



private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;



	time_t _tm;
	int _iTestEndTag;

	Syn_OTPTestInfo _OTPTestInfo;
};

