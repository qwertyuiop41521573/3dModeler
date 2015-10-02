#ifndef TPLANE_H
#define TPLANE_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TPlane : public ToolWithWidget
{
public:
    TPlane(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
    
protected:
    MySpinBox *spinBox[6];
    MyCheckBoxMW *checkBoxSquare;

private:
};

#endif // TPLANE_H
