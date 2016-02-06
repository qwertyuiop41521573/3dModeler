#include "triangle.h"
#include "model.h"

#include <iostream>

using namespace std;

Triangle::Triangle(int a, int b, int c, int smoothingGroup)
{
    _index[0] = a;
    _index[1] = b;
    _index[2] = c;

    init(smoothingGroup);
}

Triangle::Triangle(int *values, int smoothingGroup)
{
    for(int i = 0; i < 3; i++) _index[i] = values[i];
    init(smoothingGroup);
}

Triangle::Triangle(const Triangle &triangle)
{
    for(int i = 0; i < 3; i++) _index[i] = triangle.index(i);
    init(triangle.smoothingGroup());
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
    for(int j = 0; j < 3; j++) v[j] = Model::vertex()[_index[j]].position();
    _normal = QVector3D::crossProduct(v[1] - v[0], v[2] - v[0]).normalized();
}

void Triangle::operator =(const Triangle &triangle)
{
    Element::operator =(triangle);
    for(int i = 0; i < 3; i++) _index[i] = triangle.index(i);
    _smoothingGroup = triangle.smoothingGroup();
    //for(int i = 0; i < 3; i++)
      //  if(index[i] != -1) vertex(i).addTriangle(this);

    //_normal = triangle.normal();
}

const Vertex &Triangle::vertex(int ind) const
{ return Model::vertex()[_index[ind]]; }
Vertex &Triangle::vertex(int ind) { return Model::vertex()[_index[ind]]; }
void Triangle::setIndex(int num, int newIndex) { _index[num] = newIndex; }

void Triangle::remove()
{
    Element::remove();
    for(int i = 0; i < 3; i++) vertex(i).delTriangle(this);
}
