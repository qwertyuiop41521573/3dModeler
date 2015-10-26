#ifndef MODEL_H
#define MODEL_H

#include "elementcontainer.h"

#include <iostream>

using namespace std;

class Model
{
public:
    Model(Journal *journal);
    bool load(const char *fileName);
    void clear();
    bool save();

    bool textured()
    { return _textured; };

    bool empty();

    ElementContainer <Vertex> &vertex()
    { return *_vertex; };

    ElementContainer <Triangle> &triangle()
    { return *_triangle; };

    const QString &fileName()
    { return _fileName; };

    void setFileName(const QString &fileName)
    { _fileName = fileName; };

    bool loaded()
    { return _loaded; };

private:
    bool _textured;

    QString _fileName;
    bool _loaded = false;

private:
    Journal *_journal;
    ElementContainer <Vertex> *_vertex;
    ElementContainer <Triangle> *_triangle;
};

#endif // MODEL_H
