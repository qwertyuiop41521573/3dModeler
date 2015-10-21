#include "torbit.h"
#include "glwidget.h"

TOrbit::TOrbit(MainWindow *mainWindow) : Tool(mainWindow)
{
    button->setText("Orbit");
}

void TOrbit::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if(action != EXECUTE || widget->getProjection() != PERSPECTIVE) return;

    Camera &camera = widget->getCamera();
    double rotZ = (-event->x() + widget->getHalfWidth() + widget->getLastPosition().x()) / double(2);
    double rotX = (widget->getLastPosition().y() - widget->getHalfHeight() + event->y()) / double(2);
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(rotZ, 0, 0, 1);
    camera.setPosition(rotation * QVector4D(camera.position()));
    camera.addToRotation(Z, rotZ);
    rotation.setToIdentity();
    rotation.rotate(camera.rotation().z() - 90, 0, 0, 1);
    rotation.rotate(-rotX, 1, 0, 0);
    rotation.rotate(90 - camera.rotation().z(), 0, 0, 1);
    double newRotationX = camera.rotation().x() + rotX;
    if(newRotationX < 90 && newRotationX >-90)
    {
        camera.setPosition(rotation * QVector4D(camera.position()));
        camera.setRotation(X, newRotationX);
    }
}
