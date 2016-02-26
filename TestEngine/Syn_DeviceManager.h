#pragma once

#include <stdint.h>
#include <vector>

#include "Syn_Site.h"

class Syn_DeviceManager
{
public:
	Syn_DeviceManager();
	~Syn_DeviceManager();

	uint32_t Open();
	
	void GetSerialNumberList(std::vector<uint32_t> &serialNumberList);

	uint32_t UpdateFirmware(std::vector<Syn_Site*> &listOfSite);
	
	uint32_t UpdateADCOffsets(Syn_Site* &pSiteInstance, uint32_t nVdd, uint32_t nVio, uint32_t nVled, uint32_t nVddh, uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS]);
	
	uint32_t Close();

private:
	uint32_t *_pDeviceSerNumArray;
	int32_t _deviceCount;

	bool _OpenFlag;
};