#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"
#include "triangle.h"

using namespace std;

typedef enum { CREATE, REMOVE, SELECT, EDIT } Type;

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

class Edit
{
public:
    Edit()
    {
        _transformation = new QMatrix4x4;
        _transformation->setToIdentity();
    }

    ~Edit()
    {
        delete _list;
        delete _transformation;
    }

    void setList(const vector <int> &list)
    {
        _list = new vector <int>(list.size());
        for(int i = 0;i < list.size(); i++) _list->at(i) = list[i];
    }

    void transform(const QMatrix4x4 &matrix)
    {
        *_transformation = matrix * *_transformation;
    }

    const vector <int> &list() const
    { return *_list; };

    const QMatrix4x4 &transformation() const
    { return *_transformation; };

private:
    vector <int> *_list;
    QMatrix4x4 *_transformation;
};

union Data
{
    CreateOrRemove *createOrRemove;
    Select *select;
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
