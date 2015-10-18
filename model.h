#ifndef MODEL_H
#define MODEL_H

#include "triangle.h"
#include "vertexcontainer.h"


#include <iostream>

using namespace std;

class Model
{
public:
    Model(Journal *journal);
    bool load(const char *fileName);
    void clear();
    void save();

    bool textured;
    string texture;

    bool isEmpty = true;
    bool isModified = false;
    QString fileName;
    bool loaded = false;

    VertexContainer &getVertex()
    { return *vertex; };

    vector <Triangle> &getTriangle()
    { return triangle; };

private:
    VertexContainer *vertex;
    vector <Triangle> triangle;
};

#endif // MODEL_H
