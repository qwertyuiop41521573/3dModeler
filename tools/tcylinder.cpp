#include "tcylinder.h"
#include "glwidget.h"
#include "mainwindow.h"

#include "gui/mylabel.h"

TCylinder::TCylinder(MainWindow *mainWindow) : TEllipse(mainWindow)
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

    GLWidget *widget = *_activeWidget;
    VertexContainer &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
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
            removeAll(segments);
            return;
        }
        createWallsAndSecondCap(true);
        for(i = 0; i <= segments; i++) vertex[ind[segments + 1 + i]].setSelected(true);
        break;
    }
    //if(!_stage2) is done in TEllipse::function(action, event);
    case EXECUTE:
    {
        if(_stage2)
        {
            Projection projection = widget->getProjection();
            double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / double(100);

            for(i = 0; i <= segments; i++) vertex[ind[segments + 1 + i]].addToPosition(normal * dy);

            //v1 and v2 are not parallel vectors in cap, [v1, v2] is normal to cap
            QVector3D v1 = vertex[ind[0]].getPosition() - vertex[ind[segments]].getPosition();
            QVector3D v2 = vertex[ind[1]].getPosition() - vertex[ind[segments]].getPosition();

            //flip if needed
            if(QVector3D::dotProduct(normal, vertex[ind[segments]].getPosition() - vertex[ind[2 * segments + 1]].getPosition()) * QVector3D::dotProduct(normal, QVector3D::crossProduct(v1, v2)) > 0)
            {
                QVector3D temp;
                for(i = 0; i < segments / 2; i++)
                {
                    temp = vertex[ind[segments + 1 + i]].getPosition();
                    vertex[ind[segments + 1 + i]] = vertex[ind[2 * segments - i]];
                    vertex[ind[2 * segments - i]].setPosition(temp);
                    temp = vertex[ind[i]].getPosition();
                    vertex[ind[i]] = vertex[ind[segments - 1 - i]];
                    vertex[ind[segments - 1 - i]].setPosition(temp);
                }
            }
        }
        break;
    }
    case STOP:
    {
        //if height == 0 we remove cylinder (2 * segments + 2 vertices and 4 *
        //    segments triangles
        if(vertex[ind[2 * segments + 1]] == vertex[ind[segments]]) removeAll(4 * segments);
        else for(i = 0; i < 2 * segments + 2; i++) vertex[ind[i]].setSelected(true, false);
        widget->setToolIsOn(false);
        setStage2(false);
        widget->setMouseTracking(false);
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

        for(i = 0; i <= segments; i++) vertex[ind[segments + 1 + i]].setNewSelected(true);

        widget->setMouseTracking(true);
    }
    }
}

void TCylinder::createWallsAndSecondCap(bool final)
{
    VertexContainer &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int segments = spinBoxSegments->value();
    int i;

    QVector3D height = final ? normal * spinBoxHeight->value() : QVector3D(0, 0, 0);
    //add vertices for second cap
    //second cap
    for(i = 0; i <= segments; i++) ind.push_back(vertex.push(vertex[ind[i]].getPosition() + height));

    //add triangles for walls and second cap

    //cap
    for(i = 0; i < segments - 1; i++) triangle.push_back({ind[segments + 1 + i], ind[segments + 1 + i + 1], ind[segments + 1 + segments]});
    triangle.push_back({ind[segments + 1 + segments - 1], ind[segments + 1], ind[segments + 1 + segments]});

    //wall
    for(i = 0; i < segments - 1; i++)
    {
        triangle.push_back({ind[segments + 1 + i + 1], ind[segments + 1 + i], ind[i]});
        triangle.push_back({ind[i], ind[1 + i], ind[segments + 1 + i + 1]});
    }
    triangle.push_back({ind[segments + 1], ind[segments + 1 + segments - 1], ind[segments - 1]});
    triangle.push_back({ind[segments - 1], ind[0], ind[segments + 1]});
}
