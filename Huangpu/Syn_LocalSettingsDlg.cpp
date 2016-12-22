#include "Syn_LocalSettingsDlg.h"
#include "ui_Syn_LocalSettingsDlg.h"
#include <QtWidgets>

#include "syn_bridge.h"

Syn_LocalSettingsDlg::Syn_LocalSettingsDlg(QWidget *parent)
: QDialog(parent)
, _pSynDeviceManager(NULL)
, _typeChanged(false)
{
	ui = new Ui::Syn_LocalSettingsDlg();
	ui->setupUi(this);

	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui->SiteCountsLineEdit->setValidator(new QIntValidator(0, 20, this));

	ui->SiteTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui->SiteTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//ui->SiteTableWidget->setSelectionMode(QAbstractItemView::SingleSelection); 
	ui->SiteTableWidget->verticalHeader()->setVisible(false);
	ui->SiteTableWidget->horizontalHeader()->setStretchLastSection(true);

	//localSettings slots
	QObject::connect(ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui->SelectSysConfigFilePushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(ui->SelectLogFilePushButton, SIGNAL(clicked()), this, SLOT(SelectLogFilePath()));
	QObject::connect(ui->SiteCountsLineEdit, SIGNAL(editingFinished()), this, SLOT(ModifySiteCounts()));//returnPressed
	QObject::connect(ui->ModifySerialNumberPushButton, SIGNAL(clicked()), this, SLOT(CreateSerialNumberManageDialog()));
	QObject::connect(ui->UpdateADCOffsetsPushButton, SIGNAL(clicked()), this, SLOT(CreateUpdateADCOffsetsDlg()));
	QObject::connect(ui->SiteTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SetLeds(int, int)));
	QObject::connect(ui->OKPushButton, SIGNAL(clicked()), this, SLOT(ConfirmSite()));

	QObject::connect(ui->DeviceTypeComboBox, SIGNAL(currentIndexChanged(int index)), this, SLOT(TypeChanged(int index)));

	//load localsetting
	Syn_LocalSettingConfig *pSyn_LocalSettingConfig = NULL;
	Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Read,pSyn_LocalSettingConfig);
	if (NULL == pSyn_LocalSettingConfig)
	{
		Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Write, pSyn_LocalSettingConfig);
	}
	pSyn_LocalSettingConfig->GetLocalSettings(_LocalSettingsInfo);
	delete pSyn_LocalSettingConfig;
	pSyn_LocalSettingConfig = NULL;

	//operation
	ui->SysConfigFileLlineEdit->clear();
	ui->SysConfigFileLlineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath));

	ui->LogFileLineEdit->clear();
	ui->LogFileLineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strLogFilePath));

	unsigned int iLocalSettingSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	ui->SiteCountsLineEdit->clear();
	ui->SiteCountsLineEdit->setText(QString::number(iLocalSettingSiteCounts));

	ui->SiteTableWidget->setRowCount(iLocalSettingSiteCounts);
	for (size_t i = 0; i < iLocalSettingSiteCounts; i++)
	{
		AdcBaseLineInfo CurrentAdcBaseLineInfo = (_LocalSettingsInfo._listOfSiteSettings)[i]._adcBaseLineInfo;
		QString strVoltagesAll = QString::number(CurrentAdcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVio) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVddh);

		QString strAdcBaseLinesValue("");
		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			strAdcBaseLinesValue += QString::number((CurrentAdcBaseLineInfo.m_arrAdcBaseLines)[a]) + QString(" ");
		}

		QString strSiteNumber(QString::number(i+1));
		QString strSerialNumber(QString::fromStdString((_LocalSettingsInfo._listOfSiteSettings)[i]._strDutSerNum));

		ui->SiteTableWidget->setItem(i, 0, new QTableWidgetItem(strSiteNumber));
		ui->SiteTableWidget->setItem(i, 1, new QTableWidgetItem(strSerialNumber));
		ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));

		_TempVoltagesValue.nVdd = CurrentAdcBaseLineInfo.m_nVdd;
		_TempVoltagesValue.nVio = CurrentAdcBaseLineInfo.m_nVio;
		_TempVoltagesValue.nVled = CurrentAdcBaseLineInfo.m_nVled;
		_TempVoltagesValue.nVddh = CurrentAdcBaseLineInfo.m_nVddh;
	}

	if (NULL == _pSynDeviceManager)
	{
		_pSynDeviceManager = new syn_devicemanager();
	}

	QString strDeviceType = QString::fromStdString(_LocalSettingsInfo._strDeviceType);
	devicetype Type;
	if ("MPC04" == strDeviceType.toUpper())
	{
		Type = spi_mpc04;
		ui->DeviceTypeComboBox->setCurrentIndex(1);
	}
	else
	{
		Type = spi_m5;
	}
	uint32_t uiResult = _pSynDeviceManager->Connect(Type);

}

