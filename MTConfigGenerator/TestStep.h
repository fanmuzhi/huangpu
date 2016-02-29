#pragma once

#include <string>
#include <vector>
#include "stdint.h"

using std::string;
using std::vector;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TestStep
{
public:
	TestStep(const string& sName, const string& sArgNames, const string& sArgDefaults, const string& sArgDetails)
		:m_sName(sName)
	{
		//Parse the argument names and descriptions.
		if (sArgNames.size() != 0)
		{
			int argIdx = 0;
			int defaultIdx = 0;
			int descIdx = 0;
			while (sArgNames.find_first_of(';', argIdx) != std::string::npos)
			{
				m_arArgNames.push_back(sArgNames.substr(argIdx, sArgNames.find_first_of(';', argIdx) - argIdx));
				m_arArgDefaults.push_back(sArgDefaults.substr(defaultIdx, sArgDefaults.find_first_of(';', defaultIdx) - defaultIdx));
				m_arArgDetails.push_back(sArgDetails.substr(descIdx, sArgDetails.find_first_of(';', descIdx) - descIdx));
				argIdx = sArgNames.find_first_of(';', argIdx) +1;
				defaultIdx = sArgDefaults.find_first_of(';', defaultIdx) +1;
				descIdx = sArgDetails.find_first_of(';', descIdx) +1;
			}
			m_arArgNames.push_back(sArgNames.substr(argIdx));
			m_arArgDefaults.push_back(sArgDefaults.substr(defaultIdx));
			m_arArgDetails.push_back(sArgDetails.substr(descIdx));
		}
	}

	~TestStep()
	{
	}

	string			GetName()				{return m_sName;}
	int				GetNumArgs()			{return m_arArgNames.size();}
	string			GetArgName(int i)		{return m_arArgNames[i];}
	string			GetArgDefaults(int i)	{return m_arArgDefaults[i];}
	string			GetArgDetails(int i)	{return m_arArgDetails[i];}

protected:

private:
	string			m_sName;
	vector<string>	m_arArgNames;
	vector<string>	m_arArgDefaults;
	vector<string>	m_arArgDetails;
};
