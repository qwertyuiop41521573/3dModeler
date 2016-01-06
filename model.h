#ifndef MODEL_H
#define MODEL_H

#include "elementcontainer.h"

#include <iostream>

using namespace std;

namespace Model
{
    void init(Journal *journal);
    bool load(const char *fileName);
    void clear();
    bool save();

    bool textured();
    bool empty();
    ElementContainer <Vertex> &vertex();
    ElementContainer <Triangle> &triangle();
    const QString &fileName();
    void setFileName(const QString &fileName);
    bool loaded();
    bool modified();
    void modify();
}

#endif // MODEL_H
