#ifndef TOOLWITHPIVOT_H
#define TOOLWITHPIVOT_H

#include "transformingtool.h"

class ToolWithPivot : public TransformingTool
{
public:
    ToolWithPivot(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

protected:
    QVector3D pivot;
    QVector2D pivotOnScreen;
};

#endif // TOOLWITHPIVOT_H
