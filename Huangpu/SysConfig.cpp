
//Local
#include "SysConfig.h"

//C
//#include <stdio.h>
#include <io.h>

//std
#include <iostream>
using namespace std;

SysConfig::SysConfig(QString strXMLFilePath)
:q_File()
, q_DomDocument()
, q_RootNode()
,q_strFilePath(strXMLFilePath)
{
	bool bResult = ParseXML(q_strFilePath);
	if(!bResult)
	{
		cout<<"Error:Cosntructor - ::ParseXML() is failed!"<<endl;
		return;
	}
}

SysConfig::~SysConfig()
{
	q_File.close();
}

bool SysConfig::CreateSysConfigInstance(QString strXMLFilePath, SysConfig * &opSysConfigInstance)
{
	opSysConfigInstance = NULL;

	int IsExists = _access(strXMLFilePath.toStdString().c_str(),4);//2
	if (0 != IsExists)
	{
		cout << "Error:SysConfig::CreateSysConfigInstance() - strXMLFilePath is not exists!" << endl;
		return false;
	}

	opSysConfigInstance = new SysConfig(strXMLFilePath);

	return true;
}

bool SysConfig::ParseXML(const QString &file_name)
{
	if(file_name.isEmpty())  
	{
		cout<<"Error:SysConfig::ParseXML() - file_name is NULL!"<<endl;
		return false;
	}  

	//QFile q_File(file_name);
	q_File.setFileName(file_name);
	if (!q_File.exists())
	{
		cout << "Error:SysConfig::ParseXML() - q_File is not exists!" << endl;
		return false;
	}

	if(!q_File.open(QFile::ReadOnly | QFile::Text))
	{
		cout<<"Error:SysConfig::ParseXML() - q_File.open is failed!"<<endl;
		return false;  
	}  

	QString error;  
	int row = 0, column = 0;  
	if(!q_DomDocument.setContent(&q_File, false, &error, &row, &column))
	{  
		cout<<"Error:SysConfig::ParseXML() - q_DomDocument.setContent is failed!"<<endl;
		return false;  
	}  

	if(q_DomDocument.isNull())
	{  
		cout<<"Error:SysConfig::ParseXML() - q_DomDocument is null!"<<endl;
		return false;  
	}  

	QDomElement root = q_DomDocument.documentElement(); 
	/*if (strRootNodeName != root.tagName())
	{
		cout<<"Error:SysConfig::ParseXML() - root is not TesterConfig!"<<endl;
		return false;  
	}*/

	q_RootNode = root;

	return true;
}

bool SysConfig::GetElementNodeText(const QString &strNodeName,QString &strNodeTextValue)
{
	if(strNodeName.isEmpty())
	{
		cout<<"Error:SysConfig::GetElementNodeText() - strNodeName is NULL!"<<endl;
		return false;  
	}

	QDomNodeList list = q_RootNode.childNodes();
	int iCounts = list.count();
	for(int i=1; i<=iCounts;i++)
	{
		QDomNode dom_node = list.item(i-1);
		QDomElement element = dom_node.toElement();

		if(strNodeName==element.tagName())
		{
			strNodeTextValue = element.text();
			break;
		}
	}

	return true;
}

bool SysConfig::GetElementNodeTextAndAttribute(const QString &strNodeName,QString &strNodeTextValue,
														   const QString &strAttrubuteName,QString &strAttributetValue)
{
	QDomNodeList list = q_RootNode.childNodes();
	int iCounts = list.count();
	for(int i=1; i<=iCounts;i++)
	{
		QDomNode dom_node = list.item(i-1);
		QDomElement element = dom_node.toElement();

		if(strNodeName==element.tagName())
		{
			strNodeTextValue = element.text();
			strAttributetValue = element.attribute(strAttrubuteName);

			break;
		}
	}

	return true;
}

