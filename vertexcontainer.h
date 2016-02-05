#ifndef VERTEXCONTAINER_H
#define VERTEXCONTAINER_H

#include <vector>

#include "vertex.h"
#include "triangle.h"

using namespace std;

class VertexContainer : public vector<Vertex>
{
public:
    int push(const Vertex &vertex);
    void remove(int index);
    void setSelected(int index, bool value);
};

#endif // VERTEXCONTAINER_H