Syn_LocalSettingsDlg::~Syn_LocalSettingsDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
	if (NULL != _pSynDeviceManager)
	{
		delete _pSynDeviceManager;
		_pSynDeviceManager = NULL;
	}
}

void Syn_LocalSettingsDlg::ConfirmSite()
{
	bool rc(false);

	//check Site Info first
	int iUserDefineSiteCounts = ui->SiteTableWidget->rowCount();
	if (0 == iUserDefineSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Site counts is 0!"));
		return;
	}

	std::vector<string> listOfCurrentSerialNumber;
	for (int i = 0; i < iUserDefineSiteCounts; i++)
	{
		QString strSerialNumber("");
		QString strAdcValue("");

		QTableWidgetItem *pSerialNumberItem = ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = ui->SiteTableWidget->item(i, 2);
		if (NULL != pAdcValueItem)
			strAdcValue = pAdcValueItem->text();

		if (0 == strSerialNumber.size())
		{
			QMessageBox::critical(this, QString("Error"), QString("Site ")+QString::number(i+1)+QString("'s SerialNumber is 0!"));
			return;
		}

		bool IsExists(false);
		for (int j = 1; j <= listOfCurrentSerialNumber.size(); j++)
		{
			if (strSerialNumber.toStdString() == listOfCurrentSerialNumber[j - 1])
			{
				IsExists = true;
				break;
			}
		}
		if (IsExists)
		{
			QMessageBox::critical(this, QString("Error"), QString("Site ") + QString::number(i + 1) + QString("'s SerialNumber has been used!"));
			return;
		}

		listOfCurrentSerialNumber.push_back(strSerialNumber.toStdString());
	}

	//update LocalSettings info
	QString strConfigFilePath = ui->SysConfigFileLlineEdit->text();
	QFile configfile(strConfigFilePath);
	if (!configfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Config file is not exists,check it please!"));
		return;
	}
	_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();

	QString strLogFilePath = ui->LogFileLineEdit->text();
	QFile Logfile(strLogFilePath);
	if (!Logfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("LogFile Path is not exists,check it please!"));
		return;
	}
	_LocalSettingsInfo._strLogFilePath = strLogFilePath.toStdString();

	QString strDeviceType = ui->DeviceTypeComboBox->itemText(ui->DeviceTypeComboBox->currentIndex());
	_LocalSettingsInfo._strDeviceType = strDeviceType.toStdString();

	//Set SiteInfo
	//fill current info to LocalSettings Config
	_LocalSettingsInfo._listOfSiteSettings.clear();

	for (int i = 0; i < iUserDefineSiteCounts; i++)
	{
		QString strSerialNumber("");
		QString strAdcValue("");
		QTableWidgetItem *pSerialNumberItem = ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = ui->SiteTableWidget->item(i, 2);
		if (NULL != pAdcValueItem)
			strAdcValue = pAdcValueItem->text();

		SiteSettings CurrentSiteSettings;
		CurrentSiteSettings._strDutSerNum = strSerialNumber.toStdString();

		//TODO
		//need modify...................................................................................
		AdcBaseLineInfo CurrentAdcBaseLineInfo;
		QStringList strListOfAdcBaseLinesValue = strAdcValue.split(QString(" "));
		if (NUM_CURRENT_VALUES > strListOfAdcBaseLinesValue.size())
		{
			continue;
		}

		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			(CurrentAdcBaseLineInfo.m_arrAdcBaseLines)[a] = strListOfAdcBaseLinesValue[a].toInt();
		}

		CurrentAdcBaseLineInfo.m_nVdd = _TempVoltagesValue.nVdd;
		CurrentAdcBaseLineInfo.m_nVio = _TempVoltagesValue.nVio;
		CurrentAdcBaseLineInfo.m_nVled = _TempVoltagesValue.nVled;
		CurrentAdcBaseLineInfo.m_nVddh = _TempVoltagesValue.nVddh;

		CurrentSiteSettings._adcBaseLineInfo = CurrentAdcBaseLineInfo;
		_LocalSettingsInfo._listOfSiteSettings.push_back(CurrentSiteSettings);
	}

	//save
	Syn_LocalSettingConfig *pLocalSettingsInstance = NULL;
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Write,pLocalSettingsInstance);
	if (rc&&NULL != pLocalSettingsInstance)
	{
		pLocalSettingsInstance->SetLocalSettings(_LocalSettingsInfo);
		delete pLocalSettingsInstance;
		pLocalSettingsInstance = NULL;
	}

	this->close();
}

