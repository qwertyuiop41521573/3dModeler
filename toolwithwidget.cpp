#include "toolwithwidget.h"
#include "mainwindow.h"

ToolWithWidget::ToolWithWidget(MainWindow *mainWindow) : Tool(mainWindow)
{
    _widget = new QWidget;
    layout = new QGridLayout;
    _widget->setLayout(layout);
    journal = _mainWindow->getJournal();
    workWithElements = _mainWindow->getWorkWithElements();
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
