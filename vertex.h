#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QMatrix4x4>

#include "camera.h"
#include "element.h"

#include <vector>

using namespace std;

class Triangle;

class Vertex : public Element
{
public:
    Vertex(const QVector3D &position = { 0, 0, 0 });

    QVector3D &position() { return _position; }
    const QVector3D &position() const { return _position; }
    void setPosition(const QVector3D &vector) { _position = vector; }
    void setPosition(Axis axis, double value) { _position[axis] = value; }
    void move(const QVector3D &vector) { _position += vector; }

    void setPosition(float x, float y);
    void setPosition(float x, float y, float z);
    void setPosition(const QVector4D &pos);
    void setUV(float u, float v);

    bool operator ==(const Vertex &vertex)
    { return _position == vertex.position(); }

    void operator =(const Vertex &vertex);

    void addTriangle(Triangle *triangle);
    void delTriange(Triangle *triangle);
    const vector<Triangle*> &triangles() const { return _triangles; }

private:
    QVector3D _position;
    QVector2D UVCoordinates;

    vector<Triangle*> _triangles;
};

#endif // VERTEX_H
