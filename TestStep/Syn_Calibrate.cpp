#include "Syn_Calibrate.h"


Syn_Calibrate::Syn_Calibrate(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, pDutCtrl, pDut)
{
}


Syn_Calibrate::~Syn_Calibrate()
{
}

int Syn_Calibrate::Excute()
{
	if (NULL == _pSyn_DutCtrl)
	{
		return 0;
	}

	return 0;
}
