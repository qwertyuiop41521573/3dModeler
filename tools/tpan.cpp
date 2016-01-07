#include "tpan.h"
#include "glwidget.h"
#include "mathfunctions.h"
#include "workspace.h"

TPan::TPan() : Tool()
{
    button->setText("Pan");
}

void TPan::function(Action action, QMouseEvent *event)
{
    if(action != EXECUTE) return;
    GLWidget *widget = Workspace::activeWidget();
    double dx = widget->getLastPosition().x() - event->x() + widget->getHalfWidth();
    double dy = widget->getLastPosition().y() - widget->getHalfHeight() + event->y();
    const QVector3D &rotation = widget->getCamera().rotation();
    QVector3D dr(dx * sinR(rotation.z()) + dy * sinR(rotation.x()) * cosR(rotation.z()), dx *-cosR(rotation.z()) + dy * sinR(rotation.x()) * sinR(rotation.z()), dy * cosR(rotation.x()));
    dr /= widget->getProjection() == PERSPECTIVE ? 60 :widget->getScale();
    widget->getCamera().addToPosition(dr);
}
