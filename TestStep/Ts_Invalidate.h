#pragma once

#include "Syn_FingerprintTest.h"

class Ts_Invalidate : public Syn_FingerprintTest
{
public:
	Ts_Invalidate(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Invalidate();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	void Invalidate(uint32_t tagId);

	void get_invalidate_arguments(uint32_t tagId, char* outString, int &oNumBytes);
};

