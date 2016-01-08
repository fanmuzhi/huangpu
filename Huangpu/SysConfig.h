#ifndef SYSCONFIG_H
#define SYSCONFIG_H

//Qt
#include "QString.h"
#include <QtXml/QDomDocument>
#include <qfile.h>
#include <qdom.h>

//DutUtls
#include "Syn_Config.h"

//std
#include <vector>
#include <string>

//std C
#include "stdint.h"

using namespace std;

struct TestSeqInfo
{
	QString strNodeName;

	QString strSeqText;
	QString strSeqAttribute;
};

class SysConfig
{
public:
	
	static bool CreateSysConfigInstance(QString strXMLFilePath, SysConfig * &opSysConfigInstance);
	
	//SysConfig(QString strXMLFilePath);
	~SysConfig();

	bool GetSyn_SysConfig(Syn_SysConfig &oSyn_SysConfig);

private:

	SysConfig(QString strXMLFilePath);//Constructor

	bool ParseXML(const QString &file_name);

	bool GetElementNodeText(const QString &strNodeName,QString &strNodeTextValue);

	bool GetElementNodeTextAndAttribute(const QString &strNodeName,QString &strNodeTextValue,
		                                const QString &strAttrubuteName,QString &strAttributetValue);

	bool GetTestSeqList(std::vector<TestSeqInfo> &ListOfTestSeqInfo);

	void ConvertAsciiToBinary(const std::string& strAsciiValue, uint8_t *pDst, int nDstSize);


private:

	QFile q_File;

	QDomDocument q_DomDocument;
	
	QDomElement q_RootNode;
	
	QString q_strFilePath;
};

#endif // SYSCONFIG_H
