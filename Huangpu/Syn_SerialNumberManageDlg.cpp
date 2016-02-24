#include "Syn_SerialNumberManageDlg.h"

Syn_SerialNumberManageDlg::Syn_SerialNumberManageDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::WindowCloseButtonHint);

	ui.SerialNumberTableWidget->setColumnWidth(1,320);
	ui.SerialNumberTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


}

Syn_SerialNumberManageDlg::~Syn_SerialNumberManageDlg()
{

}
