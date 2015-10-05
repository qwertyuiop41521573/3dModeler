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
    MyCheckBoxMW *checkBoxCustomAxis;

    QVector3D pivot;
    QVector2D pivotOnScreen;
     vector <bool> checked;

    bool getAxis(QMatrix4x4 *rotation, double angle);
};

#endif // TROTATE_H
