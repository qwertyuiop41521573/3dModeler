#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "selectableobject.h"

class Triangle : public SelectableObject
{
public:
    Triangle(int a, int b, int c)
    { setIndices(a, b, c); };

    Triangle(int *values)
    { for(int i = 0; i < 3; i++) index[i] = values[i]; };

    int getIndex(int num)
    { return index[num]; };

    void setIndex(int num, int newIndex)
    { index[num] = newIndex; };

    void setIndices(int a, int b, int c);

private:
    int index[3];
};

#endif // TRIANGLE_H
