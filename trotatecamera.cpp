#include "trotatecamera.h"
#include "glwidget.h"

TRotateCamera::TRotateCamera(MainWindow *mainWindow, QString
                      buttonText) : Tool(mainWindow, buttonText)
{

}

void TRotateCamera::function(Action action,
                     QMouseEvent *event, VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if(action != EXECUTE || widget->getProjection() != PERSPECTIVE)
        return;
    Camera *camera = widget->getCamera();
    camera->addToRotation(Z, (-event->x() + widget->getHalfWidth() +
        widget->getLastPosition().x()) / double(3));
    double newRotationX = camera->rotation().x() + (widget->
        getLastPosition().y() - widget->getHalfHeight() + event->y())
            / double(3);
    if(newRotationX >-90 && newRotationX < 90) camera->setRotation(X,
                                                       newRotationX);
}
