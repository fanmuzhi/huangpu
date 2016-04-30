#include "Syn_UpdateFirmwareProcessDlg.h"
#include <windows.h>

Syn_UpdateFirmwareProcessDlg::Syn_UpdateFirmwareProcessDlg(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::Syn_UpdateFirmwareProcessDlg();

	ui->setupUi(this);

	//this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

	//ui->UpdateFirmwareLabel->show();

	//this->hide();
}

void Syn_UpdateFirmwareProcessDlg::showEvent(QShowEvent * event)
{
	Syn_LocalSettingsDlg *pLocalSettingsDlg = (Syn_LocalSettingsDlg*)parentWidget();
	pLocalSettingsDlg->_pSyn_DeviceManager->UpdateFirmware();
	::Sleep(2);
	this->close();
}

//void Syn_UpdateFirmwareProcessDlg::UpdateFW()
//{
//}

Syn_UpdateFirmwareProcessDlg::~Syn_UpdateFirmwareProcessDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
}