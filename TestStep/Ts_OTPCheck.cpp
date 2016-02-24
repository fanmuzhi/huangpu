#include "Ts_OTPCheck.h"


Ts_OTPCheck::Ts_OTPCheck(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, pDutCtrl, pDut)
{
}


Ts_OTPCheck::~Ts_OTPCheck()
{
}

int Ts_OTPCheck::SetUp()
{
	if (NULL == _pSyn_DutCtrl)
	{
		return 0;
	}
	if (NULL == _pSyn_Dut)
	{
		return 0;
	}

	
	
}

int Ts_OTPCheck::Excute()
{

}

int Ts_OTPCheck::CleanUp()
{

}
