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

	Syn_Exception(uint32_t iErrorNum) :_uiErrorNumber(iErrorNum), _strDescription("")
	{
	}
	Syn_Exception(uint32_t iErrorNum, std::string strDescription) :_uiErrorNumber(iErrorNum), _strDescription(strDescription)
	{
	}
	
	inline void SetError(const uint32_t error)
	{
		_uiErrorNumber = error;
	}

	inline uint32_t GetError()
	{
		return _uiErrorNumber;
	}

	inline void	SetDescription(const std::string &strDescription)
	{ 
		_strDescription = strDescription;
	}

	inline std::string GetDescription() const
	{
		return _strDescription + "(" + to_string(_uiErrorNumber) + ")";
	}

	inline const char* what()
	{
		std::string descrip = GetDescription();
		return descrip.c_str();
	}

private:

	string		_strDescription;
	uint32_t	_uiErrorNumber;
};