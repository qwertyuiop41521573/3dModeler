#ifndef TSCALE_H
#define TSCALE_H

#include "toolwithwidget.h"
#include "gui/mypushbuttonmw.h"
#include "gui/myspinbox.h"

class TScale : public ToolWithWidget
{
public:
    TScale(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyPushButtonMW *pushButton[3];
    MySpinBox *spinBox[3];

    QVector3D pivot;
    QVector2D pivotOnScreen;
    vector <bool> checked;

};

#endif // TSCALE_H