bool SysConfig::GetTestSeqList(std::vector<TestSeqInfo> &ListOfTestSeqInfo)
{
	QDomElement TestSeqElement;
	bool FindResult(false);

	QDomNodeList list = q_RootNode.childNodes();
	int iCounts = list.count();
	for(int i=1;i<=iCounts;i++)
	{
		QDomNode dom_node = list.item(i-1);
		QDomElement element = dom_node.toElement();

		if(QString("TestSeq")==element.tagName())
		{
			TestSeqElement = element;
			FindResult = true;

			break;
		}
	}

	if(FindResult)
	{
		QDomNodeList listOfTestSeq = TestSeqElement.childNodes();
		for(int i=1;i<=listOfTestSeq.count();i++)
		{
			QDomNode domNode = listOfTestSeq.item(i-1);
			QDomElement elementNode = domNode.toElement();

			TestSeqInfo CurrentTestSeqInfo;
			CurrentTestSeqInfo.strNodeName = elementNode.tagName();
			CurrentTestSeqInfo.strSeqText = elementNode.text();
			CurrentTestSeqInfo.strSeqAttribute = elementNode.attribute("Args");

			ListOfTestSeqInfo.push_back(CurrentTestSeqInfo);
		}
	}

	return FindResult;
}

void SysConfig::ConvertAsciiToBinary(const std::string& strAsciiValue, uint8_t *pDst, int nDstSize)
{
	char *p;

	int	nNumBytes = strAsciiValue.length()/2;
	//Translate the ASCII into binary values.
	for (int i = 0; (i<nNumBytes) && (i<nDstSize); i++)
	{
		std::string strTemp(strAsciiValue,i * 2, 2);

		uint8_t nVal = (uint8_t)strtol(strTemp.c_str(), &p, 16);
		//uint8_t nVal = (uint8_t)stoul(strTemp, 0, 2);
		pDst[i] = nVal;
	}
}

