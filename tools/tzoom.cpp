#include "tzoom.h"
#include "glwidget.h"
#include "functions.h"

TZoom::TZoom(MainWindow *mainWindow) : Tool(mainWindow)
{
    button->setText("Zoom");
}

void TZoom::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if(action != EXECUTE) return;
    bool perspective = widget->getProjection() == PERSPECTIVE;
    double dy = (widget->getHalfHeight() - event->y() - widget->
                 getLastPosition().y()) / (perspective ? 40 : 100);

    if(perspective)
    {
        QVector3D rotation = widget->getCamera()->rotation();
        widget->getCamera()->addToPosition(dy * cosR(rotation.x()) * cosR(rotation.z()), dy * cosR(rotation.x()) * sinR(rotation.z()), -dy * sinR(rotation.x()));
    }
    else widget->multiplyScaleBy(exp(dy));
}
