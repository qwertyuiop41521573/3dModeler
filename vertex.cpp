#include "vertex.h"
#include "triangle.h"

Vertex::Vertex(const QVector3D &position) : Element()
{
    _position = position;
    _isVertex = true;
}

void Vertex::setPosition(float x, float y)
{
    _position.setX(x);
    _position.setY(y);
    _position.setZ(0);
}

void Vertex::setPosition(float x, float y, float z)
{
    _position.setX(x);
    _position.setY(y);
    _position.setZ(z);
}
    
void Vertex::setUV(float u, float v)
{
    UVCoordinates.setX(u);
    UVCoordinates.setY(v);
}

void Vertex::operator =(const Vertex &vertex)
{
    Element::operator =(vertex);
    _position = vertex.position();
    _triangles = vertex.triangles();
}

void Vertex::addTriangle(Triangle *triangle)
{
    int i;
    for(i = 0; i < _triangles.size(); i++)
        if(_triangles[i] == triangle) break;
    if(i == _triangles.size()) _triangles.push_back(triangle);
}

void Vertex::delTriangle(Triangle *triangle)
{
    for(int i = 0; i < _triangles.size(); i++) {
        if(_triangles[i] != triangle) continue;
        _triangles.erase(_triangles.begin() + i);
        return;
    }
}

void Vertex::countNormals()
{
    _normals.clear();
    for(int i = 0; i < _triangles.size(); i++)
        _normals.push(_triangles[i]->smoothingGroup(), _triangles[i]->normal());
    for(AdditiveMap::iterator i = _normals.begin(); i != _normals.end(); i++)
        i->second.normalize();
}
