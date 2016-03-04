#include "Ts_RxStandardDev.h"

Ts_RxStandardDev::Ts_RxStandardDev(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_RxStandardDev::~Ts_RxStandardDev()
{
}

void Ts_RxStandardDev::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	//parse args
	std::vector<std::string> listOfArgValue;
	
}


void Ts_RxStandardDev::Execute()
{

}

void Ts_RxStandardDev::ProcessData()
{
	

}

void Ts_RxStandardDev::CleanUp()
{
	//PowerOff();
}