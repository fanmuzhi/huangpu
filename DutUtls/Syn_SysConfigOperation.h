#ifndef SYN_SYSCONFIGOPRERATION_H
#define SYN_SYSCONFIGOPRERATION_H

//local
#include "Syn_Config.h"

//rapidxml
#include "rapidxml.hpp"  
#include "rapidxml_utils.hpp" 

//std C
#include "stdint.h"

//std
#include <vector>
#include <string>
using namespace std;

class Syn_SysConfigOperation
{

public:

	static bool GetSysConfigInstance(std::string strConfigFilePath, Syn_SysConfigOperation * &opSyn_SysConfigOperationInstance);
	
	~Syn_SysConfigOperation();

	bool GetSysConfig(Syn_SysConfig &oSyn_SysConfig);

private:

	//constructor
	Syn_SysConfigOperation(std::string strConfigFilePath);

	//function
	bool Parse();

	void ConvertAsciiToBinary(const std::string& strAsciiValue, uint8_t *pDst, int nDstSize);

private:

	rapidxml::file<> _rapidxmlfile;

	rapidxml::xml_document<> _rapidxmldoc;

	rapidxml::xml_node<> *_rapidxmlrootNode;

};

#endif //SYN_SYSCONFIGOPRERATION_H