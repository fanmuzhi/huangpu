#pragma once

//Local
#include "Syn_Module.h"

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

	
	
	Syn_TestStep(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_Module * &pSyn_Module)
		:_strName(strName), _pSyn_DutCtrl(pDutCtrl), _pSyn_Dut(pDut), _pSyn_Module(pSyn_Module)
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

	Syn_Module *_pSyn_Module;
};

