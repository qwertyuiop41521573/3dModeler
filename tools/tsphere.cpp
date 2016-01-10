#include "tsphere.h"
#include "mainwindow.h"
#include "mathfunctions.h"
#include "journal.h"

#include "gui/mylabel.h"

//#include <QMatrix3x3>

using namespace Model;
using namespace Journal;

TSphere::TSphere() : CreatingTool()
{
    button->setText("Sphere");
    finalButton = new QPushButton("Create Sphere");

    spinBoxSegmentsXY = new QSpinBox;
    spinBoxSegmentsXY->setMaximumWidth(70);
    spinBoxSegmentsXY->setMinimum(3);
    spinBoxSegmentsXY->setValue(18);
    spinBoxSegmentsXY->setMaximum(10000);
    MyLabel *segmentsXY = new MyLabel("Segments XY:", 150);
    segmentsXY->setAlignment(Qt::AlignLeft);
    
    spinBoxSegmentsZ = new QSpinBox;
    spinBoxSegmentsZ->setMaximumWidth(70);
    spinBoxSegmentsZ->setMinimum(4);
    spinBoxSegmentsZ->setValue(18);
    spinBoxSegmentsZ->setMaximum(10000);
    spinBoxSegmentsZ->setSingleStep(2);
    MyLabel *segmentsZ = new MyLabel("Segments Z:", 150);
    segmentsZ->setAlignment(Qt::AlignLeft);

    QLabel *center = new QLabel("Center");
    QLabel *normalLabel = new QLabel("Normal");

    MyLabel *label[6];

    int i;
    for(i = 0; i < 6; i++)
    {
        label[i] = new MyLabel(QString('X' + i % 3 ) + ':', 25);
        spinBox[i] = new MySpinBox;
    } 

    MyLabel *radius = new MyLabel("Radius:", 70);
   
    spinBoxRadius = new MySpinBox;
    spinBoxRadius->setMinimum(0);
    spinBoxRadius->setValue(1);
    

    layout->addWidget(segmentsXY, 0, 0, 1, 4);
    layout->addWidget(spinBoxSegmentsXY, 1, 0, 1, 4);
    layout->addWidget(segmentsZ, 2, 0, 1, 4);
    layout->addWidget(spinBoxSegmentsZ, 3, 0, 1, 4);
    layout->addWidget(center, 4, 0, 1, 4);
    layout->addWidget(normalLabel, 4, 2, 1, 4);
    for(i = 0; i < 6; i++)
    {
        layout->addWidget(label[i], 5 + i % 3, 2 * (i / 3));
        layout->addWidget(spinBox[i], 5 + i % 3, 2 * (i / 3) + 1);
    }
    layout->addWidget(radius, 8, 0, 1, 2);
    layout->addWidget(spinBoxRadius, 8, 2, 1, 2);
    layout->addWidget(finalButton, 9, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _widget->hide();
}

void TSphere::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;
 
    GLWidget *widget = Workspace::activeWidget();
    int segmentsXY = spinBoxSegmentsXY->value();
    int segmentsZ = spinBoxSegmentsZ->value();
    int i, j;
 
    switch(action)
    {
    case START:
    {
        _busy = true;
        Journal::newRecord(CREATE);
        widget->countFinalInverseMatrix();
        widget->fromScreenToWorld(&startPosition3D, event);

        ver.clear();
        tri.clear();
        //shpere has 2 vertices on it's top and bottom (poles)
        //if normal (vector from bottom pole to top) is {0, 0, 1}, on side projection it has segmentsZ sides
        //this means that it has (segmentsZ / 2 - 1) rings, each consists of segmentsXY vertices
        //overall amount of vertices is "segmentsXY * (segmentsZ / 2 - 1) + 2"
        for(i = 0; i < segmentsXY * (segmentsZ / 2 - 1) + 2; i++)
        {
            //push all vertices to mouse pointer position
            ver.push_back(vertex().push(startPosition3D));
            vertex()[ver[i]].setNewSelected(true);
        }

        triangulate();

        break;
    }
    case FINAL:
    {
        for(i = 0; i < 3; i++) normal[i] = spinBox[3 + i]->value();
        double radius = spinBoxRadius->value();
        
        if(normal.length() == 0 || radius == 0) return;

        normal.normalize();
        QVector3D center;
        for(i = 0; i < 3; i++) center[i] = spinBox[i]->value();

        ver.clear();
        tri.clear();
        Journal::newRecord(CREATE);
        for(i = 0; i < segmentsXY * (segmentsZ / 2 - 1) + 2; i++) ver.push_back(vertex().push({{0, 0, 0}}));

        setVertices(center, radius);
        triangulate();

        for(i = 0; i < ver.size(); i++) vertex()[ver[i]].setSelected(true);
        Journal::submit();
        break;
    }
    case EXECUTE:
    {
        Projection projection = widget->getProjection();
        widget->countFinalInverseMatrix();

        QVector3D worldCoordinates;
        if(projection == PERSPECTIVE)
        {
            double height = startPosition3D.z();
            widget->fromScreenToWorld(&worldCoordinates, event, true, height);
            normal = { 0, 0, 1 };
        }
        else
        {
            QVector2D currentPosition = QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
            widget->_fromScreenToWorld(&worldCoordinates, QVector4D(currentPosition, 0, 1));
            const QVector3D &camRot = widget->getCamera().rotation();
            normal = QVector3D(cosR(camRot.x()) * cosR(camRot.z()), cosR(camRot.x()) * sinR(camRot.z()), sinR(camRot.x()));
        }
        double radius = (worldCoordinates - startPosition3D).length();

        setVertices(startPosition3D, radius);
        break;
    }
    case STOP:
    {
        //if mouse was not moved and all sphere points match
        if(vertex()[ver[0]].position() == vertex()[ver[1]].position())
        {
            removeAll();
            _busy = false;
            return;
        }
        for(i = 0; i < ver.size(); i++) vertex()[ver[i]].setSelected(true, false);
        _busy = false;
        Journal::submit();
        break;
    }
    }
}

