#pragma once

//local
#include "Syn_SPCCtrl.h"

class Syn_MPC04Ctrl :
	public Syn_SPCCtrl
{
public:
	Syn_MPC04Ctrl(uint32_t iSerialNumber);
	~Syn_MPC04Ctrl();

	virtual void SetLeds(bool bGreen, bool bRed, bool bAmber);

	virtual void GetCurrentSenseValues(uint16_t gainIdx, uint16_t oversample, uint32_t arCurrentSenseValues[4]);

};

