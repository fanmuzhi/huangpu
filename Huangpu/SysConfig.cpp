
//Local
#include "SysConfig.h"

//C
//#include <stdio.h>
#include <io.h>

//std
#include <iostream>
using namespace std;

SysConfig::SysConfig(QString strXMLFilePath)
://q_DomDocument(NULL)
q_strFilePath(strXMLFilePath)
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

	QFile q_File(file_name);
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