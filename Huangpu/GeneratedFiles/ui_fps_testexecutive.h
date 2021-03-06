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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QAction *actionExit;
    QAction *actionLocalSetttings;
    QAction *actionBinCodes;
    QAction *actionAbout_FPS_Test_Executive;
    QAction *actionEngineer_Mode;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QLabel *TestInfoLabel;
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
    QPushButton *pushButtonInvalidate;
    QTextBrowser *textBrowser;
    QLabel *OTPResultLabel;
    QToolBar *mainToolBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOption;
    QMenu *menuAbout;

    void setupUi(QMainWindow *FPS_TestExecutiveClass)
    {
        if (FPS_TestExecutiveClass->objectName().isEmpty())
            FPS_TestExecutiveClass->setObjectName(QStringLiteral("FPS_TestExecutiveClass"));
        FPS_TestExecutiveClass->resize(600, 700);
        openAct = new QAction(FPS_TestExecutiveClass);
        openAct->setObjectName(QStringLiteral("openAct"));
        saveAsAct = new QAction(FPS_TestExecutiveClass);
        saveAsAct->setObjectName(QStringLiteral("saveAsAct"));
        exitAct = new QAction(FPS_TestExecutiveClass);
        exitAct->setObjectName(QStringLiteral("exitAct"));
        actionExit = new QAction(FPS_TestExecutiveClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionLocalSetttings = new QAction(FPS_TestExecutiveClass);
        actionLocalSetttings->setObjectName(QStringLiteral("actionLocalSetttings"));
        actionBinCodes = new QAction(FPS_TestExecutiveClass);
        actionBinCodes->setObjectName(QStringLiteral("actionBinCodes"));
        actionAbout_FPS_Test_Executive = new QAction(FPS_TestExecutiveClass);
        actionAbout_FPS_Test_Executive->setObjectName(QStringLiteral("actionAbout_FPS_Test_Executive"));
        actionEngineer_Mode = new QAction(FPS_TestExecutiveClass);
        actionEngineer_Mode->setObjectName(QStringLiteral("actionEngineer_Mode"));
        centralWidget = new QWidget(FPS_TestExecutiveClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        TestInfoLabel = new QLabel(tab);
        TestInfoLabel->setObjectName(QStringLiteral("TestInfoLabel"));
        QFont font;
        font.setPointSize(8);
        TestInfoLabel->setFont(font);

        verticalLayout_2->addWidget(TestInfoLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        TestTableWidget = new QTableWidget(tab);
        if (TestTableWidget->rowCount() < 9)
            TestTableWidget->setRowCount(9);
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
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        TestTableWidget->setVerticalHeaderItem(8, __qtablewidgetitem8);
        TestTableWidget->setObjectName(QStringLiteral("TestTableWidget"));
        TestTableWidget->setAutoScroll(false);
        TestTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        TestTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        TestTableWidget->setTextElideMode(Qt::ElideMiddle);

        horizontalLayout->addWidget(TestTableWidget);


        verticalLayout_2->addLayout(horizontalLayout);

        TestOperationGroupBox = new QGroupBox(tab);
        TestOperationGroupBox->setObjectName(QStringLiteral("TestOperationGroupBox"));
        gridLayout_3 = new QGridLayout(TestOperationGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        pushButtonRun = new QPushButton(TestOperationGroupBox);
        pushButtonRun->setObjectName(QStringLiteral("pushButtonRun"));

        gridLayout_3->addWidget(pushButtonRun, 0, 0, 1, 1);


        verticalLayout_2->addWidget(TestOperationGroupBox);

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

        pushButtonInvalidate = new QPushButton(tab_2);
        pushButtonInvalidate->setObjectName(QStringLiteral("pushButtonInvalidate"));

        horizontalLayout_2->addWidget(pushButtonInvalidate);


        verticalLayout->addLayout(horizontalLayout_2);

        textBrowser = new QTextBrowser(tab_2);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        OTPResultLabel = new QLabel(tab_2);
        OTPResultLabel->setObjectName(QStringLiteral("OTPResultLabel"));

        verticalLayout->addWidget(OTPResultLabel);

        tabWidget->addTab(tab_2, QString());

        horizontalLayout_3->addWidget(tabWidget);

        FPS_TestExecutiveClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(FPS_TestExecutiveClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FPS_TestExecutiveClass->addToolBar(Qt::BottomToolBarArea, mainToolBar);
        menuBar = new QMenuBar(FPS_TestExecutiveClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOption = new QMenu(menuBar);
        menuOption->setObjectName(QStringLiteral("menuOption"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        FPS_TestExecutiveClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOption->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionExit);
        menuOption->addAction(actionLocalSetttings);
        menuOption->addAction(actionBinCodes);
        menuOption->addAction(actionEngineer_Mode);
        menuAbout->addAction(actionAbout_FPS_Test_Executive);

        retranslateUi(FPS_TestExecutiveClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FPS_TestExecutiveClass);
    } // setupUi

    void retranslateUi(QMainWindow *FPS_TestExecutiveClass)
    {
        FPS_TestExecutiveClass->setWindowTitle(QApplication::translate("FPS_TestExecutiveClass", "Huangpu", 0));
        openAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Open...", 0));
        saveAsAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Save As...", 0));
        exitAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Exit", 0));
        actionExit->setText(QApplication::translate("FPS_TestExecutiveClass", "Exit", 0));
        actionLocalSetttings->setText(QApplication::translate("FPS_TestExecutiveClass", "LocalSetttings", 0));
        actionBinCodes->setText(QApplication::translate("FPS_TestExecutiveClass", "BinCodes", 0));
        actionAbout_FPS_Test_Executive->setText(QApplication::translate("FPS_TestExecutiveClass", "Huangpu", 0));
        actionEngineer_Mode->setText(QApplication::translate("FPS_TestExecutiveClass", "Engineer Mode", 0));
        TestInfoLabel->setText(QApplication::translate("FPS_TestExecutiveClass", "Info", 0));
        QTableWidgetItem *___qtablewidgetitem = TestTableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FPS_TestExecutiveClass", "Test Site", 0));
        QTableWidgetItem *___qtablewidgetitem1 = TestTableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("FPS_TestExecutiveClass", "SerialNumber", 0));
        QTableWidgetItem *___qtablewidgetitem2 = TestTableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("FPS_TestExecutiveClass", "Status", 0));
        QTableWidgetItem *___qtablewidgetitem3 = TestTableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("FPS_TestExecutiveClass", "Result BinCode", 0));
        QTableWidgetItem *___qtablewidgetitem4 = TestTableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("FPS_TestExecutiveClass", "SNR", 0));
        QTableWidgetItem *___qtablewidgetitem5 = TestTableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("FPS_TestExecutiveClass", "Pass/Fail", 0));
        QTableWidgetItem *___qtablewidgetitem6 = TestTableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("FPS_TestExecutiveClass", "Image(NoFinger)", 0));
        QTableWidgetItem *___qtablewidgetitem7 = TestTableWidget->verticalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("FPS_TestExecutiveClass", "Image(FakeFinger)", 0));
        QTableWidgetItem *___qtablewidgetitem8 = TestTableWidget->verticalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("FPS_TestExecutiveClass", "Test Step", 0));
        TestOperationGroupBox->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Test Operation", 0));
        pushButtonRun->setText(QApplication::translate("FPS_TestExecutiveClass", "Run", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FPS_TestExecutiveClass", "Main", 0));
        pushButtonGetVer->setText(QApplication::translate("FPS_TestExecutiveClass", "GetVer", 0));
        pushButtonReadOTP->setText(QApplication::translate("FPS_TestExecutiveClass", "ReadOTP", 0));
        pushButtonInvalidate->setText(QApplication::translate("FPS_TestExecutiveClass", "Invalidate", 0));
        OTPResultLabel->setText(QApplication::translate("FPS_TestExecutiveClass", "NULL", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FPS_TestExecutiveClass", "OTPDump", 0));
        menuFile->setTitle(QApplication::translate("FPS_TestExecutiveClass", "File", 0));
        menuOption->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Option", 0));
        menuAbout->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class FPS_TestExecutiveClass: public Ui_FPS_TestExecutiveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPS_TESTEXECUTIVE_H
