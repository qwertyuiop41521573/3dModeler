#include "redit.h"
#include "model.h"
#include "trianglecontainer.h"

void REdit::undo()
{
    //replace edited elements with their "before" value
    for(int i = 0; i < _vertex.size(); i++)
        Model::vertex()[_vertex[i].index] = _vertex[i].before;
    for(int i = 0; i < _triangle.size(); i++)
        *_triangle[i].iterator = _triangle[i].before;
    updateNormals();
}

void REdit::redo()
{
    //replace edited elements with their "after" value
    for(int i = 0; i < _vertex.size(); i++)
        Model::vertex()[_vertex[i].index] = _vertex[i].after;
    for(int i = 0; i < _triangle.size(); i++)
        *_triangle[i].iterator = _triangle[i].after;
    updateNormals();
}

void REdit::updateNormals()
{
    vector<Triangle*> tri;
    for(int i = 0; i < _vertex.size(); i++) {
        const Vertex &v = Model::vertex()[_vertex[i].index];
        for(int j = 0; j < v.triangles().size(); j++) {
            Triangle *t = v.triangles()[j];
            int l;

            for(l = 0; l < _triangle.size(); l++)
                if(&*_triangle[l].iterator == t) break;
            if(l < _triangle.size()) continue;

            for(l = 0; l < tri.size(); l++) if(tri[l] == t) break;
            if(l < tri.size()) continue;

            tri.push_back(t);
        }
    }

    vector<int> ver;
    for(int i = 0; i < _triangle.size(); i++) {
        for(int j = 0; j < 3; j++) {
            int index = _triangle[i].iterator->getIndex(j);
            int k;

            for(k = 0; k < _vertex.size(); k++) if(index == _vertex[k].index) break;
            if(k < _vertex.size()) continue;

            for(k = 0; k < ver.size(); k++) if(index == ver[k]) break;
            if(k < ver.size()) continue;

            ver.push_back(index);
        }
    }
    for(int i = 0; i < tri.size(); i++) {
        for(int j = 0; j < 3; j++) {
            int index = tri[i]->getIndex(j);
            int k;

            for(k = 0; k < _vertex.size(); k++) if(index == _vertex[k].index) break;
            if(k < _vertex.size()) continue;

            for(k = 0; k < ver.size(); k++) if(index == ver[k]) break;
            if(k < ver.size()) continue;

            ver.push_back(index);
        }
    }


    for(int i = 0; i < _triangle.size(); i++) _triangle[i].iterator->countNormal();
    for(int i = 0; i < tri.size(); i++) tri[i]->countNormal();

    for(int i = 0; i < _vertex.size(); i++)
        Model::vertex()[_vertex[i].index].countNormals();
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].countNormals();
}

void REdit::setAfter()
{
    for(int i = 0; i < _vertex.size(); i++)
        _vertex[i].after = Model::vertex()[_vertex[i].index];
}
