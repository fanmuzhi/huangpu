#include "Syn_BinCodesDlg.h"

Syn_BinCodesDlg::Syn_BinCodesDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.BinCodesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui.BinCodesTableWidget->resizeColumnsToContents();
}

Syn_BinCodesDlg::~Syn_BinCodesDlg()
{

}

void Syn_BinCodesDlg::closeEvent(QCloseEvent * event)
{
	ui.ClosePushButton->click();
}