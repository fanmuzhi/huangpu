/********************************************************************************
** Form generated from reading UI file 'Syn_UpdateADCOffsetsDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_UPDATEADCOFFSETSDLG_H
#define UI_SYN_UPDATEADCOFFSETSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_UpdateADCOffsetsDlg
{
public:
    QGridLayout *gridLayout;
    QGroupBox *VoltagesGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *VddLabel;
    QLabel *VioLabel;
    QLabel *VledLabel;
    QLabel *VddhLabel;
    QLineEdit *VddLineEdit;
    QLineEdit *VioLineEdit;
    QLineEdit *VledLineEdit;
    QLineEdit *VddhLineEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *OKPushButton;
    QPushButton *CancelPushButton;

    void setupUi(QWidget *Syn_UpdateADCOffsetsDlg)
    {
        if (Syn_UpdateADCOffsetsDlg->objectName().isEmpty())
            Syn_UpdateADCOffsetsDlg->setObjectName(QStringLiteral("Syn_UpdateADCOffsetsDlg"));
        Syn_UpdateADCOffsetsDlg->setWindowModality(Qt::ApplicationModal);
        Syn_UpdateADCOffsetsDlg->resize(325, 121);
        gridLayout = new QGridLayout(Syn_UpdateADCOffsetsDlg);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        VoltagesGroupBox = new QGroupBox(Syn_UpdateADCOffsetsDlg);
        VoltagesGroupBox->setObjectName(QStringLiteral("VoltagesGroupBox"));
        gridLayout_2 = new QGridLayout(VoltagesGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        VddLabel = new QLabel(VoltagesGroupBox);
        VddLabel->setObjectName(QStringLiteral("VddLabel"));

        gridLayout_2->addWidget(VddLabel, 0, 0, 1, 1);

        VioLabel = new QLabel(VoltagesGroupBox);
        VioLabel->setObjectName(QStringLiteral("VioLabel"));

        gridLayout_2->addWidget(VioLabel, 0, 1, 1, 1);

        VledLabel = new QLabel(VoltagesGroupBox);
        VledLabel->setObjectName(QStringLiteral("VledLabel"));

        gridLayout_2->addWidget(VledLabel, 0, 2, 1, 1);

        VddhLabel = new QLabel(VoltagesGroupBox);
        VddhLabel->setObjectName(QStringLiteral("VddhLabel"));

        gridLayout_2->addWidget(VddhLabel, 0, 3, 1, 1);

        VddLineEdit = new QLineEdit(VoltagesGroupBox);
        VddLineEdit->setObjectName(QStringLiteral("VddLineEdit"));

        gridLayout_2->addWidget(VddLineEdit, 1, 0, 1, 1);

        VioLineEdit = new QLineEdit(VoltagesGroupBox);
        VioLineEdit->setObjectName(QStringLiteral("VioLineEdit"));

        gridLayout_2->addWidget(VioLineEdit, 1, 1, 1, 1);

        VledLineEdit = new QLineEdit(VoltagesGroupBox);
        VledLineEdit->setObjectName(QStringLiteral("VledLineEdit"));

        gridLayout_2->addWidget(VledLineEdit, 1, 2, 1, 1);

        VddhLineEdit = new QLineEdit(VoltagesGroupBox);
        VddhLineEdit->setObjectName(QStringLiteral("VddhLineEdit"));

        gridLayout_2->addWidget(VddhLineEdit, 1, 3, 1, 1);


        gridLayout->addWidget(VoltagesGroupBox, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(142, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        OKPushButton = new QPushButton(Syn_UpdateADCOffsetsDlg);
        OKPushButton->setObjectName(QStringLiteral("OKPushButton"));

        gridLayout->addWidget(OKPushButton, 1, 1, 1, 1);

        CancelPushButton = new QPushButton(Syn_UpdateADCOffsetsDlg);
        CancelPushButton->setObjectName(QStringLiteral("CancelPushButton"));

        gridLayout->addWidget(CancelPushButton, 1, 2, 1, 1);


        retranslateUi(Syn_UpdateADCOffsetsDlg);

        QMetaObject::connectSlotsByName(Syn_UpdateADCOffsetsDlg);
    } // setupUi

    void retranslateUi(QWidget *Syn_UpdateADCOffsetsDlg)
    {
        Syn_UpdateADCOffsetsDlg->setWindowTitle(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Update ADC Offsets", 0));
        VoltagesGroupBox->setTitle(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Voltages(mV)", 0));
        VddLabel->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Vdd:", 0));
        VioLabel->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Vio:", 0));
        VledLabel->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Vled:", 0));
        VddhLabel->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Vddh:", 0));
        OKPushButton->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "OK", 0));
        CancelPushButton->setText(QApplication::translate("Syn_UpdateADCOffsetsDlg", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_UpdateADCOffsetsDlg: public Ui_Syn_UpdateADCOffsetsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_UPDATEADCOFFSETSDLG_H
