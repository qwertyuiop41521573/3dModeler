#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include "camera.h"
#include "selectableobject.h"

class Vertex : public SelectableObject
{
public:
    Vertex(const QVector3D &newPosition = { 0, 0, 0 } )
    { position = newPosition; };

    const QVector3D &getPosition() const
    { return position; };

    void setPosition(const QVector3D &vector )
    { position = vector; };

    void setPosition( Axis axis, double value )
    { position[ axis ] = value; };

    void addToPosition(const QVector3D &vector )
    { position += vector; };

    void multiplyPosition(const QMatrix4x4 &matrix )
    { position = QVector3D( matrix * QVector4D( position, 1 ) ); };

    void setPosition( float x, float y );
    void setPosition( float x, float y, float z );
    void setUV( float u, float v );

    bool operator ==(const Vertex &vertex)
    { return position == vertex.getPosition(); };

private:
    QVector3D position;
    QVector2D UVCoordinates;
};

#endif // VERTEX_H
