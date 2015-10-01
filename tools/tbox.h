#ifndef TBOX_H
#define TBOX_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"
#include "gui/mylabel.h"

class TBox : public ToolWithWidget
{
public:
    TBox(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);


signals:

public slots:

private:
    MySpinBox **spinBox;
    MyCheckBoxMW **checkBox;
};

#endif // TBOX_H
