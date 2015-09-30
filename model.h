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

    int vertexNumber = 0, triangleNumber = 0;
    vector <Vertex> vertex;
    vector <Triangle> triangle;
    bool textured;
    string texture;

    bool isEmpty = true;
    bool isModified = false;
    QString fileName;
    bool loaded = false;

private:
};

#endif // MODEL_H
