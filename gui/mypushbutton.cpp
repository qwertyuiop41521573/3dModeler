#include "mypushbutton.h"
#include "mainwindow.h"

MyPushButton::MyPushButton(int index) : QPushButton(0)
{
    _index = index;
    setCheckable(true);
    setMaximumWidth(30);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(handleClick()));
}

void MyPushButton::handleClick() { Workspace::hideViewport(_index); }

