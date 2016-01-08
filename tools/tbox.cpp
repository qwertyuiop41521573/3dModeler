#include "tbox.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"
#include "journal.h"

#include "gui/mylabel.h"

using namespace Model;
using namespace Journal;

TBox::TBox() : TPlane()
{
    button->setText("Box");
    finalButton = new QPushButton("Create Box");
    int i;

    checkBoxCube = new QCheckBox;
    checkBoxCube->setText("Cube");
    layout->addWidget(checkBoxCube, 0, 2, 1, 2);

    QLabel *center = new QLabel("Center");
    QLabel *size = new QLabel("Size");
    layout->addWidget(center, 1, 0, 1, 2);
    layout->addWidget(size, 1, 2, 1, 4);

    MyLabel *label[6];
    for(i = 0; i < 6; i++)
    {
        spinBox[i] = new MySpinBox;
        label[i] = new MyLabel(QString('X' + i % 3) + ':', 25);
        int x = 2 * (i / 3), y = 2 + i % 3;
        layout->addWidget(label[i], y, x);
        layout->addWidget(spinBox[i], y, x + 1);
    }
    for(i = 0; i < 3; i++)
    {
        spinBox[3 + i]->setMinimum(0);
        spinBox[3 + i]->setValue(1);
    }

    layout->addWidget(finalButton, 6, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _hasStage2 = true;
    connect(checkBoxCube, SIGNAL(toggled(bool)), this, SLOT(handleCubeClick(bool)));

    _widget->hide();

    _ctrl = checkBoxCube;
}

void TBox::function(Action action, QMouseEvent *event)
{
    //in DRAW section, tool draws some lines in viewport, ex. rectangle
    //    for tool select
    if(action == DRAW) return;

    if(action != STOP && action != STAGE2) TPlane::function(action, event);

    GLWidget *widget = Workspace::activeWidget();
    int i;

    switch(action)
    {
    //if "Create Box" was pressed
    case FINAL:
    {
        for(i = 0; i < 3; i++) if(spinBox[i + 3]->value() == 0) return;

        QVector3D center, size;
        for(i = 0; i < 3; i++)
        {
            center[i] = spinBox[i]->value();
            size[i] = spinBox[i + 3]->value();
        }

        Journal::newRecord(CREATE);
        for(i = 0; i < 8; i++)
        {
            //cube with center at(0.5, 0.5, 0.5) and size(1, 1, 1)
            QVector3D v((i / 2) % 2, ((i + 1) / 2) % 2, i / 4);
            //move it's center to (0, 0, 0)
            v -= QVector3D(0.5, 0.5, 0.5);
            //stretch is to needed size
            v *= size;
            //move it's center where needed
            v += center;
            //record to model
            ver.push_back(vertex().push(v));
            vertex()[ver[i]].setSelected(true);
        }

        //triangles of the box:
        tri.push_back(triangle().push({ver[0], ver[1], ver[2]}));
        tri.push_back(triangle().push({ver[0], ver[2], ver[3]}));
        tri.push_back(triangle().push({ver[0], ver[5], ver[1]}));
        tri.push_back(triangle().push({ver[0], ver[4], ver[5]}));
        tri.push_back(triangle().push({ver[1], ver[6], ver[2]}));
        tri.push_back(triangle().push({ver[1], ver[5], ver[6]}));
        tri.push_back(triangle().push({ver[2], ver[7], ver[3]}));
        tri.push_back(triangle().push({ver[2], ver[6], ver[7]}));
        tri.push_back(triangle().push({ver[3], ver[4], ver[0]}));
        tri.push_back(triangle().push({ver[3], ver[7], ver[4]}));
        tri.push_back(triangle().push({ver[4], ver[6], ver[5]}));
        tri.push_back(triangle().push({ver[4], ver[7], ver[6]}));

        Journal::submit();
        break;
    }
    //if we drag pressed mouse in viewport
    case EXECUTE:
    {
        if(!_stage2) break;
        Projection projection = widget->getProjection();
        double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / double(100);
        Camera &camera = widget->getCamera();
        const QVector3D &rotation = camera.rotation();
        QVector3D normal = (projection == PERSPECTIVE) ? QVector3D(0, 0, 1) : QVector3D(cosR(rotation.x()) * cosR(rotation.z()), cosR(rotation.x()) * sinR(rotation.z()), sinR(rotation.x()));

        //if cube
        if(checkBoxCube->isChecked() && dy != 0)
        {
            QVector3D dh = normal * sign(dy) * (vertex()[ver[6]].position() - vertex()[ver[4]].position()).length() / qSqrt(2);
            for(i = 0; i < 4; i++) vertex()[ver[4 + i]].setPosition(vertex()[ver[i]].position() + dh);
        }
        else for(i = 0; i < 4; i++) vertex()[ver[4 + i]].move(normal * dy);

        QVector3D diagonal = vertex()[ver[6]].position() - vertex()[ver[4]].position();
        QVector3D e_x = vertex()[ver[7]].position() - vertex()[ver[4]].position();

        //flip the box if needed
        if(QVector3D::dotProduct(normal, vertex()[ver[0]].position() - vertex()[ver[4]].position()) * QVector3D::dotProduct(normal,  QVector3D::crossProduct(e_x, diagonal)) <= 0) break;
        QVector3D temp = vertex()[ver[1]].position();
        vertex()[ver[1]] = vertex()[ver[3]];
        vertex()[ver[3]].setPosition(temp);
        temp = vertex()[ver[5]].position();
        vertex()[ver[5]] = vertex()[ver[7]];
        vertex()[ver[7]].setPosition(temp);
        break;
    }
    //last click in viewport for this tool
    case STOP:
    {
        //if height == 0
        if(vertex[ver[7]] == vertex[ver[3]])
        {
            //remove box - last 8 vertices and 12 triangles
            removeAll();
            leave();
            break;
        }
        //deselect with "blue", select with "red"
        for(i = 0; i < 8; i++) vertex()[ver[i]].setSelected(true, false);
        leave();
        Journal::submit();
        break;
    }
    //"stage1" was dragging pressed mouse (and drawing the cap of box),
    //    stage2 is moving released mouse in vertical direction (changing
    //    the height of box);    STAGE2 is ran once, like START or STOP,
    //    EXECUTE is ran every time GLWidget recieves mouse move event
    case STAGE2:
    {
        //check if plane has zero height or width
        planeFailed = false;
        TPlane::function(STOP, event);
        if(planeFailed) return;

        widget->setToolIsOn(true);
        setStage2(true);

        //adding elements to create box from it's cap (4 vertices and 10
        //    triangles)
        for(i = 0; i < 4; i++)
        {
            //the second cap is the same as first
            ver.push_back(vertex().push(vertex()[ver[i]].position()));
            vertex()[ver[4 + i]].setNewSelected(true);
        }

        //triangles for walls and second cap
        tri.push_back(triangle().push({ver[0], ver[5], ver[1]}));
        tri.push_back(triangle().push({ver[0], ver[4], ver[5]}));
        tri.push_back(triangle().push({ver[1], ver[6], ver[2]}));
        tri.push_back(triangle().push({ver[1], ver[5], ver[6]}));
        tri.push_back(triangle().push({ver[2], ver[7], ver[3]}));
        tri.push_back(triangle().push({ver[2], ver[6], ver[7]}));
        tri.push_back(triangle().push({ver[3], ver[4], ver[0]}));
        tri.push_back(triangle().push({ver[3], ver[7], ver[4]}));
        tri.push_back(triangle().push({ver[4], ver[6], ver[5]}));
        tri.push_back(triangle().push({ver[4], ver[7], ver[6]}));

        widget->setMouseTracking(true);
    }
    }
}

void TBox::handleCubeClick(bool value)
{
    checkBoxSquare->setChecked(value);
    checkBoxSquare->setEnabled(!value);
}

void TBox::leave()
{
    GLWidget *widget = Workspace::activeWidget();
    widget->setToolIsOn(false);
    setStage2(false);
    widget->setMouseTracking(false);
    _busy = false;
}
