#include "toolwithwidget.h"
#include "mainwindow.h"

ToolWithWidget::ToolWithWidget(MainWindow *mainWindow) : Tool(mainWindow)
{
    _widget = new QWidget;
    layout = new QGridLayout;
    _widget->setLayout(layout);
    model = _mainWindow->getModel();
}

void ToolWithWidget::setActive(bool value)
{
    Tool::setActive(value);
    if( _widget )
    {
        if( value ) _widget->show();
        else _widget->hide();
    }
}

bool ToolWithWidget::elementsExist()
{
    return !(elements == 0);
}
