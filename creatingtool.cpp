#include "creatingtool.h"
#include "model.h"
#include "trianglecontainer.h"

void CreatingTool::removeAll()
{
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].remove();
    for(int i = 0; i < tri.size(); i++) tri[i]->remove();
}

void CreatingTool::addTriangle(int v0, int v1, int v2, int smoothingGroup)
{
    tri.push_back(Model::triangle().push({ver[v0], ver[v1], ver[v2], smoothingGroup}));
}

void CreatingTool::updateNormals()
{
    vector <int> ver2;
    for(int i = 0; i < tri.size(); i++) {
        tri[i]->countNormal();
        for(int j = 0; j < 3; j++) {
            int index = tri[i]->getIndex(j);
            int k;

            for(k = 0; k < ver.size(); k++) if(index == ver[k]) break;
            if(k < ver.size()) continue;

            for(k = 0; k < ver2.size(); k++) if(index == ver2[k]) break;
            if(k < ver2.size()) continue;

            ver2.push_back(index);
        }
    }
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].countNormals();
    for(int i = 0; i < ver2.size(); i++) Model::vertex()[ver2[i]].countNormals();
}