void TSphere::triangulate()
{
    int segmentsXY = spinBoxSegmentsXY->value();
    int segmentsZ = spinBoxSegmentsZ->value();

    int i, j;

    //create top and bottom caps
    for(i = 0; i < segmentsXY; i++)
    {
        addTriangle(0, 1 + i, 1 + (1 + i) % segmentsXY, 0);
        addTriangle(ver.size() - 1, ver.size() - 2 - i, ver.size() - 2 - (1 + i) % segmentsXY, 0);
    }
    //create walls (triangles on neighbour vertex rings)
    for(i = 0; i < segmentsZ / 2 - 2; i++)
    {
        for(j = 0; j < segmentsXY; j++)
        {
            int st = 1 + segmentsXY * i;
            int last = (st + j) % segmentsXY;
            addTriangle(st + j, st + segmentsXY + j, st + last, 0);
            addTriangle(st + last, st + segmentsXY + j, st + segmentsXY + last, 0);
        }
    }
}

void TSphere::setVertices(const QVector3D &center, double radius)
{
    int segmentsXY = spinBoxSegmentsXY->value();
    int segmentsZ = spinBoxSegmentsZ->value();
    int i, j;

    //if normal == {1, 0, 0}, e_x = {0, 0, 1}
    //else e_x = [normal, [{1, 0, 0}, normal]], [] for cross product
    QVector3D e_x = qAbs(normal.x()) == 1 ? QVector3D(0, 0, 1) : QVector3D::crossProduct(normal, QVector3D::crossProduct(QVector3D(1, 0, 0), normal)).normalized();

    //similar to setting vertices in TEllipse, but now with 2 rotations - around normal and around e_x (ellipse had only first rotation)

    //this rotates around e_x
    QVector4D rotatingVertex = QVector4D(normal, 1);
    double angle = 360 / double(segmentsZ);
    //and this around normal
    QVector4D rotatingVertexNormal;
    double angleNormal =  360 / double(segmentsXY);

    QMatrix4x4 scaleAndTranslate;
    scaleAndTranslate.setToIdentity();
    scaleAndTranslate.translate(center);
    scaleAndTranslate.scale(radius, radius, radius);

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(angle, e_x);

    QMatrix4x4 rotationNormal;
    rotationNormal.setToIdentity();
    rotationNormal.rotate(angleNormal, normal);

    //top pole
    vertex()[ver[0]].setPosition(scaleAndTranslate * QVector4D(normal, 1));
    for(i = 0; i < segmentsZ / 2 - 1; i++)
    {
        //switch to next ring
        rotatingVertex = rotation * rotatingVertex;
        rotatingVertexNormal = rotatingVertex;
        //create ring
        for(j = 0; j < segmentsXY; j++)
        {
            rotatingVertexNormal = rotationNormal * rotatingVertexNormal;
            vertex()[ver[1 + segmentsXY * i + j]].setPosition(scaleAndTranslate * rotatingVertexNormal);
        }
    }
    //bottom pole
    vertex()[ver[ver.size() - 1]].setPosition(scaleAndTranslate * QVector4D(-normal, 1));
}
