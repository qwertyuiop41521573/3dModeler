#ifndef RECORDWITH1ELEMENT_H
#define RECORDWITH1ELEMENT_H

#include "types.h"
#include "vertex.h"
#include "triangle.h"
#include "record.h"

#include <vector>

using namespace std;

struct VertexWithIndex
{
    VertexWithIndex(int newIndex) { index = newIndex; }
    Vertex value;
    int index;
};

struct TriangleWithIterator
{
    TriangleWithIterator(tr_it newIterator) { iterator = newIterator; }
    Triangle value;
    tr_it iterator;
};

class RecordWith1Element : public Record
{
public:
    const vector <VertexWithIndex> &vertex() const
    { return _vertex; }
    vector <VertexWithIndex> &vertex() { return _vertex; }
    const vector <TriangleWithIterator> &triangle() const
    { return _triangle; }
    vector <TriangleWithIterator> &triangle() { return _triangle; }

protected:
    vector <VertexWithIndex> _vertex;
    vector <TriangleWithIterator> _triangle;
};

#endif // RECORDWITH1ELEMENT_H
