#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "element.h"

class Triangle : public Element
{
public:
    Triangle(int a = -1, int b = -1, int c = -1)
    { setIndices(a, b, c); };

    Triangle(int *values);

    int getIndex(int num) const
    { return index[num]; };

    void setIndex(int num, int newIndex)
    { index[num] = newIndex; };

    void setIndices(int a, int b, int c);
    void operator =(const Triangle &triangle);

private:
    int index[3];
};

#endif // TRIANGLE_H
