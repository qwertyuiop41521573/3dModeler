#include "mypushbuttonmw.h"

MyPushButtonMW::MyPushButtonMW(QString text) : QPushButton(text)
{
    setCheckable(true);
    setMaximumWidth(70);
}

