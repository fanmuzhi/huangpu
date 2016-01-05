#pragma once

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"

//C++ std
#include <fstream>
#include <vector>
#include <string>

//C++ std::thread
#include <thread>

#include<time.h>

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	static bool	SiteThreadStart(void *vParam);

	void Run(uint8_t* arMS0, int iSize);

	//thread Test
	bool TestGetValue(std::string &strTime);
	void TestSet();



private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	//thread _syn_SiteThread;

	time_t _tm;
	int _iTestEndTag;
};

