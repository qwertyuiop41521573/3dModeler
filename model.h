#ifndef MODEL_H
#define MODEL_H

#include "triangle.h"
#include "vertex.h"

using namespace std;

class Model
{
public:
    Model();
    bool load( const char *fileName );
    void clear();
    void save();

    bool textured;
    string texture;

    bool isEmpty = true;
    bool isModified = false;
    QString fileName;
    bool loaded = false;

    vector <Vertex> &getVertex()
    { return vertex; };

    vector <Triangle> &getTriangle()
    { return triangle; };

private:
    vector <Vertex> vertex;
    vector <Triangle> triangle;
};

#endif // MODEL_H
