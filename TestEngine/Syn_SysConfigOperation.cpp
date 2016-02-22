//local
#include "Syn_SysConfigOperation.h"

//
#include "Syn_Exception.h"

//third-party
#include "easylogging++.h"

//std
#include <iostream>
#include <io.h>
#include <stdio.h>

Syn_SysConfigOperation::Syn_SysConfigOperation(std::string strConfigFilePath)
:_rapidxmlfile(strConfigFilePath.c_str())
,_rapidxmldoc()
, _rapidxmlrootNode(NULL)
{
}

Syn_SysConfigOperation::~Syn_SysConfigOperation()
{
}

uint32_t Syn_SysConfigOperation::GetSysConfigInstance(std::string strConfigFilePath, Syn_SysConfigOperation * &opSyn_SysConfigOperationInstance)
{
	opSyn_SysConfigOperationInstance = NULL;

	if (-1 == _access(strConfigFilePath.c_str(), 4))
	{
		LOG(ERROR) << "Error:Syn_SysConfigOperation::GetSysConfigInstance() - strConfigFilePath is not exists!" << endl;
		return NULL;
	}

	opSyn_SysConfigOperationInstance = new Syn_SysConfigOperation(strConfigFilePath);
	
	//reparse
	bool rc = opSyn_SysConfigOperationInstance->Parse();
	if (!rc)
	{
		delete opSyn_SysConfigOperationInstance;
		opSyn_SysConfigOperationInstance = NULL;
		LOG(ERROR) << "Error:Syn_SysConfigOperation::GetSysConfigInstance() - ::Parse is failed!" << endl;
		return false;
	}

	return true;
}

bool Syn_SysConfigOperation::Parse()
{
	_rapidxmldoc.parse<0>(_rapidxmlfile.data());

	//root
	_rapidxmlrootNode = NULL;
	_rapidxmlrootNode = _rapidxmldoc.first_node();
	if (NULL == _rapidxmlrootNode)
	{
		LOG(ERROR) << "Error:Syn_SysConfigOperation::Parse() - _rapidxmlrootNode is NULL!" << endl;
		return false;
	}

	return true;
}

