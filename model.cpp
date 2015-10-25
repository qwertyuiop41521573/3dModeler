#include "model.h"

#include <stdio.h>
#include <iostream>

using namespace std;

Model::Model(Journal *journal)
{
    _vertex = new ElementContainer <Vertex>(journal);
    _triangle = new ElementContainer <Triangle>(journal);
}

bool Model::load(const char *newFileName)
{
    _loaded = true;
    FILE *input = fopen(newFileName, "rt");
    if(!input) _loaded = false;
    else
    {
        int i, j, size;
        fscanf(input, "%i", &size);
        _vertex->resize(size);
        double x, y, z;
        for(i = 0; i < _vertex->size(); i++)
        {
            fscanf(input, "%lf %lf %lf ", &x, &y, &z);
            _vertex->at(i) = QVector3D(x, y, z);
        }
        fscanf(input, "%i", &size);
        _triangle->resize(size);
        int temp;
        for(i = 0; i < _triangle->size(); i++ ) for(j = 0; j < 3; j++)
        {
            fscanf(input, "%i", &temp);
            _triangle->at(i).setIndex(j, temp);
        }
    }

    return _loaded;
}

void Model::clear()
{
    _vertex->clear();
    _triangle->clear();
    _loaded = false;
}

bool Model::save()
{
    if(!(_fileName.toStdString().substr(_fileName.length() - 4, _fileName.length()) == ".mdl")) _fileName += ".mdl";
    FILE *output = fopen(_fileName.toStdString().c_str(), "wt");
    if(!output) return false;

    int i, j;
    int vertices = 0;
    for(i = 0; i < _vertex->size(); i++) if(_vertex->at(i).exists()) vertices++;
    fprintf(output, "%i ", vertices);
    for(i = 0; i < _vertex->size(); i++)
    {
        if(!_vertex->at(i).exists()) continue;

        const QVector3D &pos = _vertex->at(i).getPosition();
        fprintf(output, "%lg %lg %lg ", pos.x(), pos.y(), pos.z());
    }

    int triangles = 0;
    for(i = 0; i < _triangle->size(); i++) if(_triangle->at(i).exists()) triangles++;
    fprintf(output, "%i ", triangles);
    for(i = 0; i < _triangle->size(); i++ )
    {
        if(!_triangle->at(i).exists()) continue;

        for(j = 0; j < 3; j++) fprintf(output, "%i ", _triangle->at(i).getIndex(j));
    }

    fclose(output);
    _loaded = true;
}

bool Model::empty()
{
    for(int i = 0; i < _vertex->size(); i++) if(_vertex->at(i).exists()) return false;
    return true;
}

