// ManagedTestEngine.h

#pragma once

#include "Syn_DeviceManager.h"
#include "Syn_Site.h"

using namespace System;
using namespace System::Collections::Generic;

namespace SynapticsModuleTest {

	public ref class TestSite
	{
	public:

		/*site initiazlization*/
		uint32_t Init();
		uint32_t PreCheck();
		uint32_t PatchTest();
		uint32_t WithoutStimulusTest();
		uint32_t WithStimulusTest();
		uint32_t ImageQualityTest();
		uint32_t PostCheck();

		/*get number of test steps finished.*/
		//uint32_t GetNumSteps(uint16_t numSteps);
		
		/*get the test result of specified step number.*/
		//uint32_t GetTestResult(uint16_t stepNo, Syn_DutTestResult &oTestResult);

		/*get site infomation of siteNo, state, etc.*/
		//void GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo);

		/*get the current state of site*/
		//void GetState(SiteState &oSiteState);

		/*Stop the test*/
		//uint32_t Stop();

		/*close this site*/
		//uint32_t Close();

	private:
		Syn_Site * _site;
		SiteState * _siteState;
	};

	
	public ref class DeviceManager
	{
	public:

		uint32_t Open();
		List<UInt32>^ GetSerialNumberList();
		uint32_t UpdateFirmware();
		uint32_t UpdateADCOffsets();
		uint32_t Close();

	private:
		Syn_DeviceManager * _deviceManager;
		std::vector<uint32_t> * _serialNumberList;
	};
}