uint32_t Syn_SysConfigOperation::GetSysConfig(Syn_SysConfig &oSyn_SysConfig)
{
	bool rc(true);

	if (NULL == _rapidxmlrootNode)
	{
		return false;
	}

	unsigned int SysConfigJudgeTag(0);

	for (rapidxml::xml_node<char> *node = _rapidxmlrootNode->first_node(); node != NULL; node = node->next_sibling())
	{
		std::string strNodeName = node->name();
		if (std::string("AutoController") == strNodeName)
		{
			oSyn_SysConfig._strAutoController = node->value();
			//cout << "" << oSyn_SysConfig._strAutoController << endl;
			SysConfigJudgeTag += 1;
		}
		else if (std::string("DutType") == strNodeName)
		{
			oSyn_SysConfig._strDutType = node->value();
			//cout << "" << oSyn_SysConfig._strDutType << endl;
			SysConfigJudgeTag += 1;
		}
		else if (std::string("DutController") == strNodeName)
		{
			oSyn_SysConfig._strDutController = node->value();
			//cout << "" << oSyn_SysConfig._strDutController << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("DutCom") == strNodeName)
		{
			oSyn_SysConfig._strDutCom = node->value();
			//cout << "" << oSyn_SysConfig._strDutCom << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("DutPwrVdd_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVdd_mV = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiDutpwrVdd_mV << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("DutPwrVio_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVio_mV = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiDutpwrVio_mV << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("DutPwrVled_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVled_mV = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiDutpwrVled_mV << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("DutPwrVddh_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVddh_mV = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiDutpwrVddh_mV << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("NumRows") == strNodeName)
		{
			oSyn_SysConfig._uiNumRows = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiNumRows << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("NumCols") == strNodeName)
		{
			oSyn_SysConfig._uiNumCols = atoi(node->value());
			//cout << "" << oSyn_SysConfig._uiNumRows << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("WriteBootSectors") == strNodeName)
		{
			if (0 == atoi(node->value()))
			{
				oSyn_SysConfig._bWriteBootSectors = false;
			}
			else
			{
				oSyn_SysConfig._bWriteBootSectors = true;
			}
			//cout << "" << oSyn_SysConfig._bWriteBootSectors << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("BootSector0") == strNodeName)
		{
			ConvertAsciiToBinary(node->value(), oSyn_SysConfig._arrUserSpecifiedBS0, BS0_SIZE);
			//cout << "" << node->value() << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("BootSector1") == strNodeName)
		{
			ConvertAsciiToBinary(node->value(), oSyn_SysConfig._arrUserSpecifiedBS1, BS1_SIZE);
			//cout << "" << node->value() << endl;
			SysConfigJudgeTag += 1;

		}
		else if (std::string("TestSeq") == strNodeName)
		{
			//TestScript
			Syn_TestScript CurrentTestScript;
			rapidxml::xml_attribute<> *attribute = node->first_attribute("Script");
			if (NULL != attribute)
			{
				CurrentTestScript._strScriptName = attribute->value();
			}

			for (rapidxml::xml_node<char> *TestNode = node->first_node(); TestNode != NULL; TestNode = TestNode->next_sibling())
			{
				Syn_TestStepInfo CurrentSyn_TestStepInfo;
				CurrentSyn_TestStepInfo._strNodeName = TestNode->name();
				CurrentSyn_TestStepInfo._strTestStepName = TestNode->value();

				rapidxml::xml_attribute<> *pAttribute = TestNode->first_attribute("Args");
				if (NULL != pAttribute)
				{
					CurrentSyn_TestStepInfo._strTestStepArgs = pAttribute->value();
					//cout << CurrentSyn_TestStepInfo._strTestStepName << " " << CurrentSyn_TestStepInfo._strTestStepArgs << endl;
				}

				CurrentTestScript._listOfTestStep.push_back(CurrentSyn_TestStepInfo);
				//oSyn_SysConfig._listTestSteps.push_back(CurrentSyn_TestStepInfo);
			}
			SysConfigJudgeTag += 1;

			oSyn_SysConfig._listTestScript.push_back(CurrentTestScript);

		}
		else if (std::string("ImageAcqPatch") == strNodeName || std::string("PrintFile") == strNodeName || std::string("OtpReadWritePatch") == strNodeName || std::string("OpensShortsPatch") == strNodeName || std::string("WofPatch") == strNodeName || std::string("WofLowPowerPatch") == strNodeName || std::string("ScmWofPatch") == strNodeName || std::string("AfePatch") == strNodeName
				|| std::string("CacheDataRam") == strNodeName || std::string("CacheTagRam") == strNodeName || std::string("CachInstDataRam") == strNodeName || std::string("CachInstTagRam") == strNodeName || std::string("ScmAndMainRam") == strNodeName
				|| std::string("Cmd1ScmWofPlot") == strNodeName || std::string("Cmd2ScmWofBin") == strNodeName || std::string("Cmd3SweepTag") == strNodeName
				|| std::string("WofCmd1") == strNodeName || std::string("WofCmd2") == strNodeName || std::string("WofCmd3") == strNodeName || std::string("WofCmd4") == strNodeName || std::string("PixelPatch") == strNodeName || std::string("SpiFlashRamPatch") == strNodeName || std::string("BulkEraseCmd") == strNodeName || std::string("ProgramCmd") == strNodeName
				|| std::string("ReadCmd") == strNodeName || std::string("WovarPatch") == strNodeName || std::string("VCEK_IV") == strNodeName || std::string("WofLowPowerBin") == strNodeName || std::string("ProdOtpReadWritePatch") == strNodeName || std::string("SecurityMgtEngrPatch") == strNodeName || std::string("SecurityMgtProdPatch") == strNodeName || std::string("TakeOwnershipBin") == strNodeName)
		{
			Syn_PatchInfo CurrentSyn_XepatchInfo;

			std::string strXepatchName(strNodeName);

			rapidxml::xml_attribute<> *attribute = node->first_attribute("Args");
			if (NULL != attribute)
			{
				CurrentSyn_XepatchInfo._strXepatchFileName = attribute->value();
				//cout << strNodeName << " " << CurrentSyn_XepatchInfo._strXepatchFileName << endl;
			}
			else
			{
				//cout << strNodeName << " has no args " << endl;
			}

			std::string strPatchTextValue(node->value());
			int iTextLenth = strPatchTextValue.length();
			if (0 == iTextLenth)
			{
				CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
				CurrentSyn_XepatchInfo._pArrayBuf = NULL;
				CurrentSyn_XepatchInfo._uiArraySize = 0;
			}
			else
			{
				uint8_t *pArray = new uint8_t[iTextLenth / 2];
				ConvertAsciiToBinary(strPatchTextValue, pArray, iTextLenth / 2);

				CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
				CurrentSyn_XepatchInfo._pArrayBuf = pArray;
				CurrentSyn_XepatchInfo._uiArraySize = iTextLenth / 2;
			}

			oSyn_SysConfig._listPatchInfo.push_back(CurrentSyn_XepatchInfo);

			SysConfigJudgeTag += 1;

		}
		else
		{
			continue;
		}
	}

	if (SysConfigJudgeTag < 20)
	{
		LOG(ERROR) <<"SysConfigJudgeTag IS LESS THAN 20" ;
		return Syn_ExceptionCode::Syn_ConfigError;
	}

	return Syn_ExceptionCode::Syn_OK;
}

void Syn_SysConfigOperation::ConvertAsciiToBinary(const std::string& strAsciiValue, uint8_t *pDst, int nDstSize)
{
	char *p;

	int	nNumBytes = strAsciiValue.length() / 2;
	//Translate the ASCII into binary values.
	for (int i = 0; (i<nNumBytes) && (i<nDstSize); i++)
	{
		std::string strTemp(strAsciiValue, i * 2, 2);

		uint8_t nVal = (uint8_t)strtol(strTemp.c_str(), &p, 16);
		pDst[i] = nVal;
	}
}