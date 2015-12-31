#ifndef SYSCONFIG_H
#define SYSCONFIG_H

//Qt
#include "QString.h"
#include <QtXml/QDomDocument>
#include <qfile.h>
#include <qdom.h>

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

	bool ParseXML(const QString &file_name);

	bool GetElementNodeText(const QString &strNodeName,QString &strNodeTextValue);

	bool GetElementNodeTextAndAttribute(const QString &strNodeName,QString &strNodeTextValue,
		                                const QString &strAttrubuteName,QString &strAttributetValue);

	bool GetTestSeqList(std::vector<TestSeqInfo> &ListOfTestSeqInfo);

	void ConvertAsciiToBinary(const std::string& strAsciiValue, uint8_t *pDst, int nDstSize);

private:

	SysConfig(QString strXMLFilePath);

private:

	QDomDocument q_DomDocument;
	//QFile q_File;
	QDomElement q_RootNode;
	
	QString q_strFilePath;
};

#endif // SYSCONFIG_H
