#ifndef TRANSFORMINGTOOL_H
#define TRANSFORMINGTOOL_H

#include "toolwithwidget.h"

class TransformingTool : public ToolWithWidget
{
public:
    TransformingTool(MainWindow *mainWindow);

signals:

public slots:

protected:
    vector <bool> checked;
    vector <int> toTransform;
    QMatrix4x4 transformation;

    void transform();
};

#endif // TRANSFORMINGTOOL_H
