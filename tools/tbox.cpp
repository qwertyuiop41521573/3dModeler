#include "tbox.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"

#include "gui/mylabel.h"

TBox::TBox(MainWindow *mainWindow) : TPlane(mainWindow)
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
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _hasStage2 = true;
    connect(checkBoxCube, SIGNAL(clicked(bool)), this, SLOT(handleCubeClick(bool)));

    _widget->hide();
}

void TBox::function(Action action, QMouseEvent *event)
{
    //in DRAW section, tool draws some lines in viewport, ex. rectangle
    //    for tool select
    if(action == DRAW) return;

    if(action != STOP && action != STAGE2) TPlane::function(action, event);

    GLWidget *widget = *_activeWidget;
    VertexContainer &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
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
            ind.push_back(vertex.push(v));
            vertex[ind[i]].setSelected(true);
        }

        //triangles of the box:
        triangle.push_back({ind[0], ind[1], ind[2]});
        triangle.push_back({ind[0], ind[2], ind[3]});
        triangle.push_back({ind[0], ind[5], ind[1]});
        triangle.push_back({ind[0], ind[4], ind[5]});
        triangle.push_back({ind[1], ind[6], ind[2]});
        triangle.push_back({ind[1], ind[5], ind[6]});
        triangle.push_back({ind[2], ind[7], ind[3]});
        triangle.push_back({ind[2], ind[6], ind[7]});
        triangle.push_back({ind[3], ind[4], ind[0]});
        triangle.push_back({ind[3], ind[7], ind[4]});
        triangle.push_back({ind[4], ind[6], ind[5]});
        triangle.push_back({ind[4], ind[7], ind[6]});

        break;
    }
    //if we drag pressed mouse in viewport
    case EXECUTE:
    {
        if(_stage2)
        {
            Projection projection = widget->getProjection();
            double dy = (widget->getHalfHeight() - event->y() - widget->getLastPosition().y()) / double(100);
            Camera &camera = widget->getCamera();
            const QVector3D &rotation = camera.rotation();
            QVector3D normal = (projection == PERSPECTIVE) ? QVector3D(0, 0, 1) : QVector3D(cosR(rotation.x()) * cosR(rotation.z()), cosR(rotation.x()) * sinR(rotation.z()), sinR(rotation.x()));

            //if cube
            if(checkBoxCube->isChecked() && dy != 0)
            {
                QVector3D dh = normal * sign(dy) * (vertex[ind[6]].getPosition() - vertex[ind[4]].getPosition()).length() / qSqrt(2);
                for(i = 0; i < 4; i++) vertex[ind[4 + i]].setPosition(vertex[ind[i]].getPosition() + dh);
            }
            else for(i = 0; i < 4; i++) vertex[ind[4 + i]].addToPosition(normal * dy);

            QVector3D diagonal = vertex[ind[6]].getPosition() - vertex[ind[4]].getPosition();
            QVector3D e_x = vertex[ind[7]].getPosition() - vertex[ind[4]].getPosition();

            //flip the box if needed
            if(QVector3D::dotProduct(normal, vertex[ind[0]].getPosition() - vertex[ind[4]].getPosition()) * QVector3D::dotProduct(normal,  QVector3D::crossProduct(e_x, diagonal)) > 0)
            {
                    QVector3D temp = vertex[ind[1]].getPosition();
                    vertex[ind[1]] = vertex[ind[3]];
                    vertex[ind[3]].setPosition(temp);
                    temp = vertex[ind[5]].getPosition();
                    vertex[ind[5]] = vertex[ind[7]];
                    vertex[ind[7]].setPosition(temp);
            }
        }
        break;
    }
    //last click in viewport for this tool
    case STOP:
    {
        //if height == 0
        if(vertex[ind[7]] == vertex[ind[3]])
        {
            //remove box - last 8 vertices and 12 triangles
            removeAll(12);
        }
        //deselect with "blue", select with "red"
        else for(i = 0; i < 8; i++) vertex[ind[i]].setSelected(true, false);

        widget->setToolIsOn(false);
        setStage2(false);
        widget->setMouseTracking(false);
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
            ind.push_back(vertex.push(vertex[ind[i]]));
            vertex[ind[4 + i]].setNewSelected(true);
        }

        triangle.push_back({ind[0], ind[5], ind[1]});
        triangle.push_back({ind[0], ind[4], ind[5]});
        triangle.push_back({ind[1], ind[6], ind[2]});
        triangle.push_back({ind[1], ind[5], ind[6]});
        triangle.push_back({ind[2], ind[7], ind[3]});
        triangle.push_back({ind[2], ind[6], ind[7]});
        triangle.push_back({ind[3], ind[4], ind[0]});
        triangle.push_back({ind[3], ind[7], ind[4]});
        triangle.push_back({ind[4], ind[6], ind[5]});
        triangle.push_back({ind[4], ind[7], ind[6]});


        widget->setMouseTracking(true);
    }
    }
}

void TBox::handleCubeClick(bool value)
{
    checkBoxSquare->setChecked(value);
    checkBoxSquare->setEnabled(!value);
}
