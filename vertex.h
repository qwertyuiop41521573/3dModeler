#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QMatrix4x4>

#include "camera.h"
#include "element.h"

class Vertex : public Element
{
public:
    Vertex(const QVector3D &position = { 0, 0, 0 });

    QVector3D &position()
    { return _position; };

    const QVector3D &positionRO() const
    { return _position; };

    void setPosition(const QVector3D &vector)
    { _position = vector; };

    void setPosition(Axis axis, double value)
    { _position[axis] = value; };

    void move(const QVector3D &vector)
    { _position += vector; };

    void setPosition(float x, float y);
    void setPosition(float x, float y, float z);
    void setPosition(const QVector4D &pos);
    void setUV(float u, float v);

    bool operator ==(const Vertex &vertex)
    { return _position == vertex.positionRO(); };

    void operator =(const Vertex &vertex);

private:
    QVector3D _position;
    QVector2D UVCoordinates;
};

#endif // VERTEX_H
