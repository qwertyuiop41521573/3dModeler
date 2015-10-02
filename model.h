#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QTextBrowser>

#include "vertex.h"
#include "triangle.h"

#include <iostream>

using namespace std;

class Model
{
public:
    Model();
    bool load( const char *fileName );
    void clear();
    void save();

   // int vertexNumber = 0, triangleNumber = 0;


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
