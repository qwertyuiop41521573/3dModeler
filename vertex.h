#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QMatrix4x4>

#include "camera.h"
#include "element.h"
#include "types.h"

#include <vector>

using namespace std;

class Triangle;

class AdditiveMap : public map<int, QVector3D>
{
public:
    void push(int smoothingGroup, const QVector3D &normal)
    {
        if(!count(smoothingGroup))
            insert(pair<int, QVector3D>(smoothingGroup, normal));
        else at(smoothingGroup) += normal;
    }
};

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
    void setPosition(const QVector4D &pos) { _position = QVector3D(pos); }
    void setUV(float u, float v);

    bool operator ==(const Vertex &vertex)
    { return _position == vertex.position(); }

    void operator =(const Vertex &vertex);

    void addTriangle(tr_it iterator);
    void delTriangle(Triangle *triangle);
    const vector<tr_it> &triangles() const { return _triangles; }

    void countNormals();
    const QVector3D &normal(int smoothingGroup) const
    { return _normals.at(smoothingGroup); }

private:
    QVector3D _position;
    QVector2D UVCoordinates;
    AdditiveMap _normals;

    vector<tr_it> _triangles;
};

#endif // VERTEX_H
