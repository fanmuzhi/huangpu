#pragma once
#include "Syn_Dut.h"
class Syn_Metallica :
	public Syn_Dut
{
public:
	Syn_Metallica();
	~Syn_Metallica();

	void GetProjectType(ProjectType &oProjectType);

};

