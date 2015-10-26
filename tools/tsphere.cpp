#include "tsphere.h"
#include "mainwindow.h"

#include "gui/mylabel.h"

//#include <QMatrix3x3>

TSphere::TSphere(MainWindow *mainWindow) : CreatingTool(mainWindow)
{
    button->setText("Sphere");
    finalButton->setText("Create Sphere");

    spinBoxSegmentsXY = new QSpinBox;
    spinBoxSegmentsXY->setMaximumWidth(55);
    spinBoxSegmentsXY->setMinimum(3);
    spinBoxSegmentsXY->setValue(18);
    spinBoxSegmentsXY->setMaximum(10000);
    MyLabel *segmentsXY = new MyLabel("Segments XY:", 70);
    
    spinBoxSegmentsZ = new QSpinBox;
    spinBoxSegmentsZ->setMaximumWidth(55);
    spinBoxSegmentsZ->setMinimum(4);
    spinBoxSegmentsZ->setValue(18);
    spinBoxSegmentsZ->setMaximum(10000);
    MyLabel *segmentsZ = new MyLabel("Segments Z:", 70);

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
    

    layout->addWidget(segmentsXY, 0, 0, 1, 2);
    layout->addWidget(spinBoxSegmentsXY, 0, 2, 1, 2);
    layout->addWidget(segmentsZ, 1, 0, 1, 2);
    layout->addWidget(spinBoxSegmentsZ, 1, 2, 1, 2);
    layout->addWidget(center, 2, 0, 1, 4);
    layout->addWidget(normalLabel, 2, 2, 1, 4);
    for(i = 0; i < 6; i++)
    {
        layout->addWidget(label[i], 3 + i % 3, 2 * (i / 3));
        layout->addWidget(spinBox[i], 3 + i % 3, 2 * (i / 3) + 1);
    }
    layout->addWidget(radius, 6, 0, 1, 2);
    layout->addWidget(spinBoxRadius, 6, 2, 1, 2);
    layout->addWidget(finalButton, 7, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();
}

void TSphere::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;
 
    GLWidget *widget = *_activeWidget;
    ElementContainer <Vertex> &vertex = model->vertex();
    ElementContainer <Triangle> &triangle = model->triangle();
    int segmentsXY = spinBoxSegmentsXY->value();
    int segmentsZ = spinBoxSegmentsZ->value();
    int j;
    int i;
 
    switch(action)
    {
    case START:
    {
        _busy = true;
        journal->newRecord(CREATE);
        widget->countFinalInverseMatrix();
        widget->fromScreenToWorld(&startPosition3D, event);

        ver.clear();
        tri.clear();
        for(i = 0; i < segmentsXY * (segmentsZ / 2 - 1) + 2; i++)
        {
            ver.push_back(vertex.push(startPosition3D));
            vertex[ver[i]].setNewSelected(true);
        }

        for(i = 0; i < segmentsXY; i++)
        {
            tri.push_back(triangle.push({ver[0], ver[1 + i], ver[1 + (1 + i) % segmentsXY]}));
            tri.push_back(triangle.push({ver[ver.size() - 1], ver[ver.size() - 2 - i], ver[ver.size() - 2 - (1 + i) % segmentsXY]}));
        }
        for(i = 0; i < segmentsZ / 2 - 2; i++)
        {
            for(j = 0; j < segmentsXY; j++)
            {
                int st = 1 + segmentsXY * i;
                int last = (st + j) % segmentsXY; 
                tri.push_back(triangle.push({ver[st + j], ver[st + last], ver[st + segmentsXY + j]}));
                tri.push_back(triangle.push({ver[st + last], ver[st + segmentsXY + j], ver[st + segmentsXY + last]}));
            }
        }

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

        QVector3D e_x = abs(normal.x()) == 1 ? QVector3D(0, 0, 1) : QVector3D::crossProduct(normal, QVector3D::crossProduct(QVector3D(1, 0, 0), normal)).normalized();
        QVector4D rotatingVertex = QVector4D(normal, 1);
        double angle = 360 / double(segmentsZ);
        QVector4D rotatingVertexNormal;
        double angleNormal =  360 / double(segmentsXY);
        QMatrix4x4 scaleAndTranslate;
        scaleAndTranslate.setToIdentity();
        scaleAndTranslate.translate(center);
        scaleAndTranslate.scale(radius, radius, radius);
       
        ver.clear();
        tri.clear();
        journal->newRecord(CREATE);
        for(i = 0; i < segmentsXY * (segmentsZ / 2 - 1) + 2; i++) ver.push_back(vertex.push({{0, 0, 0}}));

        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(angle, e_x);

        QMatrix4x4 rotationNormal;
        rotationNormal.setToIdentity();
        rotationNormal.rotate(angleNormal, normal);

        vertex[ver[0]].setPosition(normal);
        for(i = 0; i < segmentsZ / 2 - 1; i++)
        {
            rotatingVertex = rotation * rotatingVertex;
            rotatingVertexNormal = rotatingVertex;  
            for(j = 0; j < segmentsXY; j++)
            {
                rotatingVertexNormal = rotationNormal * rotatingVertexNormal;
                vertex[ver[1 + segmentsXY * i + j]].setPosition(rotatingVertexNormal);
            }
        }
        vertex[ver[ver.size() - 1]].setPosition(-normal);

        for(i = 0; i < ver.size(); i++) vertex[ver[i]].setPosition(QVector3D(scaleAndTranslate * QVector4D(vertex[ver[i]].positionRO(), 1)));
        /////////////

        break;
    }
    }
}




