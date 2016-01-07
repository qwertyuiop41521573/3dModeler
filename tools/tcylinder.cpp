#include "tcylinder.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "model.h"
#include "journal.h"

#include "gui/mylabel.h"

using namespace Model;

TCylinder::TCylinder() : TEllipse()
{
    button->setText("Cylinder");
    finalButton->setText("Create Cylinder");

    MyLabel *height = new MyLabel("Height", 70);
    spinBoxHeight = new MySpinBox;
    spinBoxHeight->setMinimum(0);
    spinBoxHeight->setValue(1);

    layout->addWidget(height, 7, 0, 1, 2);
    layout->addWidget(spinBoxHeight, 7, 2, 1, 2);

    _hasStage2 = true;
    _widget->show();
}

void TCylinder::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    //cylinder's STAGE2 has much code from ellipse's STOP
    if(action != STOP && action != STAGE2) TEllipse::function(action, event);

    GLWidget *widget = Workspace::activeWidget();
    int segments = spinBoxSegments->value();
    int i;


    switch(action)
    {
    //all START and most of FINAL (creating first cap) was done in
    //    TEllipse::function(action, event);
    case FINAL:
    {
        if(ellipseFailed) return;
        if(spinBoxHeight->value() == 0)
        {
            removeAll();
            return;
        }
        createWallsAndSecondCap(true);
        for(i = 0; i <= segments; i++) vertex()[ver[segments + 1 + i]].setSelected(true);
        Journal::submit();
        break;
    }
    //if(!_stage2) is done in TEllipse::function(action, event);
    case EXECUTE:
    {
        if(!_stage2) break;
        Projection projection = widget->getProjection();
        double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / double(100);

        for(i = 0; i <= segments; i++) vertex()[ver[segments + 1 + i]].move(normal * dy);

        //v1 and v2 are not parallel vectors in cap, [v1, v2] is normal to cap
        QVector3D v1 = vertex()[ver[0]].positionRO() - vertex()[ver[segments]].positionRO();
        QVector3D v2 = vertex()[ver[1]].positionRO() - vertex()[ver[segments]].positionRO();
        QVector3D h = vertex()[ver[segments]].positionRO() - vertex()[ver[2 * segments + 1]].positionRO();

        //flip if needed
        if(QVector3D::dotProduct(h, QVector3D::crossProduct(v1, v2)) <= 0) break;
        QVector3D temp;
        for(i = 0; i < segments / 2; i++)
        {
            temp = vertex()[ver[segments + 1 + i]].positionRO();
            vertex()[ver[segments + 1 + i]] = vertex()[ver[2 * segments - i]];
            vertex()[ver[2 * segments - i]].setPosition(temp);
            temp = vertex()[ver[i]].positionRO();
            vertex()[ver[i]] = vertex()[ver[segments - 1 - i]];
            vertex()[ver[segments - 1 - i]].setPosition(temp);
        }
        break;
    }
    case STOP:
    {
        //if height == 0 we remove cylinder (2 * segments + 2 vertices and 4 *
        //    segments triangles
        if(vertex[ver[2 * segments + 1]] == vertex[ver[segments]])
        {
            removeAll();
            leave();
            break;
        }
        for(i = 0; i < 2 * segments + 2; i++) vertex()[ver[i]].setSelected(true, false);
        leave();
        Journal::submit();
        break;
    }
    case STAGE2:
    {
        ellipseFailed = false;
        TEllipse::function(STOP, event);
        if(ellipseFailed) return;

        widget->setToolIsOn(true);
        setStage2(true);

        createWallsAndSecondCap(false);

        for(i = 0; i <= segments; i++) vertex()[ver[segments + 1 + i]].setNewSelected(true);

        widget->setMouseTracking(true);
    }
    }
}

void TCylinder::createWallsAndSecondCap(bool final)
{
    int segments = spinBoxSegments->value();
    int i;

    QVector3D height = final ? normal * spinBoxHeight->value() : QVector3D(0, 0, 0);
    //add vertices for second cap
    for(i = 0; i <= segments; i++) ver.push_back(vertex().push(vertex()[ver[i]].positionRO() + height));

    //add triangles for walls and second cap

    //cap
    for(i = 0; i < segments; i++) tri.push_back(triangle().push({ver[segments + 1 + i], ver[segments + 1 + (i + 1) % segments], ver[segments + 1 + segments]}));

    //wall
    for(i = 0; i < segments; i++)
    {
        int t = (i + 1) % segments;
        tri.push_back(triangle().push({ver[segments + 1 + t], ver[segments + 1 + i], ver[i]}));
        tri.push_back(triangle().push({ver[i], ver[t], ver[segments + 1 + t]}));
    }
}

void TCylinder::leave()
{
    GLWidget *widget = Workspace::activeWidget();
    widget->setToolIsOn(false);
    setStage2(false);
    widget->setMouseTracking(false);
    _busy = false;
}
