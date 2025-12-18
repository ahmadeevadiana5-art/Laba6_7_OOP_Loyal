/********************************************************************************
** Form generated from reading UI file 'Laba6QtWidgetsApplication1.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABA6QTWIDGETSAPPLICATION1_H
#define UI_LABA6QTWIDGETSAPPLICATION1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Laba6QtWidgetsApplication1Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Laba6QtWidgetsApplication1Class)
    {
        if (Laba6QtWidgetsApplication1Class->objectName().isEmpty())
            Laba6QtWidgetsApplication1Class->setObjectName("Laba6QtWidgetsApplication1Class");
        Laba6QtWidgetsApplication1Class->resize(600, 400);
        menuBar = new QMenuBar(Laba6QtWidgetsApplication1Class);
        menuBar->setObjectName("menuBar");
        Laba6QtWidgetsApplication1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Laba6QtWidgetsApplication1Class);
        mainToolBar->setObjectName("mainToolBar");
        Laba6QtWidgetsApplication1Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(Laba6QtWidgetsApplication1Class);
        centralWidget->setObjectName("centralWidget");
        Laba6QtWidgetsApplication1Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Laba6QtWidgetsApplication1Class);
        statusBar->setObjectName("statusBar");
        Laba6QtWidgetsApplication1Class->setStatusBar(statusBar);

        retranslateUi(Laba6QtWidgetsApplication1Class);

        QMetaObject::connectSlotsByName(Laba6QtWidgetsApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *Laba6QtWidgetsApplication1Class)
    {
        Laba6QtWidgetsApplication1Class->setWindowTitle(QCoreApplication::translate("Laba6QtWidgetsApplication1Class", "Laba6QtWidgetsApplication1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Laba6QtWidgetsApplication1Class: public Ui_Laba6QtWidgetsApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABA6QTWIDGETSAPPLICATION1_H
