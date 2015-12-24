#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include "QString.h"
#include <QtXml/QDomDocument>
#include <qfile.h>
#include <qdom.h>

#include <vector>

struct TestSeqInfo
{
	QString strNodeName;

	QString strSeqText;
	QString strSeqAttribute;
};

class SysConfig
{
public:
	SysConfig(QString strXMLFilePath);
	~SysConfig();

	bool ParseXML(const QString &file_name);

	bool GetElementNodeText(const QString &strNodeName,QString &strNodeTextValue);

	bool GetElementNodeTextAndAttribute(const QString &strNodeName,QString &strNodeTextValue,
		                                const QString &strAttrubuteName,QString &strAttributetValue);

	bool GetTestSeqList(std::vector<TestSeqInfo> &ListOfTestSeqInfo);


private:
	QDomDocument q_DomDocument;
	//QFile q_File;
	QDomElement q_RootNode;
	
	QString q_strFilePath;
};

#endif // SYSCONFIG_H
