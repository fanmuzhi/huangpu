// This is the main DLL file.

#include "stdafx.h"

#include "ManagedTestEngine.h"

namespace SynapticsModuleTest{
	uint32_t TestSite::Init()
	{
		return _site->Init();
	}
	uint32_t TestSite::PreCheck()
	{
		return _site->ExecuteScript(1);
	}
	uint32_t TestSite::PatchTest()
	{
		return _site->ExecuteScript(2);
	}
	uint32_t TestSite::WithoutStimulusTest()
	{
		return _site->ExecuteScript(3);
	}
	uint32_t TestSite::WithStimulusTest()
	{
		return _site->ExecuteScript(4);
	}
	uint32_t TestSite::ImageQualityTest()
	{
		return _site->ExecuteScript(5);
	}
	uint32_t TestSite::PostCheck()
	{
		return _site->ExecuteScript(6);
	}

	//uint32_t TestSite::GetNumSteps(uint16_t numSteps)
	//{
	//	return _site->GetNumSteps(numSteps);
	//}
	//	
	//uint32_t TestSite::GetTestResult(uint16_t stepNo, Syn_DutTestResult &oTestResult)
	//{
	//	return _site->GetTestResult(stepNo, oTestResult);
	//}

	//void TestSite::GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo)
	//{
	//	return _site->GetSiteInfo(oSyn_SiteInfo);
	//}

	//void TestSite::GetState(SiteState &oSiteState)
	//{
	//	return _site->GetState(oSiteState);
	//}

	//uint32_t TestSite::Stop()
	//{
	//	return _site->Stop();
	//}

	//uint32_t TestSite::Close()
	//{
	//	return _site->Close();
	//}


	//Device Manager

	uint32_t DeviceManager::Open()
	{
		return _deviceManager->Open();
	}
	List<UInt32>^ DeviceManager::GetSerialNumberList()
	{
		List<UInt32>^ list;
		_deviceManager->GetSerialNumberList(*_serialNumberList);

		for (auto i = 0; i <= _serialNumberList->size(); i++)
		{
			list->Add((*_serialNumberList)[i]);
		}
		return list;
	}

	uint32_t DeviceManager::UpdateFirmware()
	{
		return _deviceManager->UpdateFirmware();
	}

	uint32_t DeviceManager::UpdateADCOffsets()
	{
		return _deviceManager->UpdateADCOffsets();
	}
	uint32_t DeviceManager::Close()
	{
		return _deviceManager->Close();
	}
}

