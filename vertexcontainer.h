#ifndef VERTEXCONTAINER_H
#define VERTEXCONTAINER_H

#include "vertex.h"

using namespace std;

class Journal;

class VertexContainer : public vector <Vertex>
{
public:
    VertexContainer(Journal *journal);
    int push(Vertex vertex);
    void remove(int index);

private:
    Journal *_journal;
};

#endif // VERTEXCONTAINER_H
