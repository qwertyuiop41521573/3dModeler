#ifndef MODEL_H
#define MODEL_H

#include "triangle.h"
#include "vertex.h"


#include <iostream>

using namespace std;

class VertexContainer : public vector <Vertex>
{
public:
    int push(Vertex vertex)
    {
        for(int i = 0; i < size(); i++)
        {
            if(!at(i).exists())
            {
                at(i) = vertex;
                return i;
            }
        }
        push_back(vertex);
        return size() - 1;
    }

    void removeArray(const vector <int> &ind)
    {

    }
};

class Model
{
public:
    Model();
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
    { return vertex; };

    vector <Triangle> &getTriangle()
    { return triangle; };

private:
    VertexContainer vertex;
    vector <Triangle> triangle;
};

#endif // MODEL_H
