#ifndef REDIT_H
#define REDIT_H

#include "types.h"
#include "vertex.h"
#include "triangle.h"
#include "record.h"

#include <vector>

using namespace std;

struct TwoVerticesWithIndex
{
    TwoVerticesWithIndex(int newIndex, const Vertex &newBefore)
    {
        index = newIndex;
        before = newBefore;
    }
    Vertex before, after;
    int index;
};

struct TwoTrianglesWithIterator
{
    TwoTrianglesWithIterator(tr_it newIterator)
    {
        before = *newIterator;
        iterator = newIterator;
    }
    Triangle before, after;
    tr_it iterator;
};

class REdit : public Record
{
public:
    void undo();
    void redo();

    vector <TwoVerticesWithIndex> &vertex() { return _vertex; }
    const vector <TwoVerticesWithIndex> &vertex() const
    { return _vertex; }
    vector <TwoTrianglesWithIterator> &triangle() { return _triangle; }
    const vector <TwoTrianglesWithIterator> &triangle() const
    { return _triangle; }

    void setAfter();

private:
    vector <TwoVerticesWithIndex> _vertex;
    vector <TwoTrianglesWithIterator> _triangle;

    void updateNormals();
};

#endif // REDIT_H
