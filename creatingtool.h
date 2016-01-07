#ifndef CREATINGTOOL_H
#define CREATINGTOOL_H

#include "toolwithwidget.h"

class CreatingTool : public ToolWithWidget
{
public:
    CreatingTool();

signals:

public slots:

protected:
    //indices of created vertices and triangles
    vector <int> ver;
    vector <int> tri;

    void removeAll();
};

#endif // CREATINGTOOL_H
