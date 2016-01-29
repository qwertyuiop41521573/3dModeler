#ifndef MODEL_H
#define MODEL_H

#include "elementcontainer.h"

#include <iostream>

using namespace std;

class TriangleContainer;

namespace Model
{
    bool load(const char *fileName);
    void clear();
    bool save();

    bool textured();
    bool empty();
    ElementContainer <Vertex> &vertex();
    //ElementContainer <Triangle> &triangle();
    TriangleContainer &triangle();
    const QString &fileName();
    void setFileName(const QString &fileName);
    bool loaded();
    bool modified();
    void modify();
}

#endif // MODEL_H
