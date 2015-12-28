#pragma once
#include "Syn_DutCtrl.h"
class Syn_SPCCtrl :
	public Syn_DutCtrl
{
public:
	Syn_SPCCtrl();
	virtual ~Syn_SPCCtrl();

	virtual bool Init();

	virtual bool PowerOn(uint8_t *pDst, int numBytes);

	virtual bool FpGetVersion(uint8_t *pDst, int numBytes);

	virtual bool PowerOff();

};

