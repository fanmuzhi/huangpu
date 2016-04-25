#include "Syn_UpdateFirmwareProcessDlg.h"

Syn_UpdateFirmwareProcessDlg::Syn_UpdateFirmwareProcessDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

	ui.UpdateFirmwareLabel->show();
}

Syn_UpdateFirmwareProcessDlg::~Syn_UpdateFirmwareProcessDlg()
{
}
