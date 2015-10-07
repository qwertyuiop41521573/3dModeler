#include "tellipse.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"

#include "gui/mylabel.h"


#include <iostream>
using namespace std;

TEllipse::TEllipse(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Ellipse");
    finalButton = new QPushButton("Create Ellipse");

    int i;
    spinBoxSegments = new QSpinBox;
    spinBoxSegments->setMaximumWidth(55);
    spinBoxSegments->setMaximum(1000);
    spinBoxSegments->setMinimum(-1000);
    for(i = 0; i < 6; i++) spinBox[i] = new MySpinBox;
    spinBoxRadius = new MySpinBox;

    spinBoxSegments->setMinimum(3);
    spinBoxSegments->setValue(18);
    spinBoxSegments->setMaximum(10000);

    MyLabel *segments = new MyLabel("Segments:", 70);

    checkBoxCircle = new MyCheckBoxMW;
    checkBoxCircle->setText("Circle");

    QLabel *center = new QLabel("Center");
    QLabel *normalLabel = new QLabel("Normal");
    MyLabel *label[6];
    for(i = 0; i < 6; i++) label[i] = new MyLabel(QString('X' + i % 3 ) +
                                                  ':', 25);
    MyLabel *radius = new MyLabel("Radius:", 70);

    spinBoxRadius->setMinimum(0);
    spinBoxRadius->setValue(1);

    layout->addWidget(segments, 0, 0, 1, 2);
    layout->addWidget(spinBoxSegments, 0, 2, 1, 2);
    layout->addWidget(checkBoxCircle, 1, 0, 1, 4);
    layout->addWidget(center, 2, 0, 1, 4);
    layout->addWidget(normalLabel, 2, 2, 1, 4);
    for(i = 0; i < 6; i++)
    {
        layout->addWidget(label[i], 3 + i % 3, 2 * (i / 3));
        layout->addWidget(spinBox[i], 3 + i % 3, 2 * (i / 3) + 1);
    }
    layout->addWidget(radius, 6, 0, 1, 2);
    layout->addWidget(spinBoxRadius, 6, 2, 1, 2);
    layout->addWidget(finalButton, 8, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();
}

void TEllipse::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int segments = spinBoxSegments->value();
    int i;

    //argument should be true for cylinder and false for ellipse, so we
    //    can use _hasStage2 and do not create new variable
    switch(action)
    {
    case START:
    {
        allocateCap(_hasStage2);
        widget->countFinalInverseMatrix();
        widget->fromScreenToWorld(startPosition3D, event);
        for(i = 0; i <= segments; i++)
        {
            vertex[vertexSize + i] = startPosition3D;
            vertex[vertexSize + i].select(NEW);
        }
        break;
    }
    case FINAL:
    {
        ellipseFailed = false;
        for(i = 0; i < 3; i++) normal[i] = spinBox[3 + i]->value();
        double radius = spinBoxRadius->value();
        if(normal.length() == 0 || radius == 0)
        {
            ellipseFailed = true;
            return;
        }

        allocateCap(_hasStage2);
        normal.normalize();
        QVector3D center;
        for(i = 0; i < 3; i++) center[i] = spinBox[i]->value();
        //widget->countFinalInverseMatrix(false);
        //center of first cap
        vertex[vertexSize + segments].setPosition(center);

        //cap is created with rotating "QVector4D rotatingVertex" around
        //    center with "QMatrix4x4 rotation" (with every iteration, 1
        //    segment is passed); then "rotation" is copied to "QVector3D
        //    currentVertex", and it's scaled and moved with "QMatrix4x4
        //    scaleAndTranslate"

        //if normal is parallel to X axis, than e_y; else [e_x, normal]
        QVector4D rotatingVertex = (abs(normal.x()) == 1) ? QVector4D(0, 1, 0,
            1) : QVector4D(QVector3D::crossProduct(QVector3D(1, 0, 0),
                                     normal).normalized(), 1);
        double angle = 360 / double(segments);
        QMatrix4x4 scaleAndTranslate;
        scaleAndTranslate.setToIdentity();
        scaleAndTranslate.translate(center);
        scaleAndTranslate.scale(radius, radius, radius);

        createCap(rotatingVertex, angle, scaleAndTranslate);

        for(i = 0; i <= segments; i++) vertex[vertexSize + i].select();
        break;
    }

    //for cylinder, this is "if(!_stage2)", so it should be done if:
    //    _hasStage2 == false || (_hasStage2 = true && _stage2 == false)
    //    (with _hasStage2 we define if this code is executed by ellipse
    //    (false) or cylinder (true)
    case EXECUTE:
    {
        if(_hasStage2 ? _stage2 == false : true)
        {
            Projection projection = widget->getProjection();
            bool circle = checkBoxCircle->isChecked();
            widget->countFinalInverseMatrix();
            if(projection == PERSPECTIVE)
            {
                double height = startPosition3D.z();
                QVector3D worldCoordinates;
                widget->fromScreenToWorld(worldCoordinates, event, true, height);
                QVector2D radius = QVector2D(worldCoordinates - startPosition3D) / double(2);
                QVector3D center = startPosition3D + radius;
                vertex[vertexSize - 1].setPosition(center);
                QMatrix4x4 scaleAndTranslate;
                scaleAndTranslate.setToIdentity();
                scaleAndTranslate.translate(center);
                if(circle)
                {
                    double length = radius.length();
                    scaleAndTranslate.scale(length, length, 1);
                }
                else scaleAndTranslate.scale(radius.x(), radius.y(), 1);

                double angle = 360 / double(segments);
                if(!circle) angle *= sign(radius.x() * radius.y());
                normal = { 0, 0, 1 };
                createCap({ 1.f, 0.f, 0.f, 1.f }, angle, scaleAndTranslate);
            }
            else
            {
                QVector2D currentPosition = QVector2D(event->x() -
                    widget->getHalfWidth(), widget->getHalfHeight() - event->y());
                QVector3D worldCoordinates;

                widget->_fromScreenToWorld(worldCoordinates, QVector4D(currentPosition, 0, 1));
                QVector3D radius = (worldCoordinates - startPosition3D) /
                        double(2);
                QVector3D center = startPosition3D + radius;
                vertex[vertexSize - 1].setPosition(center);

                normal = createNormal(widget->getCamera().rotation());
                QMatrix4x4 scaleAndTranslate;
                scaleAndTranslate.setToIdentity();
                scaleAndTranslate.translate(center);
                if(circle)
                {
                    double length = radius.length();
                    scaleAndTranslate.scale(length, length, length);
                }
                else
                {
                    for(i = 0; i < 3; i++) if(qAbs(normal[i]) > 0.01) radius[i] =
                            sign(radius[i]);
                    scaleAndTranslate.scale(radius.x(), radius.y(), radius.z());
                }
                double angle = 360 / double(segments) * sign(radius.x() *
                                                         radius.y() * radius.z());
                if(projection == TOP || projection == BOTTOM) angle *= -1;
                bool front = projection == FRONT || projection == BACK;
                createCap({ !front, front, 0.f, 1.f }, angle, scaleAndTranslate);
            }
        }
        break;
    }
    case STOP:
    {
        //if ellipse is a line
        if(QVector3D::crossProduct(vertex[vertexSize - 1].getPosition(),
                        vertex[vertexSize - 2].getPosition()).length() == 0)
        {
            //remove cap
            vertex.resize(vertexSize - segments - 1);
            triangle.resize(triangle.size() - segments);
            ellipseFailed = true;
            return;
        }

        for(int i = 1; i < segments + 2; i++) vertex[vertexSize - i].select();
    }
    }
}

