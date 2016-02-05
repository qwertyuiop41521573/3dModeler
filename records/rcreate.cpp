#include "rcreate.h"
#include "model.h"
#include "trianglecontainer.h"
#include "triangle.h"

bool RCreate::submit()
{
    for(int i = 0; i < _vertex.size(); i++)
        _vertex[i].value = Model::vertex()[_vertex[i].index];
    for(int i = 0; i < _triangle.size(); i++)
        _triangle[i].value = *_triangle[i].iterator;

    return true;
}

void RCreate::undo()
{
    for(int i = 0; i < _vertex.size(); i++)
        Model::vertex()[_vertex[i].index].remove();
    for(int i = 0; i < _triangle.size(); i++)
        _triangle[i].iterator->remove();
    updateNormals();
}

void RCreate::redo()
{
    for(int i = 0; i < _vertex.size(); i++)
        Model::vertex()[_vertex[i].index] = _vertex[i].value;
    for(int i = 0; i < _triangle.size(); i++) {
        *_triangle[i].iterator = _triangle[i].value;
        Model::triangle().record(_triangle[i].iterator);
    }
    updateNormals();
}
