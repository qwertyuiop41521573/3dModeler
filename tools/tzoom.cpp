#include "tzoom.h"
#include "glwidget.h"
#include "mathfunctions.h"
#include "workspace.h"

TZoom::TZoom() : Tool()
{
    button->setText("Zoom");
}

void TZoom::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = Workspace::activeWidget();
    if(action != EXECUTE) return;
    bool perspective = widget->getProjection() == PERSPECTIVE;
    double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / (perspective ? 40 : 100);

    if(perspective)
    {
        const QVector3D &rotation = widget->getCamera().rotation();
        widget->getCamera().addToPosition(dy * cosR(rotation.x()) * cosR(rotation.z()), dy * cosR(rotation.x()) * sinR(rotation.z()), -dy * sinR(rotation.x()));
    }
    else widget->multiplyScaleBy(exp(dy));
}
