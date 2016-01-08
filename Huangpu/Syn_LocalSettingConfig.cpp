//Local
#include "Syn_LocalSettingConfig.h"

//Qt
#include <QTextStream>

//std
#include <iostream>

Syn_LocalSettingConfig::Syn_LocalSettingConfig(QString strConfigFilePath)
:_strFilePath(strConfigFilePath)
, q_DomDocument("Syn_LocalSettingXML")
, q_DomRootNode()
{
}

Syn_LocalSettingConfig::~Syn_LocalSettingConfig()
{
}

bool Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig * &opLSConfigInstance, QString strXMLFilePath)
{
	opLSConfigInstance = NULL;

	if (QString("") == strXMLFilePath)
	{
		cout << "Error:Syn_LocalSettingConfig::CreateLSConfigInstance() - strXMLFilePath is NULL!" << endl;
		return false;
	}

	opLSConfigInstance = new Syn_LocalSettingConfig(strXMLFilePath);
	bool rc = opLSConfigInstance->Initialize();
	if (!rc)
	{
		delete opLSConfigInstance;
		opLSConfigInstance = NULL;
	}

	return rc;
}

bool Syn_LocalSettingConfig::Initialize()
{
	QFile xmlFile(_strFilePath);
	//q_File.
	if (xmlFile.exists())
	{
		if (!xmlFile.open(QFile::ReadOnly | QFile::Text))
		{
			cout << "Syn_LocalSettingConfig::Initialize() - xmlFile.open is failed!" << endl;
			return false;
		}

		QString error;
		int row = 0, column = 0;
		if (!q_DomDocument.setContent(&xmlFile, false, &error, &row, &column))
		{
			cout << "Syn_LocalSettingConfig::Initialize() - q_DomDocument.setContent is failed!" << endl;
			return false;
		}

		if (q_DomDocument.isNull())
		{
			cout << "Syn_LocalSettingConfig::Initialize() - q_DomDocument is null!" << endl;
			return false;
		}

		QDomElement root = q_DomDocument.documentElement();
		q_DomRootNode = root;

		xmlFile.close();
	}
	else
	{
		QDomProcessingInstruction instruction = q_DomDocument.createProcessingInstruction(QString("xml"),QString("version = \"1.0\" encoding = \"UTF-8\""));
		q_DomDocument.appendChild(instruction);

		/*QDomElement root = q_DomDocument.createElement("LocalSettings");
		q_DomDocument.appendChild(root);

		q_DomRootNode = root;

		QFile qFile(_strFilePath);
		if (!qFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
			return false;

		QTextStream qTextStream(&qFile);

		qTextStream.setCodec("UTF-8");

		q_DomDocument.save(qTextStream, 4, QDomNode::EncodingFromTextStream);

		qFile.close();*/
	}

	return true;
}

bool Syn_LocalSettingConfig::GetLocalSettings(Syn_LocalSettings &oSyn_LocalSettings)
{
	bool rc(true);

	QDomElement rootNode = q_DomDocument.documentElement();
	if (rootNode.isNull())
	{
		cout << "Error:Syn_LocalSettingConfig::GetLocalSettings() - rootNode is null!" << endl;
		return false;
	}

	QDomNodeList ListOfChildNode = rootNode.childNodes();
	if (ListOfChildNode.isEmpty())
	{
		cout << "Error:Syn_LocalSettingConfig::GetLocalSettings() - ListOfChildNode is null!" << endl;
		return false;
	}

	oSyn_LocalSettings._listOfSiteSettings.clear();
	int iLength = ListOfChildNode.length();
	for (int i = 1; i <= iLength; i++)
	{
		QDomNode dom_node = ListOfChildNode.item(i - 1);
		QDomElement element = dom_node.toElement();

		QString strTagName(element.tagName());
		if (QString("SysConfigFile") == strTagName)
		{
			oSyn_LocalSettings._strSysConfigFilePath = element.text().toStdString();
		}
		else if (QString("AutoController") == strTagName)
		{
			oSyn_LocalSettings._strAutoController = element.text().toStdString();
		}
		else if (QString("Site") == strTagName.left(4))
		{
			SiteSettings CurrentSiteSettings;

			uint32_t uiSerialNumber = element.text().toUInt();
			CurrentSiteSettings._uiDutSerNum = uiSerialNumber;
			
			AdcBaseLineInfo CurrentAdcBaseLineInfo;
			QString strVoltagesValue = element.attribute(QString("Voltages"));
			QStringList strListOfVoltagesValue = strVoltagesValue.split(QString(" "));
			if (4 == strListOfVoltagesValue.size())
			{
				CurrentAdcBaseLineInfo.m_nVdd = strListOfVoltagesValue[0].toUInt();
				CurrentAdcBaseLineInfo.m_nVio = strListOfVoltagesValue[1].toUInt();
				CurrentAdcBaseLineInfo.m_nVled = strListOfVoltagesValue[2].toUInt();
				CurrentAdcBaseLineInfo.m_nVddh = strListOfVoltagesValue[3].toUInt();
			}

			QString strAdcBaseLinesValue = element.attribute(QString("AdcBaseLines"));
			QStringList strListOfAdcBaseLinesValue = strAdcBaseLinesValue.split(QString(" "));
			QString strTempValue(strListOfAdcBaseLinesValue[strListOfAdcBaseLinesValue.size() - 1]);
			if (QString(" ") == strTempValue || QString("") == strTempValue)
			{
				strListOfAdcBaseLinesValue.removeLast();
			}
			int AdcBaseLinesSize = strListOfAdcBaseLinesValue.size();
			if ((NUM_CURRENT_VALUES*KNUMGAINS) == AdcBaseLinesSize)
			{
				for (int a = 0; a < NUM_CURRENT_VALUES; a++)
				{
					for (int b = 0; b < KNUMGAINS; b++)
					{
						uint32_t iTempValue = strListOfAdcBaseLinesValue[(NUM_CURRENT_VALUES)*a + b ].toInt();
						(CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b] = iTempValue;
					}
				}
			}

			CurrentSiteSettings._adcBaseLineInfo = CurrentAdcBaseLineInfo;

			oSyn_LocalSettings._listOfSiteSettings.push_back(CurrentSiteSettings);
		}
		else
		{
			continue;
		}
	}

	return true;
}

