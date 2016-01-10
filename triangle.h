#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "element.h"

class Triangle : public Element
{
public:
    Triangle(int a = -1, int b = -1, int c = -1, int smoothingGroup = 0);
    Triangle(int *values, int smoothingGroup);

    int getIndex(int num) const
    { return index[num]; }

    void setIndex(int num, int newIndex)
    { index[num] = newIndex; }

    void operator =(const Triangle &triangle);

    int smoothingGroup() const { return _smoothingGroup; }

private:
    int index[3];
    int _smoothingGroup;
};

#endif // TRIANGLE_H
