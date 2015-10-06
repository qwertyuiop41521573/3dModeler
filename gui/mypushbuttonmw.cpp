#include "mypushbuttonmw.h"

MyPushButtonMW::MyPushButtonMW(const QString &text) : QPushButton(text)
{
    setCheckable(true);
    setMaximumWidth(70);
}