void Syn_LocalSettingsDlg::SelectConfigFile()
{
	QString strConfigFilePath = QFileDialog::getOpenFileName(this, "Select Config File", "", "xml file(*.xml)");
	if (QString("") != strConfigFilePath)
	{
		QFile TempFile(strConfigFilePath);
		if (!TempFile.exists())
			return;

		ui->SysConfigFileLlineEdit->clear();
		ui->SysConfigFileLlineEdit->setText(strConfigFilePath);

		_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();
	}
}

void Syn_LocalSettingsDlg::SelectLogFilePath()
{
	QString strLogFileFolderPath = QFileDialog::getExistingDirectory(this, "Select LogFile Path");
	if (QString("") != strLogFileFolderPath)
	{
		QFile TempFile(strLogFileFolderPath);
		if (!TempFile.exists())
			return;

		ui->LogFileLineEdit->clear();
		ui->LogFileLineEdit->setText(strLogFileFolderPath);

		_LocalSettingsInfo._strLogFilePath = strLogFileFolderPath.toStdString();
	}
}

void Syn_LocalSettingsDlg::ModifySiteCounts()
{
	if (NULL == _pSynDeviceManager)
	{
		return;
	}

	QString strUserSiteCounts = ui->SiteCountsLineEdit->text();
	int iUserSiteCounts = strUserSiteCounts.toInt();

	devicetype Type;
	QString strDeviceType = ui->DeviceTypeComboBox->itemText(ui->DeviceTypeComboBox->currentIndex());
	if ("MPC04" == strDeviceType.toUpper())
		Type = spi_mpc04;
	else
		Type = spi_m5;

	_pSynDeviceManager->Connect(Type);
	std::vector<std::string> listOfSerialNumber;
	_pSynDeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (iUserSiteCounts > listOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Site Numbers is more than real Device conunts(") + QString::number(listOfSerialNumber.size()) + QString(")!"));
		return;
	}

	int iCurrentRowCounts = ui->SiteTableWidget->rowCount();
	if (iCurrentRowCounts < iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts+1; i <= iUserSiteCounts; i++)
		{
			ui->SiteTableWidget->insertRow(i-1);
			ui->SiteTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
			ui->SiteTableWidget->setItem(i - 1, 1, new QTableWidgetItem(QString("0")));
			ui->SiteTableWidget->setItem(i - 1, 2, new QTableWidgetItem("0 0 0 0 "));
		}
	}
	else if (iCurrentRowCounts > iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts; i > iUserSiteCounts; i--)
		{
			ui->SiteTableWidget->removeRow(i - 1);
		}
	}
}

void Syn_LocalSettingsDlg::SetLeds(int rowNumber, int columnNumber)
{
	return;
}

void Syn_LocalSettingsDlg::CreateSerialNumberManageDialog()
{
	Syn_SerialNumberManageDlg *_pSyn_SerialNumberManageDlg = new Syn_SerialNumberManageDlg(this);
	_pSyn_SerialNumberManageDlg->setAttribute(Qt::WA_DeleteOnClose);
	_pSyn_SerialNumberManageDlg->show();
}

