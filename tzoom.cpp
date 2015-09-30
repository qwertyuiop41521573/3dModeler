#include "tzoom.h"
#include "glwidget.h"
#include "functions.h"

TZoom::TZoom(MainWindow *mainWindow) : Tool(mainWindow)
{
    button->setText("Zoom");
}

void TZoom::function(Action action, QMouseEvent
                     *event, VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if(action != EXECUTE) return;
    bool perspective = widget->getProjection() == PERSPECTIVE;
    double dy = (widget->getHalfHeight() - event->y() - widget->
                 getLastPosition().y()) / (perspective ? 40 : 100);

    if(perspective)
    {
        QVector3D rotation = widget->getCamera()->rotation();
        double radZ = inRadians(rotation.z());
        double radX = inRadians(rotation.x());
        widget->getCamera()->addToPosition(dy * cos(radX) * cos(radZ),
                         dy * cos(radX) * sin(radZ), -dy * sin(radX));
    }
    else widget->multiplyScaleBy(exp(dy));
}
