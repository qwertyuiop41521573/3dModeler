#include "myframe.h"

MyFrame::MyFrame(bool vertical)
{
    setFrameShape(vertical ? QFrame::VLine : QFrame::HLine);
    setFrameShadow(QFrame::Sunken);
    if(vertical) setMaximumHeight(200);
    else setMaximumWidth(150);
}

