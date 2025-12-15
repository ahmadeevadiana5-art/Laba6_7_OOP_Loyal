#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Laba6QtWidgetsApplication1.h"

class Laba6QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    Laba6QtWidgetsApplication1(QWidget *parent = nullptr);
    ~Laba6QtWidgetsApplication1();

private:
    Ui::Laba6QtWidgetsApplication1Class ui;
};

