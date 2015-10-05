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
    Vertex( QVector3D newPosition = { 0, 0, 0 } )
    { position = newPosition; };

    QVector3D getPosition()
    { return position; };

    void setPosition( QVector3D vector )
    { position = vector; };

    void setPosition( Axis axis, double value )
    { position[ axis ] = value; };

    void addToPosition( QVector3D vector )
    { position += vector; };

    void multiplyPosition( QMatrix4x4 matrix )
    { position = QVector3D( matrix * QVector4D( position, 1 ) ); };

    void setPosition( float x, float y );
    void setPosition( float x, float y, float z );
    void setUV( float u, float v );

    bool operator ==( Vertex vertex )
    { return position == vertex.getPosition(); };

private:
    QVector3D position;
    QVector2D UVCoordinates;
};

#endif // VERTEX_H
