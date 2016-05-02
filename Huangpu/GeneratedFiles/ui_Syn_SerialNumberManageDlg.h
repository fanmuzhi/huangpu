/********************************************************************************
** Form generated from reading UI file 'Syn_SerialNumberManageDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_SERIALNUMBERMANAGEDLG_H
#define UI_SYN_SERIALNUMBERMANAGEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_SerialNumberManageDlg
{
public:
    QGridLayout *gridLayout;
    QTableWidget *SerialNumberTableWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *OKPushButton;
    QPushButton *CancelPushButton;
    QLabel *SiteManagePromptLabel;

    void setupUi(QDialog *Syn_SerialNumberManageDlg)
    {
        if (Syn_SerialNumberManageDlg->objectName().isEmpty())
            Syn_SerialNumberManageDlg->setObjectName(QStringLiteral("Syn_SerialNumberManageDlg"));
        Syn_SerialNumberManageDlg->setWindowModality(Qt::ApplicationModal);
        Syn_SerialNumberManageDlg->resize(240, 300);
        gridLayout = new QGridLayout(Syn_SerialNumberManageDlg);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        SerialNumberTableWidget = new QTableWidget(Syn_SerialNumberManageDlg);
        if (SerialNumberTableWidget->columnCount() < 1)
            SerialNumberTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        SerialNumberTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        SerialNumberTableWidget->setObjectName(QStringLiteral("SerialNumberTableWidget"));
        SerialNumberTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        gridLayout->addWidget(SerialNumberTableWidget, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        OKPushButton = new QPushButton(Syn_SerialNumberManageDlg);
        OKPushButton->setObjectName(QStringLiteral("OKPushButton"));

        horizontalLayout->addWidget(OKPushButton);

        CancelPushButton = new QPushButton(Syn_SerialNumberManageDlg);
        CancelPushButton->setObjectName(QStringLiteral("CancelPushButton"));

        horizontalLayout->addWidget(CancelPushButton);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        SiteManagePromptLabel = new QLabel(Syn_SerialNumberManageDlg);
        SiteManagePromptLabel->setObjectName(QStringLiteral("SiteManagePromptLabel"));

        gridLayout->addWidget(SiteManagePromptLabel, 0, 0, 1, 1);


        retranslateUi(Syn_SerialNumberManageDlg);

        QMetaObject::connectSlotsByName(Syn_SerialNumberManageDlg);
    } // setupUi

    void retranslateUi(QDialog *Syn_SerialNumberManageDlg)
    {
        Syn_SerialNumberManageDlg->setWindowTitle(QApplication::translate("Syn_SerialNumberManageDlg", "SerialNumber Manage", 0));
        QTableWidgetItem *___qtablewidgetitem = SerialNumberTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Syn_SerialNumberManageDlg", "SerialNumber", 0));
        OKPushButton->setText(QApplication::translate("Syn_SerialNumberManageDlg", "OK", 0));
        CancelPushButton->setText(QApplication::translate("Syn_SerialNumberManageDlg", "Cancel", 0));
        SiteManagePromptLabel->setText(QApplication::translate("Syn_SerialNumberManageDlg", "SiteManage Prompt", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_SerialNumberManageDlg: public Ui_Syn_SerialNumberManageDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_SERIALNUMBERMANAGEDLG_H
