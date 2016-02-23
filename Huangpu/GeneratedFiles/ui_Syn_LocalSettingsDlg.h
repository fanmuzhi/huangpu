/********************************************************************************
** Form generated from reading UI file 'Syn_LocalSettingsDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_LOCALSETTINGSDLG_H
#define UI_SYN_LOCALSETTINGSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_LocalSettingsDlg
{
public:
    QGridLayout *gridLayout_8;
    QGroupBox *OptionsGroupBox;
    QGridLayout *gridLayout_5;
    QGroupBox *ModeGroupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *VerboseLogCheckBox;
    QCheckBox *QAModeCheckBox;
    QCheckBox *LGAModecheckBox;
    QGroupBox *AutoRepeatGroupBox;
    QGridLayout *gridLayout_3;
    QCheckBox *AutoRepeatEnabledCheckBox;
    QGroupBox *AutoControllerGroupBox;
    QGridLayout *gridLayout_4;
    QComboBox *AutoControllerComboBox;
    QGroupBox *SysConfigFileGroupBox;
    QGridLayout *gridLayout;
    QLineEdit *SysConfigFileLlineEdit;
    QPushButton *SelectSysConfigFilePushButton;
    QGroupBox *SiteGroupBox;
    QGridLayout *gridLayout_6;
    QTableWidget *SiteTableWidget;
    QPushButton *UpdateSitePushButton;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QLabel *SiteCountsSettingLabel;
    QLineEdit *SiteCountsLineEdit;
    QFrame *frame;
    QGridLayout *gridLayout_7;
    QPushButton *CancelPushButton;
    QPushButton *OKPushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Syn_LocalSettingsDlg)
    {
        if (Syn_LocalSettingsDlg->objectName().isEmpty())
            Syn_LocalSettingsDlg->setObjectName(QStringLiteral("Syn_LocalSettingsDlg"));
        Syn_LocalSettingsDlg->setWindowModality(Qt::ApplicationModal);
        Syn_LocalSettingsDlg->resize(489, 632);
        gridLayout_8 = new QGridLayout(Syn_LocalSettingsDlg);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        OptionsGroupBox = new QGroupBox(Syn_LocalSettingsDlg);
        OptionsGroupBox->setObjectName(QStringLiteral("OptionsGroupBox"));
        gridLayout_5 = new QGridLayout(OptionsGroupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        ModeGroupBox = new QGroupBox(OptionsGroupBox);
        ModeGroupBox->setObjectName(QStringLiteral("ModeGroupBox"));
        gridLayout_2 = new QGridLayout(ModeGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        VerboseLogCheckBox = new QCheckBox(ModeGroupBox);
        VerboseLogCheckBox->setObjectName(QStringLiteral("VerboseLogCheckBox"));

        gridLayout_2->addWidget(VerboseLogCheckBox, 0, 0, 1, 1);

        QAModeCheckBox = new QCheckBox(ModeGroupBox);
        QAModeCheckBox->setObjectName(QStringLiteral("QAModeCheckBox"));

        gridLayout_2->addWidget(QAModeCheckBox, 0, 1, 1, 1);

        LGAModecheckBox = new QCheckBox(ModeGroupBox);
        LGAModecheckBox->setObjectName(QStringLiteral("LGAModecheckBox"));

        gridLayout_2->addWidget(LGAModecheckBox, 0, 2, 1, 1);


        gridLayout_5->addWidget(ModeGroupBox, 0, 0, 1, 1);

        AutoRepeatGroupBox = new QGroupBox(OptionsGroupBox);
        AutoRepeatGroupBox->setObjectName(QStringLiteral("AutoRepeatGroupBox"));
        gridLayout_3 = new QGridLayout(AutoRepeatGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        AutoRepeatEnabledCheckBox = new QCheckBox(AutoRepeatGroupBox);
        AutoRepeatEnabledCheckBox->setObjectName(QStringLiteral("AutoRepeatEnabledCheckBox"));

        gridLayout_3->addWidget(AutoRepeatEnabledCheckBox, 0, 0, 1, 1);


        gridLayout_5->addWidget(AutoRepeatGroupBox, 1, 0, 1, 1);

        AutoControllerGroupBox = new QGroupBox(OptionsGroupBox);
        AutoControllerGroupBox->setObjectName(QStringLiteral("AutoControllerGroupBox"));
        gridLayout_4 = new QGridLayout(AutoControllerGroupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        AutoControllerComboBox = new QComboBox(AutoControllerGroupBox);
        AutoControllerComboBox->setObjectName(QStringLiteral("AutoControllerComboBox"));

        gridLayout_4->addWidget(AutoControllerComboBox, 0, 0, 1, 1);


        gridLayout_5->addWidget(AutoControllerGroupBox, 2, 0, 1, 1);


        gridLayout_8->addWidget(OptionsGroupBox, 1, 0, 1, 1);

        SysConfigFileGroupBox = new QGroupBox(Syn_LocalSettingsDlg);
        SysConfigFileGroupBox->setObjectName(QStringLiteral("SysConfigFileGroupBox"));
        gridLayout = new QGridLayout(SysConfigFileGroupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        SysConfigFileLlineEdit = new QLineEdit(SysConfigFileGroupBox);
        SysConfigFileLlineEdit->setObjectName(QStringLiteral("SysConfigFileLlineEdit"));
        SysConfigFileLlineEdit->setReadOnly(true);

        gridLayout->addWidget(SysConfigFileLlineEdit, 0, 0, 1, 1);

        SelectSysConfigFilePushButton = new QPushButton(SysConfigFileGroupBox);
        SelectSysConfigFilePushButton->setObjectName(QStringLiteral("SelectSysConfigFilePushButton"));

        gridLayout->addWidget(SelectSysConfigFilePushButton, 0, 1, 1, 1);


        gridLayout_8->addWidget(SysConfigFileGroupBox, 0, 0, 1, 1);

        SiteGroupBox = new QGroupBox(Syn_LocalSettingsDlg);
        SiteGroupBox->setObjectName(QStringLiteral("SiteGroupBox"));
        gridLayout_6 = new QGridLayout(SiteGroupBox);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        SiteTableWidget = new QTableWidget(SiteGroupBox);
        if (SiteTableWidget->columnCount() < 3)
            SiteTableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        SiteTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        SiteTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        SiteTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        SiteTableWidget->setObjectName(QStringLiteral("SiteTableWidget"));

        gridLayout_6->addWidget(SiteTableWidget, 1, 0, 1, 1);

        UpdateSitePushButton = new QPushButton(SiteGroupBox);
        UpdateSitePushButton->setObjectName(QStringLiteral("UpdateSitePushButton"));

        gridLayout_6->addWidget(UpdateSitePushButton, 2, 0, 1, 1);

        frame_2 = new QFrame(SiteGroupBox);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        SiteCountsSettingLabel = new QLabel(frame_2);
        SiteCountsSettingLabel->setObjectName(QStringLiteral("SiteCountsSettingLabel"));

        horizontalLayout->addWidget(SiteCountsSettingLabel);

        SiteCountsLineEdit = new QLineEdit(frame_2);
        SiteCountsLineEdit->setObjectName(QStringLiteral("SiteCountsLineEdit"));

        horizontalLayout->addWidget(SiteCountsLineEdit);


        gridLayout_6->addWidget(frame_2, 0, 0, 1, 1);


        gridLayout_8->addWidget(SiteGroupBox, 2, 0, 1, 1);

        frame = new QFrame(Syn_LocalSettingsDlg);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frame);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        CancelPushButton = new QPushButton(frame);
        CancelPushButton->setObjectName(QStringLiteral("CancelPushButton"));

        gridLayout_7->addWidget(CancelPushButton, 0, 2, 1, 1);

        OKPushButton = new QPushButton(frame);
        OKPushButton->setObjectName(QStringLiteral("OKPushButton"));

        gridLayout_7->addWidget(OKPushButton, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 0, 0, 1, 1);


        gridLayout_8->addWidget(frame, 3, 0, 1, 1);


        retranslateUi(Syn_LocalSettingsDlg);

        QMetaObject::connectSlotsByName(Syn_LocalSettingsDlg);
    } // setupUi

    void retranslateUi(QWidget *Syn_LocalSettingsDlg)
    {
        Syn_LocalSettingsDlg->setWindowTitle(QApplication::translate("Syn_LocalSettingsDlg", "Local Settings", 0));
        OptionsGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Options", 0));
        ModeGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Mode", 0));
        VerboseLogCheckBox->setText(QApplication::translate("Syn_LocalSettingsDlg", "Verbose Log", 0));
        QAModeCheckBox->setText(QApplication::translate("Syn_LocalSettingsDlg", "QA Mode", 0));
        LGAModecheckBox->setText(QApplication::translate("Syn_LocalSettingsDlg", "LGA Mode", 0));
        AutoRepeatGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Auto Repeat", 0));
        AutoRepeatEnabledCheckBox->setText(QApplication::translate("Syn_LocalSettingsDlg", "Enabled", 0));
        AutoControllerGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Auto Controller", 0));
        SysConfigFileGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Config File", 0));
        SelectSysConfigFilePushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Select", 0));
        SiteGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Site", 0));
        QTableWidgetItem *___qtablewidgetitem = SiteTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Syn_LocalSettingsDlg", "Site Index", 0));
        QTableWidgetItem *___qtablewidgetitem1 = SiteTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Syn_LocalSettingsDlg", "Serial Numebr", 0));
        QTableWidgetItem *___qtablewidgetitem2 = SiteTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Syn_LocalSettingsDlg", "ADC Offsets", 0));
        UpdateSitePushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Update", 0));
        SiteCountsSettingLabel->setText(QApplication::translate("Syn_LocalSettingsDlg", "Number of Sites:", 0));
        CancelPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Cancel", 0));
        OKPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_LocalSettingsDlg: public Ui_Syn_LocalSettingsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_LOCALSETTINGSDLG_H
