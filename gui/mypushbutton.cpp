#include "mypushbutton.h"
#include "mainwindow.h"

MyPushButton::MyPushButton(int index, MainWindow *mainWindow,
                           QWidget *parent) :
    QPushButton(parent)
{
    _index = index;
    _mainWindow = mainWindow;
    setCheckable(true);
    setMaximumWidth(30);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(handleClick()));
}

void MyPushButton::handleClick()
{
    _mainWindow->hideViewport(_index);
};

