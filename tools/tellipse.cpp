#include "tellipse.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TEllipse::TEllipse(MainWindow *mainWindow) : ToolWithWidget(
                                                 mainWindow)
{
    button->setText("Ellipse");
    finalButton = new QPushButton("Create Ellipse");

    int i;
    for(i = 0; i < 8; i++) spinBox[i] = new MySpinBox;
    spinBox[0]->setMinimum(3);
    spinBox[0]->setValue(18);

    MyLabel *segments = new MyLabel("Segments:", 70);

    checkBox = new MyCheckBoxMW;
    checkBox->setText("Circle");

    QLabel *center = new QLabel("Center");
    QLabel *normalLabel = new QLabel("Normal");
    MyLabel *label[6];
    for(int i = 0; i < 6; i++) label[i] = new MyLabel(QString( 'X'
                                              + i % 3 ) + ':', 25);
    MyLabel *radius = new MyLabel("Radius:", 70);

    spinBox[7]->setMinimum(0);
    spinBox[7]->setValue(1);

    layout->addWidget(segments, 0, 0, 1, 2);
    layout->addWidget(spinBox[0], 0, 2, 1, 2);
    layout->addWidget(checkBox, 1, 0, 1, 4);
    layout->addWidget(center, 2, 0, 1, 4);
    layout->addWidget(normalLabel, 2, 2, 1, 4);
    for(int i = 0; i < 3; i++)
    {
        layout->addWidget(label[i], 3 + i, 0);
        layout->addWidget(spinBox[1 + i], 3 + i, 1);
        layout->addWidget(label[i + 3], 3 + i, 2);
        layout->addWidget(spinBox[4 + i], 3 + i, 3);
    }
    layout->addWidget(radius, 6, 0, 1, 2);
    layout->addWidget(spinBox[7], 6, 2, 1, 2);
    layout->addWidget(finalButton, 7, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(
                final()));
}

void TEllipse::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;
    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int segments = spinBox[0]->value();
    int i;

    //START and FINAL have much common code
    if(action == START || action == FINAL)
    {
        //argument should be true for cylinder and false for ellipse, so we can
        //    use _hasStage2 and do not create new variable
        createCap(_hasStage2);

        if(action == START)
        {
            QVector3D worldCoordinates = fromScreenToWorld(event, widget);
            widget->setStartPosition3D(worldCoordinates);
            for(i = 0; i <= segments; i++)
            {
                vertex[vertexSize + i] = worldCoordinates;
                vertex[vertexSize + i].setNewSelected(true);
            }
        }
        else
        {
            for(i = 0; i < 3; i++) normal[i] = spinBox[4 + i]->value();
            if(normal.length() == 0) return;
            normal.normalize();
            double radius = spinBox[7]->value();
            if(radius == 0) return;
            QVector3D center;
            for(i = 0; i < 3; i++) center[i] = spinBox[1 + i]->value();

            widget->countFinalInverseMatrix(false);
            //center of first cap
            vertex[vertexSize + segments].setPosition(center);

            //cap is created with rotating "QVector4D rotatingVertex" around
            //    center with "QMatrix4x4 rotation" (with every iteration, 1
            //    segment is passed); then "rotation" is copied to "QVector3D
            //    currentVertex", and it's scaled and moved with "QMatrix4x4
            //    scaleAndTranslate"
            double angle = 360 / double(segments);
            QMatrix4x4 rotation;
            rotation.setToIdentity();
            rotation.rotate(angle, normal);
            //if normal is parallel to X axis, than e_y; else [e_x, normal]
            QVector4D rotatingVertex = (normal.x() == 1) ? QVector4D(0, 1, 0, 1) :
                QVector4D(QVector3D::crossProduct(QVector3D(1, 0, 0), normal).
                          normalized(), 1);

            QMatrix4x4 scaleAndTranslate = createScaleAndTranslate(radius,
                                                        radius, radius, center);
            QVector3D currentVertex;
            for(i = 0; i < segments; i++)
            {
                rotatingVertex = rotation * rotatingVertex;
                currentVertex = QVector3D(scaleAndTranslate * rotatingVertex);
                //first cap
                vertex[vertexSize + i].setPosition(currentVertex);
            }

            for(i = 0; i <= segments; i++) vertex[vertexSize + i].setSelected(
                        true);
        }
    }
    //for cylinder, this is "if(!_stage2)", so it should be done if:
    //    _hasStage2 == false || (_hasStage2 = true && _stage2 == false)
    //    (with _hasStage2 we define if this code is executed by ellipse (false)
    //    or cylinder (true)
    if(action == EXECUTE && (_hasStage2 ? _stage2 == false : true))
    {
        Projection projection = widget->getProjection();
        bool circle = checkBox->isChecked();
        if(projection == PERSPECTIVE)
        {
            QVector3D start = widget->startPosition3D();
            double height = start.z();
            QVector3D worldCoordinates = fromScreenToWorld(event, widget,
                                                           true, height);
            QVector2D radius = QVector2D(worldCoordinates - start) /
                    double(2);
            QVector3D center = start + radius;
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
            createEllipseCap({ 1.f, 0.f, 0.f, 1.f }, angle, { 0, 0, 1 },
                             model, vertexSize, segments, scaleAndTranslate);
        }
        else
        {
            widget->countFinalInverseMatrix(false);
            QVector3D start = widget->startPosition3D();
            QVector2D currentPosition = QVector2D(event->x() -
                widget->getHalfWidth(), widget->getHalfHeight() - event->y());
            QVector3D worldCoordinates = fromScreenToWorld_vector(
                        currentPosition, widget);
            QVector3D radius = (worldCoordinates - start) / double(2);
            QVector3D center = start + radius;
            vertex[vertexSize - 1].setPosition(center);

            QVector3D normal = createNormal(widget->getCamera()->rotation());
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
            createEllipseCap({ !front, front, 0.f, 1.f }, angle, normal,
                             model, vertexSize, segments, scaleAndTranslate);
        }
    }
    if(action == STOP)
    {
        //if ellipse is a line
        if(QVector3D::crossProduct(vertex[vertexSize - 1].getPosition(), vertex[
                                   vertexSize - 2].getPosition()).length() == 0)
        {
            //remove cap
            vertex.resize(vertexSize - segments - 1);
            triangle.resize(triangle.size() - segments);
            return;
        }

        for(int i = 1; i < segments + 2; i++)
        {
            vertex[vertexSize - i].setNewSelected(false);
            vertex[vertexSize - i].setSelected(true);
        }
    }
}

void TEllipse::createCap(bool flip)
{
    //(flip == true) - ellipse's normal is directed down (in perspective)
    //    (needed for cylinder's lower cap

    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int segments = spinBox[0]->value();
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

QMatrix4x4 TEllipse::createScaleAndTranslate(double scaleX, double scaleY, double
                                           scaleZ, QVector3D center)
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(center);
    m.scale(scaleX, scaleY, scaleZ);
    return m;
}

QVector3D TEllipse::createNormal(QVector3D camRot)
{
    return ((*_activeWidget)->getProjection() == PERSPECTIVE) ? QVector3D(0, 0,
        1 ) : QVector3D(cos(inRadians(camRot.x())) * cos(inRadians(camRot.z())),
                        cos(inRadians(camRot.x())) * sin(inRadians(camRot.z())),
                        sin(inRadians(camRot.x())));
}
