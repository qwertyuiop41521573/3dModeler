#ifndef CREATINGTOOL_H
#define CREATINGTOOL_H

#include "toolwithwidget.h"
#include "types.h"

class CreatingTool : public ToolWithWidget
{
protected:
    //indices of created vertices and triangles
    vector <int> ver;
    vector<tr_it> tri;

    void removeAll();
    void addTriangle(int v0, int v1, int v2, int smoothingGroup);
    void updateNormals();
};

#endif // CREATINGTOOL_H