bool Syn_LocalSettingConfig::SetLocalSettings(Syn_LocalSettings iSyn_LocalSettings)
{
	bool rc(true);

	//delete all
	QDomElement root = q_DomDocument.documentElement();
	if (!root.isNull())
	{
		q_DomDocument.removeChild(root);
	}

	//root
	QDomElement rootNode = q_DomDocument.createElement("LocalSettings");
	q_DomDocument.appendChild(rootNode);

	//SysConfigFile
	QDomElement SysConfigFileNode = q_DomDocument.createElement("SysConfigFile");
	QString strSysConfigFilePath(QString::fromStdString(iSyn_LocalSettings._strSysConfigFilePath));
	QDomText SysConfigFileTextNode = q_DomDocument.createTextNode(strSysConfigFilePath);
	rootNode.appendChild(SysConfigFileNode);
	SysConfigFileNode.appendChild(SysConfigFileTextNode);

	//NumSites
	int iNumSites = iSyn_LocalSettings._listOfSiteSettings.size();
	QDomElement NumSitesNode = q_DomDocument.createElement("NumSites");
	QString strNumSites(QString::number(iNumSites));
	QDomText NumSitesTextNode = q_DomDocument.createTextNode(strNumSites);
	rootNode.appendChild(NumSitesNode);
	NumSitesNode.appendChild(NumSitesTextNode);
	for (int i = 1; i <= iNumSites; i++)
	{
		SiteSettings CurrentSiteSettings = (iSyn_LocalSettings._listOfSiteSettings)[i - 1];

		QString strSiteTitle(QString("Site")+QString::number(i));
		QDomElement SiteNode = q_DomDocument.createElement(strSiteTitle);
		QString strSerialNumber(QString::number(CurrentSiteSettings._uiDutSerNum));
		QDomText SiteTextNode = q_DomDocument.createTextNode(strSerialNumber);
		rootNode.appendChild(SiteNode);
		SiteNode.appendChild(SiteTextNode);

		//AdcBaseLines:attribute
		QDomAttr qAdcBaseLinesAttribute = q_DomDocument.createAttribute(QString("AdcBaseLines"));
		QString strAdcBaseLinesValue("");
		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				strAdcBaseLinesValue += QString::number((CurrentSiteSettings._adcBaseLineInfo.m_arAdcBaseLines)[a][b])+ QString(" ");
			}
		}
		qAdcBaseLinesAttribute.setValue(strAdcBaseLinesValue);
		SiteNode.appendChild(qAdcBaseLinesAttribute);

		//Voltages:attribute
		QDomAttr qVoltagesAttribute = q_DomDocument.createAttribute(QString("Voltages"));
		QString strVoltagesValue(QString::number(CurrentSiteSettings._adcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentSiteSettings._adcBaseLineInfo.m_nVio) + QString(" ") +
									QString::number(CurrentSiteSettings._adcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentSiteSettings._adcBaseLineInfo.m_nVddh));
		
		qVoltagesAttribute.setValue(strVoltagesValue);
		SiteNode.appendChild(qVoltagesAttribute);
	}

	//AutoController
	QDomElement AutoControllerNode = q_DomDocument.createElement("AutoController");
	QString strAutoController(QString::fromStdString(iSyn_LocalSettings._strAutoController));
	QDomText AutoControllerTextNode = q_DomDocument.createTextNode(strAutoController);
	rootNode.appendChild(AutoControllerNode);
	AutoControllerNode.appendChild(AutoControllerTextNode);

	//Save
	QFile qFile(_strFilePath);
	//q_File.setFileName(_strFilePath);
	if (!qFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		return false;

	QTextStream qTextStream(&qFile);

	qTextStream.setCodec("UTF-8");

	q_DomDocument.save(qTextStream, 4, QDomNode::EncodingFromTextStream);

	qFile.close();

	return true;
}