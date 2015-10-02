#ifndef TROTATE_H
#define TROTATE_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TRotate : public ToolWithWidget
{
public:
    TRotate(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MySpinBox *spinBox[4];
    MyCheckBoxMW *checkBox;

};

#endif // TROTATE_H
