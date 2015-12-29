//local
#include "Syn_Site.h"

//std
#include <iostream>

Syn_Site::Syn_Site()
:_pSyn_Dut(NULL)
{
}

Syn_Site::~Syn_Site()
{
	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}
}

void Syn_Site::Run()
{
	//Syn_Dut.ReadOTP()
}
