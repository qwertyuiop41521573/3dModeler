#include "triangle.h"
#include "model.h"

#include <iostream>

using namespace std;

Triangle::Triangle(int a, int b, int c, int smoothingGroup)
{
    index[0] = a;
    index[1] = b;
    index[2] = c;

    init(smoothingGroup);
}

Triangle::Triangle(int *values, int smoothingGroup)
{
    for(int i = 0; i < 3; i++) index[i] = values[i];
    init(smoothingGroup);
}

void Triangle::init(int smoothingGroup)
{
    _isVertex = false;
    _smoothingGroup = smoothingGroup;
    //countNormal();
}

void Triangle::countNormal()
{
    QVector3D v[3];
    for(int j = 0; j < 3; j++) v[j] = Model::vertex()[getIndex(j)].position();
    _normal = QVector3D::crossProduct(v[1] - v[0], v[2] - v[0]).normalized();
}

void Triangle::operator =(const Triangle &triangle)
{
    Element::operator =(triangle);
    for(int i = 0; i < 3; i++) index[i] = triangle.getIndex(i);
    _smoothingGroup = triangle.smoothingGroup();
    //_normal = triangle.normal();
}
