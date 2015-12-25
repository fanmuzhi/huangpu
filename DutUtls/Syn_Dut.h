#pragma once

enum ProjectType {
	Viper1 = 0x1000,
	Viper2,
	Metallica
};

class Syn_Dut
{
public:
	Syn_Dut();
	virtual ~Syn_Dut();
};

