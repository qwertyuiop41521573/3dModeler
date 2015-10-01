#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TEllipse : public ToolWithWidget
{
public:
    TEllipse(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MySpinBox *spinBox[8];
    MyCheckBoxMW *checkBox;
};

#endif // TELLIPSE_H
