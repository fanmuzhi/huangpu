#ifndef SYN_UPDATEFIRMWAREPROCESSDLG_H
#define SYN_UPDATEFIRMWAREPROCESSDLG_H

#include <QWidget>
#include <QtWidgets/QDialog>
#include "ui_Syn_UpdateFirmwareProcessDlg.h"
#include "Syn_LocalSettingsDlg.h"

class Syn_UpdateFirmwareProcessDlg : public QDialog
{
	Q_OBJECT

public:
	Syn_UpdateFirmwareProcessDlg(QWidget *parent = 0);
	~Syn_UpdateFirmwareProcessDlg();

	void showEvent(QShowEvent * event);

private:
	Ui::Syn_UpdateFirmwareProcessDlg *ui;
	//updateFW
	//void UpdateFW();
};

#endif // SYN_UPDATEFIRMWAREPROCESSDLG_H