#include "toolwithwidget.h"
#include "mainwindow.h"

ToolWithWidget::ToolWithWidget(MainWindow *mainWindow, QWidget *widget, WidgetElements *newElements, QString
    finalButtonText, bool hasStage2) : Tool(mainWindow)
{
    _widget = widget;
    elements = newElements;
    if(!finalButtonText.isNull()) finalButton = new QPushButton(
                finalButtonText);
     _hasStage2 = hasStage2;


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
