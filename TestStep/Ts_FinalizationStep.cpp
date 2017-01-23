#include "Ts_FinalizationStep.h"

#include <stdio.h>
#include <io.h>

Ts_FinalizationStep::Ts_FinalizationStep(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_FinalizationStep::~Ts_FinalizationStep()
{
}

void Ts_FinalizationStep::SetUp()
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
}

void Ts_FinalizationStep::Execute()
{
	if (0 == _pSyn_Dut->_pSyn_DutTestResult->_binCodes.size())
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sPass);
	}

	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("FinalizationStep", "Pass"));
}

void Ts_FinalizationStep::ProcessData()
{
	//WriteLog();
}

void Ts_FinalizationStep::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc = _pSyn_DutCtrl->PowerOff();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("PowerOff command failed!");
		throw ex;
		return;
	}
}