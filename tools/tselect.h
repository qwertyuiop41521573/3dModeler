#ifndef TSELECT_H
#define TSELECT_H

#include "toolwithwidget.h"
#include "gui/mycheckbox.h"

class TSelect : public ToolWithWidget
{
public:
    TSelect(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyCheckBox *checkBox[2];
};

#endif // TSELECT_H
