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
#include <QtWidgets/QDialog>
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

QT_BEGIN_NAMESPACE

class Ui_Syn_LocalSettingsDlg
{
public:
    QGridLayout *gridLayout_8;
    QGroupBox *LogFileGroupBox;
    QGridLayout *gridLayout_5;
    QLineEdit *LogFileLineEdit;
    QPushButton *SelectLogFilePushButton;
    QGroupBox *SysConfigFileGroupBox;
    QGridLayout *gridLayout;
    QLineEdit *SysConfigFileLlineEdit;
    QPushButton *SelectSysConfigFilePushButton;
    QGroupBox *SiteGroupBox;
    QGridLayout *gridLayout_6;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QLabel *SiteCountsSettingLabel;
    QLineEdit *SiteCountsLineEdit;
    QTableWidget *SiteTableWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ModifySerialNumberPushButton;
    QPushButton *UpdateADCOffsetsPushButton;
    QFrame *frame;
    QGridLayout *gridLayout_7;
    QPushButton *CancelPushButton;
    QPushButton *OKPushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *Syn_LocalSettingsDlg)
    {
        if (Syn_LocalSettingsDlg->objectName().isEmpty())
            Syn_LocalSettingsDlg->setObjectName(QStringLiteral("Syn_LocalSettingsDlg"));
        Syn_LocalSettingsDlg->setWindowModality(Qt::ApplicationModal);
        Syn_LocalSettingsDlg->resize(489, 632);
        gridLayout_8 = new QGridLayout(Syn_LocalSettingsDlg);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        LogFileGroupBox = new QGroupBox(Syn_LocalSettingsDlg);
        LogFileGroupBox->setObjectName(QStringLiteral("LogFileGroupBox"));
        gridLayout_5 = new QGridLayout(LogFileGroupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        LogFileLineEdit = new QLineEdit(LogFileGroupBox);
        LogFileLineEdit->setObjectName(QStringLiteral("LogFileLineEdit"));
        LogFileLineEdit->setReadOnly(true);

        gridLayout_5->addWidget(LogFileLineEdit, 0, 0, 1, 1);

        SelectLogFilePushButton = new QPushButton(LogFileGroupBox);
        SelectLogFilePushButton->setObjectName(QStringLiteral("SelectLogFilePushButton"));

        gridLayout_5->addWidget(SelectLogFilePushButton, 0, 1, 1, 1);


        gridLayout_8->addWidget(LogFileGroupBox, 1, 0, 1, 1);

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
        SiteTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        SiteTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_6->addWidget(SiteTableWidget, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ModifySerialNumberPushButton = new QPushButton(SiteGroupBox);
        ModifySerialNumberPushButton->setObjectName(QStringLiteral("ModifySerialNumberPushButton"));

        horizontalLayout_2->addWidget(ModifySerialNumberPushButton);

        UpdateADCOffsetsPushButton = new QPushButton(SiteGroupBox);
        UpdateADCOffsetsPushButton->setObjectName(QStringLiteral("UpdateADCOffsetsPushButton"));

        horizontalLayout_2->addWidget(UpdateADCOffsetsPushButton);


        gridLayout_6->addLayout(horizontalLayout_2, 2, 0, 1, 1);


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

    void retranslateUi(QDialog *Syn_LocalSettingsDlg)
    {
        Syn_LocalSettingsDlg->setWindowTitle(QApplication::translate("Syn_LocalSettingsDlg", "Local Settings", 0));
        LogFileGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "LogFile Path", 0));
        SelectLogFilePushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Select", 0));
        SysConfigFileGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Config File", 0));
        SelectSysConfigFilePushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Select", 0));
        SiteGroupBox->setTitle(QApplication::translate("Syn_LocalSettingsDlg", "Site", 0));
        SiteCountsSettingLabel->setText(QApplication::translate("Syn_LocalSettingsDlg", "Number of Sites:", 0));
        QTableWidgetItem *___qtablewidgetitem = SiteTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Syn_LocalSettingsDlg", "Site Index", 0));
        QTableWidgetItem *___qtablewidgetitem1 = SiteTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Syn_LocalSettingsDlg", "Serial Numebr", 0));
        QTableWidgetItem *___qtablewidgetitem2 = SiteTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Syn_LocalSettingsDlg", "ADC Offsets", 0));
        ModifySerialNumberPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Modify SerialNumber", 0));
        UpdateADCOffsetsPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Update ADC Offsets", 0));
        CancelPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "Cancel", 0));
        OKPushButton->setText(QApplication::translate("Syn_LocalSettingsDlg", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_LocalSettingsDlg: public Ui_Syn_LocalSettingsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_LOCALSETTINGSDLG_H
