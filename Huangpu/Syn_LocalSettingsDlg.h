#ifndef SYN_LOCALSETTINGSDLG_H
#define SYN_LOCALSETTINGSDLG_H

#include <QWidget>
namespace Ui {class Syn_LocalSettingsDlg;};

class Syn_LocalSettingsDlg : public QWidget
{
	Q_OBJECT

	friend class FPS_TestExecutive;

public:
	Syn_LocalSettingsDlg(QWidget *parent = 0);
	~Syn_LocalSettingsDlg();

	inline void SetUpdateTag(bool iUpdateTag){ _UpdateTag = iUpdateTag; };

	inline void GetUpdateTag(bool &oUpdateTag){ oUpdateTag = _UpdateTag; };

	void closeEvent(QCloseEvent * event);

private:
	Ui::Syn_LocalSettingsDlg *ui;

	bool _UpdateTag;
};

#endif // SYN_LOCALSETTINGSDLG_H
