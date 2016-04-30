#ifndef SYN_UPDATEADCOFFSETSDLG_H
#define SYN_UPDATEADCOFFSETSDLG_H

#include <QWidget>
#include <QtWidgets/QDialog>
#include "ui_Syn_UpdateADCOffsetsDlg.h"
#include "Syn_LocalSettingsDlg.h"

class Syn_UpdateADCOffsetsDlg : public QDialog
{
	Q_OBJECT

public:
	Syn_UpdateADCOffsetsDlg(QWidget *parent = 0);
	~Syn_UpdateADCOffsetsDlg();

	//void closeEvent(QCloseEvent * event);

public Q_SLOTS:

	void UpdateADC();

private:

	Ui::Syn_UpdateADCOffsetsDlg *ui;

};

#endif // SYN_UPDATEADCOFFSETSDLG_H
