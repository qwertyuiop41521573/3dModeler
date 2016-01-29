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
    vector<tr_it> tri;
    for(int i = 0; i < _triangle.size(); i++)
        tri.push_back(_triangle[i].iterator);
    for(int i = 0; i < _vertex.size(); i++) {
        for(tr_it it = Model::triangle().begin(); it != Model::triangle().end(); it++) {
            for(int k = 0; k < 3; k++) {
                if(it->getIndex(k) == _vertex[i].index) {
                    int l;
                    for(l = 0; l < tri.size(); l++) if(tri[l] == it)
                        break;
                    if(l == tri.size()) tri.push_back(it);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < tri.size(); i++) tri[i]->countNormal();
}

void REdit::setAfter()
{
    for(int i = 0; i < _vertex.size(); i++)
        _vertex[i].after = Model::vertex()[_vertex[i].index];
}
