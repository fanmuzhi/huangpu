#include "Syn_LocalSettingsDlg.h"
#include "ui_Syn_LocalSettingsDlg.h"

Syn_LocalSettingsDlg::Syn_LocalSettingsDlg(QWidget *parent)
: QWidget(parent)
, _UpdateTag(false)
{
	ui = new Ui::Syn_LocalSettingsDlg();
	ui->setupUi(this);

	setWindowFlags(Qt::WindowCloseButtonHint);

	ui->SiteCountsLineEdit->setValidator(new QIntValidator(0, 20, this));

	ui->SiteTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui->SiteTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//ui->SiteTableWidget->setSelectionMode(QAbstractItemView::SingleSelection); 
	ui->SiteTableWidget->verticalHeader()->setVisible(false);
	ui->SiteTableWidget->horizontalHeader()->setStretchLastSection(true);
}

Syn_LocalSettingsDlg::~Syn_LocalSettingsDlg()
{
	delete ui;
}