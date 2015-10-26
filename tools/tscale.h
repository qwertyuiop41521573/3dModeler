#ifndef TSCALE_H
#define TSCALE_H

#include "toolwithpivot.h"

#include "gui/mypushbuttonmw.h"

class TScale : public ToolWithPivot
{
public:
    TScale(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyPushButtonMW *pushButton[3];

};

#endif // TSCALE_H
