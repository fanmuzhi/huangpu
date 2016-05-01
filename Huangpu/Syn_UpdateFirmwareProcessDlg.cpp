#include "Syn_UpdateFirmwareProcessDlg.h"
#include <windows.h>

Syn_UpdateFirmwareProcessDlg::Syn_UpdateFirmwareProcessDlg(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::Syn_UpdateFirmwareProcessDlg();

	ui->setupUi(this);

	//connetion type is important for updateFW after window loaded.
	connect(this, SIGNAL(window_loaded()), this, SLOT(UpdateFW()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
}

void Syn_UpdateFirmwareProcessDlg::showEvent(QShowEvent * event)
{
	QDialog::showEvent(event);
	emit window_loaded();
}


void Syn_UpdateFirmwareProcessDlg::UpdateFW()
{
	//::Sleep(5000);
	Syn_LocalSettingsDlg *pLocalSettingsDlg = (Syn_LocalSettingsDlg*)parentWidget();
	pLocalSettingsDlg->_pSyn_DeviceManager->UpdateFirmware();
}

Syn_UpdateFirmwareProcessDlg::~Syn_UpdateFirmwareProcessDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
}