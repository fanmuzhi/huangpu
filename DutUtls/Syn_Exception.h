// MTExceptions.h
#pragma once

//std
#include <exception>
#include <cstdint>
#include <string>
using namespace std;

class Syn_Exception : public exception
{
public:

	Syn_Exception(uint16_t iErrorNum) :_uiErrorNumber(iErrorNum), _strDescription("")
	{
	}
	Syn_Exception(uint16_t iErrorNum, std::string strDescription) :_uiErrorNumber(iErrorNum), _strDescription(strDescription)
	{
	}

	inline void	SetDescription(const std::string &strDescription)
	{ 
		_strDescription = strDescription;
	}

	inline std::string GetDescription() const
	{
		return _strDescription + "(" + to_string(_uiErrorNumber) + ")";
	}

private:

	string		_strDescription;
	uint16_t	_uiErrorNumber;
};