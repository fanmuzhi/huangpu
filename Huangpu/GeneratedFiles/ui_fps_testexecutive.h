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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QGridLayout *gridLayout_4;
    QGroupBox *SettingOptionGroupBox;
    QGridLayout *gridLayout_2;
    QPushButton *LocalSettingsPushButton;
    QGroupBox *TestGroupBox;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QTableWidget *TestTableWidget;
    QGroupBox *TestOperationGroupBox;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonRun;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBox;
    QPushButton *pushButtonGetVer;
    QPushButton *pushButtonReadOTP;
    QTextBrowser *textBrowser;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FPS_TestExecutiveClass)
    {
        if (FPS_TestExecutiveClass->objectName().isEmpty())
            FPS_TestExecutiveClass->setObjectName(QStringLiteral("FPS_TestExecutiveClass"));
        FPS_TestExecutiveClass->resize(690, 671);
        openAct = new QAction(FPS_TestExecutiveClass);
        openAct->setObjectName(QStringLiteral("openAct"));
        saveAsAct = new QAction(FPS_TestExecutiveClass);
        saveAsAct->setObjectName(QStringLiteral("saveAsAct"));
        exitAct = new QAction(FPS_TestExecutiveClass);
        exitAct->setObjectName(QStringLiteral("exitAct"));
        centralWidget = new QWidget(FPS_TestExecutiveClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        SettingOptionGroupBox = new QGroupBox(centralWidget);
        SettingOptionGroupBox->setObjectName(QStringLiteral("SettingOptionGroupBox"));
        gridLayout_2 = new QGridLayout(SettingOptionGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        LocalSettingsPushButton = new QPushButton(SettingOptionGroupBox);
        LocalSettingsPushButton->setObjectName(QStringLiteral("LocalSettingsPushButton"));

        gridLayout_2->addWidget(LocalSettingsPushButton, 0, 0, 1, 1);


        gridLayout_4->addWidget(SettingOptionGroupBox, 0, 0, 1, 1);

        TestGroupBox = new QGroupBox(centralWidget);
        TestGroupBox->setObjectName(QStringLiteral("TestGroupBox"));
        gridLayout = new QGridLayout(TestGroupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(TestGroupBox);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        TestTableWidget = new QTableWidget(tab);
        if (TestTableWidget->rowCount() < 7)
            TestTableWidget->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(6, __qtablewidgetitem6);
        TestTableWidget->setObjectName(QStringLiteral("TestTableWidget"));
        TestTableWidget->setAutoScroll(false);

        horizontalLayout->addWidget(TestTableWidget);


        verticalLayout_3->addLayout(horizontalLayout);

        TestOperationGroupBox = new QGroupBox(tab);
        TestOperationGroupBox->setObjectName(QStringLiteral("TestOperationGroupBox"));
        gridLayout_3 = new QGridLayout(TestOperationGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        pushButtonRun = new QPushButton(TestOperationGroupBox);
        pushButtonRun->setObjectName(QStringLiteral("pushButtonRun"));

        gridLayout_3->addWidget(pushButtonRun, 0, 0, 1, 1);


        verticalLayout_3->addWidget(TestOperationGroupBox);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);

        pushButtonGetVer = new QPushButton(tab_2);
        pushButtonGetVer->setObjectName(QStringLiteral("pushButtonGetVer"));

        horizontalLayout_2->addWidget(pushButtonGetVer);

        pushButtonReadOTP = new QPushButton(tab_2);
        pushButtonReadOTP->setObjectName(QStringLiteral("pushButtonReadOTP"));

        horizontalLayout_2->addWidget(pushButtonReadOTP);


        verticalLayout->addLayout(horizontalLayout_2);

        textBrowser = new QTextBrowser(tab_2);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_4->addWidget(TestGroupBox, 1, 0, 1, 1);

        FPS_TestExecutiveClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(FPS_TestExecutiveClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FPS_TestExecutiveClass->addToolBar(Qt::BottomToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FPS_TestExecutiveClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FPS_TestExecutiveClass->setStatusBar(statusBar);

        retranslateUi(FPS_TestExecutiveClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FPS_TestExecutiveClass);
    } // setupUi

    void retranslateUi(QMainWindow *FPS_TestExecutiveClass)
    {
        FPS_TestExecutiveClass->setWindowTitle(QApplication::translate("FPS_TestExecutiveClass", "FPS Test Executive", 0));
        openAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Open...", 0));
        saveAsAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Save As...", 0));
        exitAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Exit", 0));
        SettingOptionGroupBox->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Setting Options", 0));
        LocalSettingsPushButton->setText(QApplication::translate("FPS_TestExecutiveClass", "Local Settings", 0));
        TestGroupBox->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Test", 0));
        QTableWidgetItem *___qtablewidgetitem = TestTableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FPS_TestExecutiveClass", "Status", 0));
        QTableWidgetItem *___qtablewidgetitem1 = TestTableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("FPS_TestExecutiveClass", "Test", 0));
        QTableWidgetItem *___qtablewidgetitem2 = TestTableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("FPS_TestExecutiveClass", "Result", 0));
        QTableWidgetItem *___qtablewidgetitem3 = TestTableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("FPS_TestExecutiveClass", "Bin", 0));
        QTableWidgetItem *___qtablewidgetitem4 = TestTableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("FPS_TestExecutiveClass", "Pass/Total", 0));
        QTableWidgetItem *___qtablewidgetitem5 = TestTableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("FPS_TestExecutiveClass", "Image", 0));
        QTableWidgetItem *___qtablewidgetitem6 = TestTableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("FPS_TestExecutiveClass", "Test Info", 0));
        TestOperationGroupBox->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Test Operation", 0));
        pushButtonRun->setText(QApplication::translate("FPS_TestExecutiveClass", "Run", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FPS_TestExecutiveClass", "Main", 0));
        pushButtonGetVer->setText(QApplication::translate("FPS_TestExecutiveClass", "GetVer", 0));
        pushButtonReadOTP->setText(QApplication::translate("FPS_TestExecutiveClass", "ReadOTP", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FPS_TestExecutiveClass", "OTPDump", 0));
    } // retranslateUi

};

namespace Ui {
    class FPS_TestExecutiveClass: public Ui_FPS_TestExecutiveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPS_TESTEXECUTIVE_H
