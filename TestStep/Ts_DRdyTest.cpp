#include "Ts_DRdyTest.h"


Ts_DRdyTest::Ts_DRdyTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_DRdyTest::~Ts_DRdyTest()
{
}

void Ts_DRdyTest::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = 0x10;	//bit 1
	_pSyn_Dut->_pSyn_DutTestInfo->_DRdyInfo.m_pinMsk = 1;		//port B




	//power on

}

void Ts_DRdyTest::Execute()
{
}

void Ts_DRdyTest::ProcessData()
{

}

void Ts_DRdyTest::CleanUp()
{

}
