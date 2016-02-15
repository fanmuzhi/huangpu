#pragma once

//Local
#include "Syn_ModuleDecorator.h"

//DutUtls
#include "Syn_DutCtrl.h"
#include "Syn_Dut.h"

//C++
#include <string>
using namespace std;

extern "C" {
#include "SYN_TestUtils.h"
};

class Syn_TestStep
{
public:

	
	
	Syn_TestStep(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_ModuleDecorator * &pSyn_ModuleDecorator) 
		:_strName(strName), _pSyn_DutCtrl(pDutCtrl), _pSyn_Dut(pDut), _pSyn_ModuleDecorator(pSyn_ModuleDecorator)
	{
		//if (pDut)
		//_pSyn_ModuleDecorator = 
	}

	virtual ~Syn_TestStep()
	{
	}

	string GetName()
	{
		return _strName; 
	}

	//pure virtual function
	virtual int	SetUp() = 0;

	virtual int	Execute() = 0;

	virtual int ProcessData() = 0;

	virtual int	CleanUp() = 0;

protected:

	string _strName;

	Syn_DutCtrl *_pSyn_DutCtrl;
	
	Syn_Dut *_pSyn_Dut;

	Syn_ModuleDecorator *_pSyn_ModuleDecorator;
};

