#include "tpan.h"
#include "glwidget.h"
#include "functions.h"

TPan::TPan(MainWindow *mainWindow) : Tool(mainWindow)
{
    button->setText("Pan");
}

void TPan::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if(action == EXECUTE)
    {
        double dx = widget->getLastPosition().x() - event->x() + widget->
                getHalfWidth();
        double dy = widget->getLastPosition().y() - widget->getHalfHeight() +
                event->y();
        double radZ = inRadians(widget->getCamera()->rotation().z());
        double radX = inRadians(widget->getCamera()->rotation().x());
        QVector3D dr(dx * sin(radZ) + dy * sin(radX) * cos(radZ),
                     dx *-cos(radZ) + dy * sin(radX) * sin(radZ),
                     dy * cos(radX));
        dr /= widget->getProjection() == PERSPECTIVE ? 60 :widget->getScale();
        widget->getCamera()->addToPosition(dr);
    }
}
