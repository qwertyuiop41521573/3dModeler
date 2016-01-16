#include "creatingtool.h"
#include "model.h"

CreatingTool::CreatingTool() : ToolWithWidget()
{

}

void CreatingTool::removeAll()
{
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].remove();
    for(int i = 0; i < tri.size(); i++) Model::triangle()[tri[i]].remove();
}

void CreatingTool::addTriangle(int v0, int v1, int v2, int smoothingGroup)
{
    tri.push_back(Model::triangle().push({ver[v0], ver[v1], ver[v2], smoothingGroup}));
}

void CreatingTool::updateNormals()
{
    for(int i = 0; i < tri.size(); i++) Model::triangle()[tri[i]].countNormal();
}
