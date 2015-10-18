#ifndef RECORD_H
#define RECORD_H

#include "vertex.h"

using namespace std;

typedef enum { CREATE, REMOVE, SELECT } Type;

struct VertexAndIndex
{
    Vertex vertex;
    int index;
};

struct SelValueAndIndex
{
    bool value;
    int index;
};

union Data
{
    vector <VertexAndIndex> *createOrRemove;
    vector <SelValueAndIndex> *select;
};

class Record
{
public:
    Record(Type type);

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
