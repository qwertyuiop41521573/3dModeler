#include "triangle.h"

#include <iostream>

using namespace std;

Triangle::Triangle(int a, int b, int c, int smoothingGroup)
{
    _isVertex = false;
    index[0] = a;
    index[1] = b;
    index[2] = c;

    _smoothingGroup = smoothingGroup;
}

Triangle::Triangle(int *values, int smoothingGroup)
{
    _isVertex = false;
    for(int i = 0; i < 3; i++) index[i] = values[i];
    _smoothingGroup = smoothingGroup;
}

void Triangle::operator =(const Triangle &triangle)
{
    Element::operator =(triangle);
    for(int i = 0; i < 3; i++) index[i] = triangle.getIndex(i);
    _smoothingGroup = triangle.smoothingGroup();
}
