#pragma once

//Local
#include "Syn_Module.h"

//DutUtls
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Config.h"
#include "Syn_BinCodes.h"
#include "Syn_TestUtils.h"

//C++
#include <string>
#include <vector>
using namespace std;


class Syn_TestStep
{
public:
	
	Syn_TestStep(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
		:_strName(strName),  _strArgs(strArgs), _pSyn_DutCtrl(pDutCtrl), _pSyn_Dut(pDut)
	{
	}

	virtual ~Syn_TestStep()
	{
	}

	string GetName()
	{
		return _strName; 
	}

	//pure virtual function
	virtual	void	SetUp() = 0;

	virtual void	Execute() = 0;

	virtual void	ProcessData() = 0;

	virtual void	CleanUp() = 0;

protected:

	string _strName;

	string _strArgs;

	Syn_DutCtrl *_pSyn_DutCtrl;
	
	Syn_Dut *_pSyn_Dut;
};