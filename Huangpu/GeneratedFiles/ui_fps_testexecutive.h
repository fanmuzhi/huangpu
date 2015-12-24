/********************************************************************************
** Form generated from reading UI file 'fps_testexecutive.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPS_TESTEXECUTIVE_H
#define UI_FPS_TESTEXECUTIVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FPS_TestExecutiveClass
{
public:
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonPowerOn;
    QPushButton *pushButtonGetVer;
    QPushButton *pushButtonPowerOff;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FPS_TestExecutiveClass)
    {
        if (FPS_TestExecutiveClass->objectName().isEmpty())
            FPS_TestExecutiveClass->setObjectName(QStringLiteral("FPS_TestExecutiveClass"));
        FPS_TestExecutiveClass->resize(513, 470);
        openAct = new QAction(FPS_TestExecutiveClass);
        openAct->setObjectName(QStringLiteral("openAct"));
        saveAsAct = new QAction(FPS_TestExecutiveClass);
        saveAsAct->setObjectName(QStringLiteral("saveAsAct"));
        exitAct = new QAction(FPS_TestExecutiveClass);
        exitAct->setObjectName(QStringLiteral("exitAct"));
        centralWidget = new QWidget(FPS_TestExecutiveClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonPowerOn = new QPushButton(centralWidget);
        pushButtonPowerOn->setObjectName(QStringLiteral("pushButtonPowerOn"));

        horizontalLayout->addWidget(pushButtonPowerOn);

        pushButtonGetVer = new QPushButton(centralWidget);
        pushButtonGetVer->setObjectName(QStringLiteral("pushButtonGetVer"));

        horizontalLayout->addWidget(pushButtonGetVer);

        pushButtonPowerOff = new QPushButton(centralWidget);
        pushButtonPowerOff->setObjectName(QStringLiteral("pushButtonPowerOff"));

        horizontalLayout->addWidget(pushButtonPowerOff);


        verticalLayout->addLayout(horizontalLayout);

        FPS_TestExecutiveClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(FPS_TestExecutiveClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FPS_TestExecutiveClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FPS_TestExecutiveClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FPS_TestExecutiveClass->setStatusBar(statusBar);

        retranslateUi(FPS_TestExecutiveClass);

        QMetaObject::connectSlotsByName(FPS_TestExecutiveClass);
    } // setupUi

    void retranslateUi(QMainWindow *FPS_TestExecutiveClass)
    {
        FPS_TestExecutiveClass->setWindowTitle(QApplication::translate("FPS_TestExecutiveClass", "FPS_TestExecutive", 0));
        openAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Open...", 0));
        saveAsAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Save As...", 0));
        exitAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Exit", 0));
        pushButtonPowerOn->setText(QApplication::translate("FPS_TestExecutiveClass", "PowerOn", 0));
        pushButtonGetVer->setText(QApplication::translate("FPS_TestExecutiveClass", "GetVer", 0));
        pushButtonPowerOff->setText(QApplication::translate("FPS_TestExecutiveClass", "PowerOff", 0));
    } // retranslateUi

};

namespace Ui {
    class FPS_TestExecutiveClass: public Ui_FPS_TestExecutiveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPS_TESTEXECUTIVE_H
