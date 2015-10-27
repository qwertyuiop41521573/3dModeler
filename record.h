#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"
#include "triangle.h"

using namespace std;

//record types
//CREATE - new elements
//EDIT   - changing elements (transform, remove, select / deselect)
typedef enum { CREATE, EDIT } Type;

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

template <class T> class TwoElementsWithIndex
{
public:
    TwoElementsWithIndex(int index, const T &before)
    {
        _index = index;
        _before = before;
    }

    void setAfter(const T &after)
    { _after = after; };

    const T &before() const
    { return _before; };

    const T &after() const
    { return _after; };

    int index() const
    { return _index; };

private:
    T _before, _after;
    int _index;
};


class Create
{
public:
    Create()
    {
        _vertex = new vector <ElementWithIndex <Vertex> >;
        _triangle = new vector <ElementWithIndex <Triangle> >;
    }

    ~Create()
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

class Edit
{
public:
    Edit()
    {
        _vertex = new vector <TwoElementsWithIndex <Vertex> >;
        _triangle = new vector <TwoElementsWithIndex <Triangle> >;
    }

    ~Edit()
    {
        delete _vertex;
        delete _triangle;
    }

    vector <TwoElementsWithIndex <Vertex> > &vertex()
    { return *_vertex; };

    const vector <TwoElementsWithIndex <Vertex> > &verRO() const
    { return *_vertex; };

    vector <TwoElementsWithIndex <Triangle> > &triangle()
    { return *_triangle; };

    const vector <TwoElementsWithIndex <Triangle> > &triRO() const
    { return *_triangle; };

private:
    vector <TwoElementsWithIndex <Vertex> > *_vertex;
    vector <TwoElementsWithIndex <Triangle> > *_triangle;
};

union Data
{
    Create *create;
    Edit *edit;
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
