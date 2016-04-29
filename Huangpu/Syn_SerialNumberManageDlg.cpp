#include "Syn_SerialNumberManageDlg.h"

Syn_SerialNumberManageDlg::Syn_SerialNumberManageDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui.SerialNumberTableWidget->setColumnWidth(1,320);
	ui.SerialNumberTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::connect(ui.CancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

Syn_SerialNumberManageDlg::~Syn_SerialNumberManageDlg()
{
}

void Syn_SerialNumberManageDlg::closeEvent(QCloseEvent * event)
{
	ui.CancelPushButton->click();
}