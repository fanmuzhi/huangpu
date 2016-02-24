#pragma once

#include <stdint.h>
#include <vector>

class Syn_DeviceManager
{
public:
	Syn_DeviceManager();
	~Syn_DeviceManager();

	uint32_t Open();
	void GetSerialNumberList(std::vector<uint32_t> &serialNumberList);
	uint32_t UpdateFirmware();
	uint32_t UpdateADCOffsets();
	uint32_t Close();

private:
	uint32_t *_pDeviceSerNumArray;
	int32_t _deviceCount;

	bool _OpenFlag;
};