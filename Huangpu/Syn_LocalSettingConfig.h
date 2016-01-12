#ifndef SYN_LOCALSETTINGCONFIG_H
#define SYN_LOCALSETTINGCONFIG_H

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

//DutUtls
#include "Syn_Config.h"

using namespace std;

class Syn_LocalSettingConfig
{
public:

	static bool CreateLSConfigInstance(Syn_LocalSettingConfig * &opLSConfigInstance, QString strXMLFilePath = QString("LocalSettings.xml"));
	
	//Syn_LocalSettingConfig(QString strConfigFilePath);
	~Syn_LocalSettingConfig();

	bool GetLocalSettings(Syn_LocalSettings &oSyn_LocalSettings);

	bool SetLocalSettings(Syn_LocalSettings iSyn_LocalSettings);

private:

	Syn_LocalSettingConfig(QString strConfigFilePath);

	bool Initialize();

private:

	QString _strFilePath;

	QDomDocument q_DomDocument;

	QDomElement q_DomRootNode;
	
};

#endif // SYN_LOCALSETTINGCONFIG_H
