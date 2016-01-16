#ifndef TRANSFORMINGTOOL_H
#define TRANSFORMINGTOOL_H

#include "toolwithwidget.h"

#include "gui/myspinbox.h"

//transforms with multiplying vertex coordinates by transformation matrix (matrix * vertex)
class TransformingTool : public ToolWithWidget
{
public:
    TransformingTool();

signals:

public slots:

protected:
    MySpinBox *spinBox[3];

    vector <bool> checked;
    //list of selected vertices
    vector <int> toTransform;
    vector <int> tri;
    QMatrix4x4 transformation;

    void function(Action action, QMouseEvent *event);
    void updateNormals();
};

#endif // TRANSFORMINGTOOL_H
