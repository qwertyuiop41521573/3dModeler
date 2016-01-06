#include "model.h"
#include "journal.h"

#include <stdio.h>
#include <iostream>

using namespace std;

namespace Model
{
    bool _textured;

    QString _fileName;
    bool _loaded = false;
    bool _modified = false;

    Journal *_journal;
    ElementContainer <Vertex> *_vertex;
    ElementContainer <Triangle> *_triangle;
}

void Model::init(Journal *journal)
{
    _journal = journal;
    _vertex = new ElementContainer <Vertex>(journal);
    _triangle = new ElementContainer <Triangle>(journal);
}

bool Model::load(const char *newFileName)
{
    _journal->cleanAll();
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
        for(i = 0; i < _triangle->size(); i++ )
        {
            for(j = 0; j < 3; j++)
            {
                fscanf(input, "%i", &temp);
                _triangle->at(i).setIndex(j, temp);
            }
            _triangle->at(i).setSelected(false, false);
            _triangle->at(i).undoRemove();
        }
    }
    _modified = false;

    return _loaded;
}

void Model::clear()
{
    _journal->cleanAll();
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

        const QVector3D &pos = _vertex->at(i).positionRO();
        fprintf(output, "%lg %lg %lg ", pos.x(), pos.y(), pos.z());
    }

    int triangles = 0;
    for(i = 0; i < _triangle->size(); i++) if(_triangle->at(i).exists()) triangles++;
    fprintf(output, "%i ", triangles);
    for(i = 0; i < _triangle->size(); i++)
    {
        if(!_triangle->at(i).exists()) continue;

        for(j = 0; j < 3; j++) fprintf(output, "%i ", _triangle->at(i).getIndex(j));
    }

    fclose(output);
    _loaded = true;
    _modified = false;

    return true;
}

bool Model::empty()
{
    for(int i = 0; i < _vertex->size(); i++) if(_vertex->at(i).exists()) return false;
    return true;
}

bool Model::textured() { return _textured; }
ElementContainer <Vertex> &Model::vertex() { return *_vertex; }
ElementContainer <Triangle> &Model::triangle() { return *_triangle; }
const QString &Model::fileName() { return _fileName; }
void Model::setFileName(const QString &fileName) { _fileName = fileName; }
bool Model::loaded() { return _loaded; }
bool Model::modified() { return _modified; }
void Model::modify() { _modified = true; }
