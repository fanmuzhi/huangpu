#include "Syn_BinCodesDlg.h"

Syn_BinCodesDlg::Syn_BinCodesDlg(QWidget *parent)
	:QDialog(parent)
{
	ui = new Ui::Syn_BinCodesDlg();
	
	ui->setupUi(this);

	ui->BinCodesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->BinCodesTableWidget->resizeColumnsToContents();

	QObject::connect(ui->ClosePushButton, SIGNAL(clicked()), this, SLOT(close()));
}

Syn_BinCodesDlg::~Syn_BinCodesDlg()
{
	delete ui;
	ui = NULL;
}

//void Syn_BinCodesDlg::closeEvent(QCloseEvent * event)
//{
//}