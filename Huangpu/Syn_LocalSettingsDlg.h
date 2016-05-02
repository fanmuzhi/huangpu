#ifndef SYN_LOCALSETTINGSDLG_H
#define SYN_LOCALSETTINGSDLG_H

#include <QWidget>
#include <QtWidgets/QDialog>

#include "Syn_Dut.h"
#include "Syn_DeviceManager.h"
#include "Syn_LocalSettingConfig.h"
#include "Syn_SerialNumberManageDlg.h"
#include "Syn_UpdateADCOffsetsDlg.h"
#include "Syn_UpdateFirmwareProcessDlg.h"
#include "Syn_Exception.h"


namespace Ui {class Syn_LocalSettingsDlg;};

struct Syn_VoltagesValue
{
	uint32_t nVdd;
	uint32_t nVio;
	uint32_t nVled;
	uint32_t nVddh;
};


class Syn_LocalSettingsDlg : public QDialog
{
	Q_OBJECT

	//friend class FPS_TestExecutive;

public:
	Syn_LocalSettingsDlg(QWidget *parent = 0);
	~Syn_LocalSettingsDlg();

	//void closeEvent(QCloseEvent * event);
	Syn_DeviceManager *_pSyn_DeviceManager;

	void GetSerialNumberList(std::vector<uint32_t> &listOfSerialNumber);
	void SetSerialNumberForSite(const QString strSerialNumber);
	void UpdateADCOffsets(int Vdd, int Vio, int Vled, int Vddh);
	int GetSiteRowIndex();

public Q_SLOTS:

	void SelectConfigFile();
	void SelectLogFilePath();
	void ModifySiteCounts();
	void SetLeds(int rowNumber, int columnNumber);
	void ConfirmSite();

	void CreateUpdateADCOffsetsDlg();
	//void CloseUpdateADCOffsetsDialog();

	//SerialNumber Manage Dialog slots
	void CreateSerialNumberManageDialog();

private:
	Ui::Syn_LocalSettingsDlg *ui;

	Syn_LocalSettings _LocalSettingsInfo;

	//Temp Voltages,save VoltagesValue from user define or configfile
	Syn_VoltagesValue _TempVoltagesValue;

	//Syn_SerialNumberManageDlg *_pSyn_SerialNumberManageDlg;
	//Syn_UpdateADCOffsetsDlg *_pSyn_UpdateADCOffsetsDlg;
	//Syn_UpdateFirmwareProcessDlg *_pSyn_UpdateFirmwareProcessDlg;
};

#endif // SYN_LOCALSETTINGSDLG_H