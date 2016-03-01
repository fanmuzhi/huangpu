#pragma once

#include <stdint.h>
#include <vector>

//Reference inc
extern "C" {
#include "SYN_TestUtils.h"
};

class Syn_DeviceManager
{
public:
	Syn_DeviceManager();
	~Syn_DeviceManager();

	uint32_t Open();
	
	void GetSerialNumberList(std::vector<uint32_t> &serialNumberList);

	uint32_t UpdateFirmware();
	
	uint32_t UpdateADCOffsets(uint32_t uiSerialNumber, uint32_t nVdd, uint32_t nVio, uint32_t nVled, uint32_t nVddh, uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS]);
	
	uint32_t SetLeds(uint32_t uiSerialNumber);

	uint32_t Close();

private:
	uint32_t *_pDeviceSerNumArray;
	int32_t _deviceCount;

	bool _OpenFlag;
};