void Syn_LocalSettingsDlg::GetSerialNumberList(std::vector<std::string> &listOfSerialNumber)
{

	if (NULL == _pSynDeviceManager)
	{
		return;
	}

	devicetype Type;
	QString strDeviceType = ui->DeviceTypeComboBox->itemText(ui->DeviceTypeComboBox->currentIndex());
	if ("MPC04" == strDeviceType.toUpper())
		Type = spi_mpc04;
	else
		Type = spi_m5;

	_pSynDeviceManager->Connect(Type);
	_pSynDeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (0 == listOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't find device,check it please!"));
		return;
	}
}

void Syn_LocalSettingsDlg::SetSerialNumberForSite(const QString strSerialNumber)
{
	int iSiteRowIndex = ui->SiteTableWidget->currentRow();
	if (iSiteRowIndex < 0)
		iSiteRowIndex = 0;
	ui->SiteTableWidget->setItem(iSiteRowIndex, 1, new QTableWidgetItem(strSerialNumber));
}

void Syn_LocalSettingsDlg::CreateUpdateADCOffsetsDlg()
{

	Syn_UpdateADCOffsetsDlg *_pSyn_UpdateADCOffsetsDlg = new Syn_UpdateADCOffsetsDlg(this);
	_pSyn_UpdateADCOffsetsDlg->setAttribute(Qt::WA_DeleteOnClose);
	_pSyn_UpdateADCOffsetsDlg->show();
}

void Syn_LocalSettingsDlg::UpdateADCOffsets(int Vdd, int Vio, int Vled, int Vddh)
{
	int iSiteCounts = ui->SiteTableWidget->rowCount();
	if (0 == iSiteCounts)
	{
		return;
	}
	_TempVoltagesValue.nVdd  = Vdd;
	_TempVoltagesValue.nVio  = Vio;
	_TempVoltagesValue.nVled = Vled;
	_TempVoltagesValue.nVddh = Vddh;

	delete _pSynDeviceManager; _pSynDeviceManager = NULL;
	devicetype Type;
	uint32_t clockRate(MPC04_CLOCKRATE);
	QString strDeviceType = ui->DeviceTypeComboBox->itemText(ui->DeviceTypeComboBox->currentIndex());
	if ("MPC04" == strDeviceType.toUpper())
	{
		Type = spi_mpc04;
		clockRate = MPC04_CLOCKRATE;
	}
	else
	{
		Type = spi_m5;
		clockRate = M5_CLOCKRATE;
	}

	uint32_t rc = 0;
	for (int i = 0; i < iSiteCounts; i++)
	{
		QString strSerialNumber("");
		QTableWidgetItem *item = ui->SiteTableWidget->item(i, 1);
		if (NULL != item)
		{
			strSerialNumber = item->text();
			uint32_t arrAdcBaseLines[NUM_CURRENT_VALUES] = { 0 };

			syn_bridge *pBridge = NULL;
			rc = syn_bridge::CreateDeviceInstance(strSerialNumber.toStdString(), Type, pBridge);
			if (0 == rc&&NULL != pBridge)
			{
				pBridge->SetPortSPI(clockRate);
				pBridge->SetVoltages(Vddh,Vdd);

				uint32_t arrLowGain[2] = { 2 };
				uint32_t arrHighGain[2] = { 2 };
				rc = pBridge->GetCurrentValues(arrLowGain);
				rc = pBridge->GetCurrentValues(arrHighGain,false);

				QString strAdcBaseLinesValue("");
				strAdcBaseLinesValue = QString::number(arrLowGain[0]) + QString(" ") + QString::number(arrLowGain[1]) + QString(" ")+
										QString::number(arrHighGain[0]) + QString(" ") + QString::number(arrHighGain[1]) + QString(" ");

				pBridge->SetVoltages(0, 0);
				delete pBridge;
				pBridge = NULL;

				ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));
			}
		}
	}

	_pSynDeviceManager = new syn_devicemanager();
	rc = _pSynDeviceManager->Connect(Type);
}

int Syn_LocalSettingsDlg::GetSiteRowIndex()
{ 
	if (NULL != ui)
	{
		return ui->SiteTableWidget->currentRow(); 
	}
	else
	{
		return 0;
	}
}

void Syn_LocalSettingsDlg::TypeChanged(int index)
{

}