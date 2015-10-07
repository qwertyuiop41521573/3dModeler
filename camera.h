#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QVector4D>

typedef enum { X, Y, Z } Axis;

class Camera
{
public:
    Camera();

    void addToPosition(const QVector3D &vector)
    { _position += vector; };

    void addToPosition(double x, double y, double z)
    { _position += QVector3D(x, y, z); };

    const QVector3D &position()
    { return _position; };

    void setPosition(double x, double y, double z)
    { _position = { x, y, z }; };

    void setPosition(const QVector4D &vector)
    { _position = QVector3D(vector); };

    const QVector3D &rotation()
    { return _rotation; };

    void setRotation(double x, double y, double z)
    { _rotation = { x, y, z }; };

    void setRotation(Axis axis, double value)
    { _rotation[axis] = value; };

    void addToRotation(Axis axis, double value);

private:
    QVector3D _position;
    QVector3D _rotation;
};

#endif // CAMERA_H
