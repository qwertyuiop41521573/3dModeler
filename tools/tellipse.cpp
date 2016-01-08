#include "tellipse.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"
#include "journal.h"

#include "gui/mylabel.h"


#include <iostream>

using namespace std;
using namespace Model;
using namespace Journal;

TEllipse::TEllipse() : CreatingTool()
{
    button->setText("Ellipse");
    finalButton = new QPushButton("Create Ellipse");

    int i;
    spinBoxSegments = new QSpinBox;
    spinBoxSegments->setMaximumWidth(55);
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
    for(i = 0; i < 6; i++) label[i] = new MyLabel(QString('X' + i % 3 ) + ':', 25);
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
    //final button y coordinate is 8, not 7 because we leave space for cyllinder's spinBox
    layout->addWidget(finalButton, 8, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _widget->hide();

    _shift = checkBoxCircle;
}

void TEllipse::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = Workspace::activeWidget();
    int segments = spinBoxSegments->value();
    int i;

    //argument should be true for cylinder and false for ellipse, so we
    //    can use _hasStage2 and do not create new variable
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
        for(i = 0; i <= segments; i++)
        {
            ver.push_back(vertex().push(startPosition3D));
            vertex()[ver[i]].setNewSelected(true);
        }
        triangulateCap(_hasStage2);
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

        normal.normalize();
        QVector3D center;
        for(i = 0; i < 3; i++) center[i] = spinBox[i]->value();

        //cap is created with rotating "QVector4D rotatingVertex" around
        //    center with "QMatrix4x4 rotation" (with every iteration, 1
        //    segment is passed); then "rotation" is copied to "QVector3D
        //    currentVertex", and it's scaled and moved with "QMatrix4x4
        //    scaleAndTranslate"

        //if normal is parallel to X axis, than e_y; else [e_x, normal]
        QVector4D rotatingVertex = (qAbs(normal.x()) == 1) ? QVector4D(0, 1, 0, 1) : QVector4D(QVector3D::crossProduct(QVector3D(1, 0, 0), normal).normalized(), 1);
        double angle = 360 / double(segments);
        QMatrix4x4 scaleAndTranslate;
        scaleAndTranslate.setToIdentity();
        scaleAndTranslate.translate(center);
        scaleAndTranslate.scale(radius, radius, radius);

        ver.clear();
        tri.clear();
        Journal::newRecord(CREATE);
        for(i = 0; i < segments; i++) ver.push_back(vertex().push({{0, 0, 0}}));
        createCap(rotatingVertex, angle, scaleAndTranslate);
        //center of first cap
        ver.push_back(vertex().push(center));

        triangulateCap(_hasStage2);

        for(i = 0; i <= segments; i++) vertex()[ver[i]].setSelected(true);
        if(!_hasStage2) Journal::submit();
        break;
    }

    //for cylinder, this is "if(!_stage2)", so it should be done if:
    //    _hasStage2 == false || (_hasStage2 = true && _stage2 == false)
    //    (with _hasStage2 we define if this code is executed by ellipse
    //    (false) or cylinder (true)
    case EXECUTE:
    {
        if(_hasStage2 ? _stage2 == true : false) break;
        Projection projection = widget->getProjection();
        bool circle = checkBoxCircle->isChecked();
        widget->countFinalInverseMatrix();
        if(projection == PERSPECTIVE)
        {
            double height = startPosition3D.z();
            QVector3D worldCoordinates;
            widget->fromScreenToWorld(&worldCoordinates, event, true, height);
            QVector2D radius = QVector2D(worldCoordinates - startPosition3D) / double(2);
            QVector3D center = startPosition3D + radius;
            vertex()[ver[segments]].setPosition(center);
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate(center);
            if(circle) scaleAndTranslate.scale(radius.length(), radius.length(), 1);
            else scaleAndTranslate.scale(radius.x(), radius.y(), 1);

            double angle = 360 / double(segments);
            if(!circle) angle *= sign(radius.x() * radius.y());
            normal = { 0, 0, 1 };
            createCap({ 1.f, 0.f, 0.f, 1.f }, angle, scaleAndTranslate);
        }
        else
        {
            QVector2D currentPosition = QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
            QVector3D worldCoordinates;

            widget->_fromScreenToWorld(&worldCoordinates, QVector4D(currentPosition, 0, 1));
            QVector3D radius = (worldCoordinates - startPosition3D) / double(2);
            QVector3D center = startPosition3D + radius;
            vertex()[ver[segments]].setPosition(center);

            const QVector3D &camRot = widget->getCamera().rotation();
            normal = QVector3D(cosR(camRot.x()) * cosR(camRot.z()), cosR(camRot.x()) * sinR(camRot.z()), sinR(camRot.x()));
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate(center);
            if(circle) scaleAndTranslate.scale(radius.length(), radius.length(), radius.length());
            else
            {
                for(i = 0; i < 3; i++) if(qAbs(normal[i]) > 0.01) radius[i] = sign(radius[i]);
                scaleAndTranslate.scale(radius.x(), radius.y(), radius.z());
            }
            double angle = 360 / double(segments) * sign(radius.x() * radius.y() * radius.z());
            if(projection == TOP || projection == BOTTOM) angle *= -1;
            bool front = projection == FRONT || projection == BACK;
            createCap({ !front, front, 0.f, 1.f }, angle, scaleAndTranslate);
        }
        break;
    }
    case STOP:
    {
        //if ellipse is a line
        QVector3D v1 = vertex()[ver[segments - 1]].position() - vertex()[ver[segments]].position();
        QVector3D v2 = vertex()[ver[segments - 2]].position() - vertex()[ver[segments]].position();
        if(QVector3D::crossProduct(v1, v2).length() == 0)
        {
            //remove cap
            removeAll();
            ellipseFailed = true;
            _busy = false;
            return;
        }

        for(i = 0; i <= segments; i++) vertex()[ver[i]].setSelected(true, false);

        if(_hasStage2) break;
        _busy = false;
        Journal::submit();
    }
    }
}

void TEllipse::triangulateCap(bool flip)
{
    //(flip == true) - ellipse's normal is directed down (in perspective)
    //    (needed for cylinder's lower cap

    int segments = spinBoxSegments->value();
    int i;

    //set triangle indices for first cap
    for(i = 0; i < segments; i++) tri.push_back(triangle().push({ver[(i + flip) % segments], ver[(i + !flip) % segments], ver[segments]}));
}

void TEllipse::createCap(QVector4D rotatingVertex, double angle, const QMatrix4x4 &scaleAndTranslate)
{
    int segments = spinBoxSegments->value();

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(angle, normal);
    for(int i = 0; i < segments; i++)
    {
        rotatingVertex = rotation * rotatingVertex;
        vertex()[ver[i]].setPosition(QVector3D(scaleAndTranslate * rotatingVertex));
    }
}

