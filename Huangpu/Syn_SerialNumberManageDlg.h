#ifndef SYN_SERIALNUMBERMANAGEDLG_H
#define SYN_SERIALNUMBERMANAGEDLG_H

#include <QWidget>
#include <QtWidgets/QDialog>
#include "ui_Syn_SerialNumberManageDlg.h"
#include "Syn_LocalSettingsDlg.h"

class Syn_SerialNumberManageDlg : public QDialog
{
	Q_OBJECT

	//friend class Syn_LocalSettingsDlg;

public:
	Syn_SerialNumberManageDlg(QWidget *parent = 0);
	~Syn_SerialNumberManageDlg();

	void showEvent(QShowEvent * event);

public Q_SLOTS:

	void SelectSerialNumber();

private:

	Ui::Syn_SerialNumberManageDlg *ui;
};

#endif // SYN_SERIALNUMBERMANAGEDLG_H
