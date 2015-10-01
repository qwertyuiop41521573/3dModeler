#ifndef TCYLINDER_H
#define TCYLINDER_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TCylinder : public ToolWithWidget
{
public:
    TCylinder(MainWindow *mainWindow);

       void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
       MySpinBox **spinBox;
       MyCheckBoxMW *checkBox;
};

#endif // TCYLINDER_H
