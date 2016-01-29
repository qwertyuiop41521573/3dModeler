#include "model.h"
#include "journal.h"
#include "types.h"
#include "trianglecontainer.h"

#include <stdio.h>
#include <iostream>

using namespace std;

namespace Model
{
    bool _textured;

    QString _fileName;
    bool _loaded = false;
    bool _modified = false;

    ElementContainer <Vertex> _vertex;
    //ElementContainer <Triangle> _triangle;
    TriangleContainer _triangle;
}

bool Model::load(const char *newFileName)
{
    Journal::cleanAll();
    _loaded = true;
    FILE *input = fopen(newFileName, "rt");
    if(!input) _loaded = false;
    else
    {
        int i, j, size;
        fscanf(input, "%i", &size);
        _vertex.resize(size);
        double x, y, z;
        for(i = 0; i < _vertex.size(); i++)
        {
            fscanf(input, "%lf %lf %lf ", &x, &y, &z);
            _vertex[i] = QVector3D(x, y, z);
        }
        fscanf(input, "%i", &size);
        int temp;
        for(i = 0; i < size; i++ )
        {
            _triangle.push_back(Triangle());
            for(j = 0; j < 3; j++) {
                fscanf(input, "%i", &temp);
                _triangle.back().setIndex(j, temp);
            }

            _triangle.back().setSelected(false, false);
            _triangle.back().undoRemove();
            _triangle.back().record();
        }
    }
    _modified = false;

    return _loaded;
}

void Model::clear()
{
    Journal::cleanAll();
    _vertex.clear();

    _triangle.clear();
    _loaded = false;
}

bool Model::save()
{
    if(!(_fileName.toStdString().substr(_fileName.length() - 4, _fileName.length()) == ".mdl")) _fileName += ".mdl";
    FILE *output = fopen(_fileName.toStdString().c_str(), "wt");
    if(!output) return false;

    int i, j;
    int vertices = 0;
    for(i = 0; i < _vertex.size(); i++) if(_vertex[i].exists()) vertices++;
    fprintf(output, "%i ", vertices);
    for(i = 0; i < _vertex.size(); i++)
    {
        if(!_vertex[i].exists()) continue;

        const QVector3D &pos = _vertex[i].position();
        fprintf(output, "%lg %lg %lg ", pos.x(), pos.y(), pos.z());
    }

    int triangles = 0;
    for(tr_it it = _triangle.begin(); it != _triangle.end(); it++)
        if(it->exists()) triangles++;
    fprintf(output, "%i ", triangles);

    for(tr_it it = _triangle.begin(); it != _triangle.end(); it++)
        for(j = 0; j < 3; j++) fprintf(output, "%i ", it->getIndex(j));

    fclose(output);
    _loaded = true;
    _modified = false;

    return true;
}

bool Model::empty()
{
    for(int i = 0; i < _vertex.size(); i++) if(_vertex[i].exists()) return false;
    return true;
}

bool Model::textured() { return _textured; }
ElementContainer <Vertex> &Model::vertex() { return _vertex; }
//ElementContainer <Triangle> &Model::triangle() { return _triangle; }
TriangleContainer &Model::triangle() { return _triangle; }
const QString &Model::fileName() { return _fileName; }
void Model::setFileName(const QString &fileName) { _fileName = fileName; }
bool Model::loaded() { return _loaded; }
bool Model::modified() { return _modified; }
void Model::modify() { _modified = true; }
