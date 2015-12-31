#pragma once
#include "Syn_SPCCtrl.h"
class Syn_MPC04Ctrl :
	public Syn_SPCCtrl
{
public:
	Syn_MPC04Ctrl(uint32_t iSerialNumber);
	~Syn_MPC04Ctrl();
};

