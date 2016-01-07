#ifndef TROTATE_H
#define TROTATE_H

#include "toolwithpivot.h"

#include "gui/mycheckboxmw.h"

class TRotate : public ToolWithPivot
{
public:
    TRotate();

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MySpinBox *spinBoxAngle;
    MyCheckBoxMW *checkBoxCustomAxis;

    bool createRotationMatrix(QMatrix4x4 *rotation, double angle);
};

#endif // TROTATE_H
