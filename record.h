#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"

using namespace std;

typedef enum { CREATE, CHANGE, DELETE } Type;

struct VertexAndIndex
{
    Vertex vertex;
    int index;
};

class Record
{
public:
    Record(Type type);

    const vector <VertexAndIndex> &getVerticesRO() const
    { return vertices; };

    vector <VertexAndIndex> &getVertices()
    { return vertices; };

    Type type() const
    { return _type; };

signals:

public slots:

private:
    Type _type;
    vector <VertexAndIndex> vertices;
};

#endif // RECORD_H
