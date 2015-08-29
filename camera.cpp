#include "camera.h"

Camera::Camera()
{
}

void Camera::addToRotation( Axis axis, double value )
{
    _rotation[ axis ] += value;
    while ( _rotation[ axis ] < 0 )
        _rotation[ axis ] += 360;
    while ( _rotation[ axis ] > 360 )
        _rotation[ axis ] -= 360;
}
