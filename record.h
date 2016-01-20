#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"
#include "triangle.h"
#include "types.h"

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

    const T &value() const
    { return _value; }

    T &value()
    { return _value; }

    int index() const
    { return _index; }

private:
    T _value;
    int _index;
};

class ElementWithIndex_Triangle
{
public:
    ElementWithIndex_Triangle(tr_it iterator)
    {
        _value = *iterator;
        _iterator = iterator;
    }

    const Triangle &value() const { return _value; }
    Triangle &value() { return _value; }
    //Triangle *pointer() { return _pointer; }
    Triangle &reference() const { return *_iterator; }

private:
    Triangle _value;
    tr_it _iterator;
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
    { _after = after; }

    const T &before() const
    { return _before; }

    const T &after() const
    { return _after; }

    int index() const
    { return _index; }

private:
    T _before, _after;
    int _index;
};

class TwoElementsWithIndex_Triangle
{
public:
    TwoElementsWithIndex_Triangle(tr_it iterator)
    {
        _before = *iterator;
        _iterator = iterator;
    }

    void setAfter() { _after = *_iterator; }
    const Triangle &before() const { return _before; }
    const Triangle &after() const { return _after; }
    tr_it iterator() const { return _iterator; }
    Triangle &reference() const { return *_iterator; }
    //int index() const
    //{ return _index; }

private:
    Triangle _before, _after;
    tr_it _iterator;
};

class Create
{
public:
    Create()
    {
        _vertex = new vector <ElementWithIndex <Vertex> >;
        _triangle = new vector <ElementWithIndex_Triangle>;
    }

    ~Create()
    {
        delete _vertex;
        delete _triangle;
    }

    const vector <ElementWithIndex <Vertex> > &vertex() const
    { return *_vertex; }

    vector <ElementWithIndex <Vertex> > &vertex()
    { return *_vertex; }

    const vector <ElementWithIndex_Triangle> &triangle() const
    { return *_triangle; }

    vector <ElementWithIndex_Triangle> &triangle()
    { return *_triangle; }

private:
    vector <ElementWithIndex <Vertex> > *_vertex;
    vector <ElementWithIndex_Triangle> *_triangle;
};

class Edit
{
public:
    Edit()
    {
        _vertex = new vector <TwoElementsWithIndex <Vertex> >;
        _triangle = new vector <TwoElementsWithIndex_Triangle>;
    }

    ~Edit()
    {
        delete _vertex;
        delete _triangle;
    }

    vector <TwoElementsWithIndex <Vertex> > &vertex()
    { return *_vertex; }

    const vector <TwoElementsWithIndex <Vertex> > &vertex() const
    { return *_vertex; }

    //vector <TwoElementsWithIndex <Triangle> > &triangle()
    vector <TwoElementsWithIndex_Triangle> &triangle()
    { return *_triangle; }

    const vector <TwoElementsWithIndex_Triangle> &triangle() const
    { return *_triangle; }

private:
    vector <TwoElementsWithIndex <Vertex> > *_vertex;
    //vector <TwoElementsWithIndex <Triangle> > *_triangle;
    vector <TwoElementsWithIndex_Triangle> *_triangle;
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

    const Data &data() const
    { return _data; }

    Data &data()
    { return _data; }

    Type type() const
    { return _type; }

signals:

public slots:

private:
    Type _type;
    Data _data;
};

#endif // RECORD_H
