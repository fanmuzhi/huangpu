#pragma once

//local
#include "Syn_Dut.h"

//std
#include <vector>
using namespace std;

class Syn_DutOperation
{
public:
	Syn_DutOperation();
	~Syn_DutOperation();

	static bool StartDutOperation(vector<Syn_Dut*> &ListOfDuctPtr, std::string &strValue);
};

