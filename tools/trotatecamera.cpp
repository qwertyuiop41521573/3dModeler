#include "trotatecamera.h"
#include "glwidget.h"
#include "workspace.h"

TRotateCamera::TRotateCamera() : Tool()
{
    button->setText("Rotate");
}

void TRotateCamera::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = Workspace::activeWidget();
    if(action != EXECUTE || widget->getProjection() != PERSPECTIVE) return;
    Camera &camera = widget->getCamera();
    camera.addToRotation(Z, (-event->x() + widget->getHalfWidth() + widget->getLastPosition().x()) / double(3));
    double newRotationX = camera.rotation().x() + (widget->getLastPosition().y() - widget->getHalfHeight() + event->y()) / double(3);
    if(newRotationX >-90 && newRotationX < 90) camera.setRotation(X, newRotationX);
}