bool SysConfig::GetSyn_SysConfig(Syn_SysConfig &oSyn_SysConfig)
{
	bool rc(false);//return code

	char *p = NULL;

	//1st
	QString qstrAutoControllerName("AutoController"), qstrAutoControllerValue("");
	QString qstrDutTypeName("DutType"), qstrDutTypeValue("");
	QString qstrDutControllerName("DutController"), qstrDutControllerValue("");
	QString qstrDutComName("DutCom"), qstrDutComValue("");
	rc = this->GetElementNodeText(qstrAutoControllerName, qstrAutoControllerValue);
	rc = this->GetElementNodeText(qstrDutTypeName, qstrDutTypeValue);
	rc = this->GetElementNodeText(qstrDutControllerName, qstrDutControllerValue);
	rc = this->GetElementNodeText(qstrDutComName, qstrDutComValue);
	if (QString("") == qstrAutoControllerValue || QString("") == qstrDutTypeValue || QString("") == qstrDutControllerValue || QString("") == qstrDutComValue)
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 1st operation is failed!" << endl;
		return false;
	}

	//2nd
	QString qstrDutPwrVdd_mVName("DutPwrVdd_mV"), qstrDutPwrVdd_mVValue("");
	QString qstrDutPwrVio_mVName("DutPwrVio_mV"), qstrDutPwrVio_mVValue("");
	QString qstrDutPwrVled_mVName("DutPwrVled_mV"), qstrDutPwrVled_mVValue("");
	QString qstrDutPwrVddh_mVName("DutPwrVddh_mV"), qstrDutPwrVddh_mVValue("");
	rc = this->GetElementNodeText(qstrDutPwrVdd_mVName, qstrDutPwrVdd_mVValue);
	rc = this->GetElementNodeText(qstrDutPwrVio_mVName, qstrDutPwrVio_mVValue);
	rc = this->GetElementNodeText(qstrDutPwrVled_mVName, qstrDutPwrVled_mVValue);
	rc = this->GetElementNodeText(qstrDutPwrVddh_mVName, qstrDutPwrVddh_mVValue);
	if (QString("") == qstrDutPwrVdd_mVValue || QString("") == qstrDutPwrVio_mVValue || QString("") == qstrDutPwrVled_mVValue || QString("") == qstrDutPwrVddh_mVValue)
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 2nd operation is failed!" << endl;
		return false;
	}

	//3rd
	QString qstrNumRowsName("NumRows"), qstrNumRowsValue("");
	QString qstrNumColsName("NumCols"), qstrNumColsValue("");
	rc = this->GetElementNodeText(qstrNumRowsName, qstrNumRowsValue);
	rc = this->GetElementNodeText(qstrNumColsName, qstrNumColsValue);
	if (QString("") == qstrNumRowsValue || QString("") == qstrNumColsValue)
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 3rd operation is failed!" << endl;
		return false;
	}

	//4th
	QString qstrWriteBootSectorsName("WriteBootSectors"), qstrWriteBootSectorsValue("");
	QString qstrBootSector0Name("BootSector0"), qstrBootSector0Value("");
	QString qstrBootSector1Name("BootSector1"), qstrBootSector1Value("");
	rc = this->GetElementNodeText(qstrWriteBootSectorsName, qstrWriteBootSectorsValue);
	rc = this->GetElementNodeText(qstrBootSector0Name, qstrBootSector0Value);
	rc = this->GetElementNodeText(qstrBootSector1Name, qstrBootSector1Value);
	if (QString("") == qstrWriteBootSectorsValue || QString("") == qstrBootSector0Value || QString("") == qstrBootSector1Value)
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 4th operation is failed!" << endl;
		return false;
	}

	//5th
	std::vector<TestSeqInfo> listOfTestSeqInfo;
	rc = this->GetTestSeqList(listOfTestSeqInfo);
	std::vector<Syn_TestStepInfo> listOfTestSteps;
	if (0 == listOfTestSeqInfo.size())
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 5th operation is failed!" << endl;
		return false;
	}
	for (int i = 1; i <= listOfTestSeqInfo.size(); i++)
	{
		Syn_TestStepInfo CurrentTestStepInfo;
		CurrentTestStepInfo._strNodeName = listOfTestSeqInfo[i - 1].strNodeName.toStdString();
		CurrentTestStepInfo._strTestStepName = listOfTestSeqInfo[i - 1].strSeqText.toStdString();
		CurrentTestStepInfo._strTestStepArgs = listOfTestSeqInfo[i - 1].strSeqAttribute.toStdString();
	}

	//6th
	std::vector<std::string> listOfXepathName;

	listOfXepathName.push_back(std::string("ImageAcqPatch"));
	listOfXepathName.push_back(std::string("PrintFile"));
	listOfXepathName.push_back(std::string("OtpReadWritePatch"));
	listOfXepathName.push_back(std::string("OpensShortsPatch"));
	listOfXepathName.push_back(std::string("WofPatch"));
	listOfXepathName.push_back(std::string("WofLowPowerPatch"));
	listOfXepathName.push_back(std::string("ScmWofPatch"));
	listOfXepathName.push_back(std::string("AfePatch"));

	//RAM Patch Test
	listOfXepathName.push_back(std::string("CacheDataRam"));
	listOfXepathName.push_back(std::string("CacheTagRam"));
	listOfXepathName.push_back(std::string("CachInstDataRam"));
	listOfXepathName.push_back(std::string("CachInstTagRam"));
	listOfXepathName.push_back(std::string("ScmAndMainRam"));

	listOfXepathName.push_back(std::string("Cmd1ScmWofPlot"));
	listOfXepathName.push_back(std::string("Cmd2ScmWofBin"));
	listOfXepathName.push_back(std::string("Cmd3SweepTag"));

	listOfXepathName.push_back(std::string("WofCmd1"));
	listOfXepathName.push_back(std::string("WofCmd2"));
	listOfXepathName.push_back(std::string("WofCmd3"));
	listOfXepathName.push_back(std::string("WofCmd4"));
	listOfXepathName.push_back(std::string("PixelPatch"));
	listOfXepathName.push_back(std::string("SpiFlashRamPatch"));
	listOfXepathName.push_back(std::string("BulkEraseCmd"));
	listOfXepathName.push_back(std::string("ProgramCmd"));
	listOfXepathName.push_back(std::string("ReadCmd"));
	listOfXepathName.push_back(std::string("WovarPatch"));
	listOfXepathName.push_back(std::string("VCEK_IV"));
	listOfXepathName.push_back(std::string("WofLowPowerBin"));
	listOfXepathName.push_back(std::string("ProdOtpReadWritePatch"));
	listOfXepathName.push_back(std::string("SecurityMgtEngrPatch"));
	listOfXepathName.push_back(std::string("SecurityMgtProdPatch"));
	listOfXepathName.push_back(std::string("TakeOwnershipBin"));

	std::vector<Syn_XepatchInfo> listofXepatchInfo;
	for (auto i = 0; i < listOfXepathName.size(); i++)
	{
		Syn_XepatchInfo CurrentSyn_XepatchInfo;

		std::string strXepatchName(listOfXepathName[i]);

		QString qstrXepatchName(QString::fromStdString(strXepatchName)), qstrXepatchTextValue("");
		QString qstrXepatchArgsName("Args"), qstrXepatchDisplayName("");

		rc = this->GetElementNodeTextAndAttribute(qstrXepatchName, qstrXepatchTextValue, qstrXepatchArgsName, qstrXepatchDisplayName);

		std::string strXepatchTextValue(qstrXepatchTextValue.toStdString());
		int iXepatchTextLenth = strXepatchTextValue.length();
		if (0 == iXepatchTextLenth)
		{
			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = NULL;
			CurrentSyn_XepatchInfo._uiArraySize = 0;
		}
		else
		{
			uint8_t *pArray = new uint8_t[iXepatchTextLenth / 2];
			this->ConvertAsciiToBinary(strXepatchTextValue, pArray, iXepatchTextLenth / 2);

			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = pArray;
			CurrentSyn_XepatchInfo._uiArraySize = iXepatchTextLenth / 2;
		}

		listofXepatchInfo.push_back(CurrentSyn_XepatchInfo);
	}
	if (0 == listofXepatchInfo.size())
	{
		cout << "Error:SysConfig::GetSyn_SysConfig() - 6th operation is failed!" << endl;
		return false;
	}

	//Fill Syn_SysConfig
	oSyn_SysConfig._strAutoController = qstrAutoControllerValue.toStdString();
	oSyn_SysConfig._strDutType = qstrDutTypeValue.toStdString();
	oSyn_SysConfig._strDutController = qstrDutControllerValue.toStdString();
	oSyn_SysConfig._strDutCom = qstrDutComValue.toStdString();

	oSyn_SysConfig._uiDutpwrVdd_mV = (uint16_t)strtol(qstrDutPwrVdd_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVio_mV = (uint16_t)strtol(qstrDutPwrVio_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVled_mV = (uint16_t)strtol(qstrDutPwrVled_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVddh_mV = (uint16_t)strtol(qstrDutPwrVddh_mVValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._uiNumRows = (uint16_t)strtol(qstrNumRowsValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiNumCols = (uint16_t)strtol(qstrNumColsValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._bWriteBootSectors = (uint16_t)strtol(qstrWriteBootSectorsValue.toStdString().c_str(), &p, 10);
	this->ConvertAsciiToBinary(qstrBootSector0Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS0, BS0_SIZE);
	this->ConvertAsciiToBinary(qstrBootSector1Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS1, BS1_SIZE);

	oSyn_SysConfig._listTestSteps = listOfTestSteps;

	oSyn_SysConfig._listXepatchInfo = listofXepatchInfo;

	return true;
}