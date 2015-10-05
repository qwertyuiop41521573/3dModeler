#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include "camera.h"

class Vertex
{
public:
    Vertex( QVector3D newPosition = { 0, 0, 0 } )
    { position = newPosition; };

    void setSelected( bool value )
    { _isSelected = value; };

    bool isSelected()
    { return _isSelected; };

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

    bool newSelected()
    { return _newSelected; };

    void setNewSelected( bool value )
    { _newSelected = value; };

    bool operator ==( Vertex vertex )
    { return position == vertex.getPosition(); };

private:
    bool _isSelected = false;
    bool _newSelected = false;
    QVector3D position;
    QVector2D UVCoordinates;
};

#endif // VERTEX_H
