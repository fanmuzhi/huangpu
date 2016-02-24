#ifndef SYN_SERIALNUMBERMANAGEDLG_H
#define SYN_SERIALNUMBERMANAGEDLG_H

#include <QWidget>
#include "ui_Syn_SerialNumberManageDlg.h"

class Syn_SerialNumberManageDlg : public QWidget
{
	Q_OBJECT

	friend class FPS_TestExecutive;

public:
	Syn_SerialNumberManageDlg(QWidget *parent = 0);
	~Syn_SerialNumberManageDlg();

private:
	Ui::Syn_SerialNumberManageDlg ui;
};

#endif // SYN_SERIALNUMBERMANAGEDLG_H
