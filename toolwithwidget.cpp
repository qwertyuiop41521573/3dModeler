#include "toolwithwidget.h"
#include "mainwindow.h"
#include "target.h"

using namespace Target;

ToolWithWidget::ToolWithWidget() : Tool()
{
    _widget = new QWidget;
    layout = new QGridLayout;
    _widget->setLayout(layout);
}

void ToolWithWidget::setActive(bool value)
{
    Tool::setActive(value);
    if(_widget)
    {
        if(value) _widget->show();
        else _widget->hide();
    }
}
