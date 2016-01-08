#include "vertex.h"

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

void Vertex::setPosition(const QVector4D &pos)
{
    _position = QVector3D(pos);
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
}