void TEllipse::allocateCap(bool flip)
{
    //(flip == true) - ellipse's normal is directed down (in perspective)
    //    (needed for cylinder's lower cap

    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int segments = spinBoxSegments->value();
    int i;

    //resize vectors and set triangle indices for first cap :
    //resize
    vertex.resize(vertexSize + segments + 1);
    triangle.resize(triangleSize + segments);
    //set indices
    for(i = 0; i < segments - 1; i++) triangle[triangleSize + i].setIndices(
        vertexSize + i + flip, vertexSize + i + !flip, vertexSize + segments);
    triangle[triangleSize + i].setIndices(vertexSize + (segments - 1) * !flip,
                   vertexSize + (segments - 1) * flip, vertexSize + segments);
}

QVector3D TEllipse::createNormal(const QVector3D &camRot)
{
    return ((*_activeWidget)->getProjection() == PERSPECTIVE) ? QVector3D(0,
        0, 1) : QVector3D(cosR(camRot.x()) * cosR(camRot.z()), cosR(camRot.x()) * sinR(camRot.z()), sinR(camRot.x()));
}

void TEllipse::createCap(QVector4D rotatingVertex, double angle, const QMatrix4x4 &scaleAndTranslate)
{
    vector <Vertex> &vertex = model->getVertex();
    int vertexSize = vertex.size();
    int segments = spinBoxSegments->value();

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(angle, normal);
    for(int i = 0; i < segments; i++)
    {
        rotatingVertex = rotation * rotatingVertex;
        vertex[vertexSize - segments - 1 + i].setPosition(QVector3D(
                                        scaleAndTranslate * rotatingVertex));
    }
}

