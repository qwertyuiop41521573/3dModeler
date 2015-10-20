#ifndef MODEL_H
#define MODEL_H

#include "elementcontainer.h"

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

    ElementContainer <Vertex> &getVertex()
    { return *vertex; };

    ElementContainer <Triangle> &getTriangle()
    { return *triangle; };

private:
    ElementContainer <Vertex> *vertex;
    ElementContainer <Triangle> *triangle;
};

#endif // MODEL_H
