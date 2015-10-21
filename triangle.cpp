#include "triangle.h"

#include <iostream>

using namespace std;

Triangle::Triangle(int *values)
{
    for(int i = 0; i < 3; i++) index[i] = values[i];
    _isVertex = false;
}

void Triangle::setIndices(int a, int b, int c)
{
    index[0] = a;
    index[1] = b;
    index[2] = c;
}

void Triangle::operator =(const Triangle &triangle)
{
    Element::operator =(triangle);
    for(int i = 0; i < 3; i++) index[i] = triangle.getIndex(i);
}
