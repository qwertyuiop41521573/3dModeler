#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"
#include "triangle.h"

using namespace std;

typedef enum { CREATE, REMOVE, SELECT } Type;

template <class T> class ElementWithIndex
{
public:
    ElementWithIndex(const T &t, int index)
    {
        _value = t;
        _index = index;
    }

    const T &valRO() const
    { return _value; };

    T &value()
    { return _value; };

    int index() const
    { return _index; };

private:
    T _value;
    int _index;
};

class CreateOrRemove
{
public:
    CreateOrRemove()
    {
        _vertex = new vector <ElementWithIndex <Vertex> >;
        _triangle = new vector <ElementWithIndex <Triangle> >;
    }

    ~CreateOrRemove()
    {
        delete _vertex;
        delete _triangle;
    }

    const vector <ElementWithIndex <Vertex> > &verRO() const
    { return *_vertex; };

    vector <ElementWithIndex <Vertex> > &ver()
    { return *_vertex; };

    const vector <ElementWithIndex <Triangle> > &triRO() const
    { return *_triangle; };

    vector <ElementWithIndex <Triangle> > &tri()
    { return *_triangle; };

private:
    vector <ElementWithIndex <Vertex> > *_vertex;
    vector <ElementWithIndex <Triangle> > *_triangle;
};

struct ValueAndIndex
{
    bool value;
    int index;
};

class Select : public vector <ValueAndIndex>
{
public:
    bool vertices()
    { return _vertices; };

    void setVertices(bool value)
    { _vertices = value; };

private:
    bool _vertices;
};

union Data
{
    CreateOrRemove *createOrRemove;
    Select *select;
};

class Record
{
public:
    Record(Type type);
    void clean();

    const Data &dataRO() const
    { return _data; };

    Data &data()
    { return _data; };

    Type type() const
    { return _type; };

signals:

public slots:

private:
    Type _type;
    Data _data;
};

#endif // RECORD_H
