/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionPlay;
    QAction *actionPause;
    QAction *actionStop;
    QAction *actionManageEPG;
    QAction *actionClean_the_library;
    QAction *actionSetup;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuVideo;
    QMenu *menuLibrary;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icons/folder_page_white.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionPlay = new QAction(MainWindow);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/icons/control_play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon1);
        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/icons/control_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon2);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/icons/control_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon3);
        actionManageEPG = new QAction(MainWindow);
        actionManageEPG->setObjectName(QStringLiteral("actionManageEPG"));
        actionClean_the_library = new QAction(MainWindow);
        actionClean_the_library->setObjectName(QStringLiteral("actionClean_the_library"));
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName(QStringLiteral("actionSetup"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuVideo = new QMenu(menuBar);
        menuVideo->setObjectName(QStringLiteral("menuVideo"));
        menuLibrary = new QMenu(menuBar);
        menuLibrary->setObjectName(QStringLiteral("menuLibrary"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuVideo->menuAction());
        menuBar->addAction(menuLibrary->menuAction());
        menuFile->addAction(actionOpen);
        menuVideo->addAction(actionPlay);
        menuVideo->addAction(actionPause);
        menuVideo->addAction(actionStop);
        menuLibrary->addAction(actionSetup);
        menuLibrary->addAction(actionManageEPG);
        menuLibrary->addAction(actionClean_the_library);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPlay);
        mainToolBar->addAction(actionPause);
        mainToolBar->addAction(actionStop);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "Open A File", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionPlay->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        actionPause->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        actionManageEPG->setText(QApplication::translate("MainWindow", "View EPG", Q_NULLPTR));
        actionClean_the_library->setText(QApplication::translate("MainWindow", "Update the library", Q_NULLPTR));
        actionSetup->setText(QApplication::translate("MainWindow", "Connect and setup", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuVideo->setTitle(QApplication::translate("MainWindow", "Video", Q_NULLPTR));
        menuLibrary->setTitle(QApplication::translate("MainWindow", "Library", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
