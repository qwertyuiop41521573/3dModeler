#ifndef TOOLWITHWIDGET_H
#define TOOLWITHWIDGET_H

#include "tool.h"
#include "model.h"

class ToolWithWidget : public Tool
{
public:
    ToolWithWidget(MainWindow *mainWindow);

    void setActive(bool value);

    QWidget *getWidget()
    { return _widget; };

    bool elementsExist();

    QPushButton *getFinalButton()
    { return finalButton; };

    void setStage2( bool value )
    { _stage2 = value; };

    bool stage2()
    { return _stage2; };

    WidgetElements *getElements()
    { return elements; }

signals:

public slots:

protected:
    QWidget *_widget;
    QGridLayout *layout;
    WidgetElements *elements;
    QPushButton *finalButton;
    Model *model;
    bool _stage2 = false;

private:



};

#endif // TOOLWITHWIDGET_H
