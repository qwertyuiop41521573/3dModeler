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
    void addTriangle(int v0, int v1, int v2, int smoothingGroup);
    void updateNormals();
};

#endif // CREATINGTOOL_H
