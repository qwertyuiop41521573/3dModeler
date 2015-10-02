#include "tbox.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TBox::TBox(MainWindow *mainWindow) : TPlane(mainWindow)
{
    button->setText("Box");
    finalButton->setText("Create Box");

    checkBoxCube = new QCheckBox;
    checkBoxCube->setText("Cube");
    layout->addWidget(checkBoxCube, 0, 2, 1, 2);

    _hasStage2 = true;
    connect(checkBoxCube, SIGNAL(clicked(bool)), this, SLOT(h(bool)));
}

void TBox::function(Action action, QMouseEvent *event)
{
    //in DRAW section, tool draws some lines in viewport, ex. rectangle
    //    for tool select
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int i;

    //if "Create Box" was pressed
    if(action == FINAL)
    {
        for(i = 0; i < 3; i++) if(spinBox[i + 3]->value() <= 0) return;

        vertex.resize(vertexSize + 8);
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
            vertex[vertexSize + i].setPosition(v);
            vertex[vertexSize + i].setSelected(true);
        }

        triangle.resize( triangleSize + 12 );
        //triangles of the box:
        triangle[triangleSize     ].setIndices(vertexSize,     vertexSize + 1,
                                               vertexSize + 2);
        triangle[triangleSize + 1 ].setIndices(vertexSize,     vertexSize + 2,
                                               vertexSize + 3);
        triangle[triangleSize + 2 ].setIndices(vertexSize,     vertexSize + 5,
                                               vertexSize + 1);
        triangle[triangleSize + 3 ].setIndices(vertexSize,     vertexSize + 4,
                                               vertexSize + 5);
        triangle[triangleSize + 4 ].setIndices(vertexSize + 1, vertexSize + 6,
                                               vertexSize + 2);
        triangle[triangleSize + 5 ].setIndices(vertexSize + 1, vertexSize + 5,
                                               vertexSize + 6);
        triangle[triangleSize + 6 ].setIndices(vertexSize + 2, vertexSize + 7,
                                               vertexSize + 3);
        triangle[triangleSize + 7 ].setIndices(vertexSize + 2, vertexSize + 6,
                                               vertexSize + 7);
        triangle[triangleSize + 8 ].setIndices(vertexSize + 3, vertexSize + 4,
                                               vertexSize    );
        triangle[triangleSize + 9 ].setIndices(vertexSize + 3, vertexSize + 7,
                                               vertexSize + 4);
        triangle[triangleSize + 10].setIndices(vertexSize + 4, vertexSize + 6,
                                               vertexSize + 5);
        triangle[triangleSize + 11].setIndices(vertexSize + 4, vertexSize + 7,
                                               vertexSize + 6);
    }
    //if we clicked the viewport
    if(action == START) TPlane::function(action, event);
    //if we drag pressed mouse in viewport
    if(action == EXECUTE)
    {
        Projection projection = widget->getProjection();
        //if cap is done
        if(_stage2)
        {
            double dy = (widget->getHalfHeight() - event->y() - widget->
                         getLastPosition().y()) / double(100);
            Camera *camera = widget->getCamera();
            QVector3D normal = (projection == PERSPECTIVE) ?
                        QVector3D(0, 0, 1) : QVector3D(
                            cos(inRadians(camera->rotation().x())) *
                            cos(inRadians(camera->rotation().z())),
                            cos(inRadians(camera->rotation().x())) *
                            sin(inRadians(camera->rotation().z())),
                            sin(inRadians(camera->rotation().x())));

            //if cube
            if(checkBoxCube->isChecked() && dy != 0)
            {
                QVector3D dh = normal * sign(dy) * (vertex[vertexSize - 2].
                        getPosition() - vertex[vertexSize - 4].getPosition()).
                        length() / qSqrt(2);
                for(i = -4; i < 0; i++) vertex[vertexSize + i].setPosition(
                            vertex[vertexSize - 4 + i].getPosition() + dh);
            }
            else for(i = -4; i < 0; i++) vertex[vertexSize + i].addToPosition(
                        normal * dy);

            QVector3D diagonal = vertex[vertexSize - 2].getPosition() -
                    vertex[vertexSize - 4].getPosition();
            QVector3D e_x = vertex[vertexSize - 1].getPosition() - vertex[
                    vertexSize - 4].getPosition();

            //flip the box if needed
            if(QVector3D::dotProduct(normal, vertex[vertexSize - 8].
                getPosition() - vertex[vertexSize - 4].getPosition()) *
                QVector3D::dotProduct(normal, QVector3D::crossProduct(e_x,
                                                                diagonal)) > 0)
            {
                    QVector3D temp = vertex[vertexSize - 7].getPosition();
                    vertex[vertexSize - 7] = vertex[vertexSize - 5];
                    vertex[vertexSize - 5].setPosition(temp);
                    temp = vertex[vertexSize - 3].getPosition();
                    vertex[vertexSize - 3] = vertex[vertexSize - 1];
                    vertex[vertexSize - 1].setPosition(temp);
            }
        }
        else
        {
            //drawing the cap
            QVector2D diagonal;
            //with posA and posB we track if we need to flip cap
            QVector3D posA, posB;
            //drawing square if "square" or "cube" options set
            bool square = checkBoxSquare->isChecked() || checkBoxCube->
                    isChecked();
            if(projection == PERSPECTIVE)
            {
                //height can be 0 or 1 (which point will be returned by
                //    fromScreenToWorld(event, widget) at action == START, depends
                //    on camera angle
                double height = vertex[vertexSize - 4].getPosition().z();
                QVector3D worldCoordinates = fromScreenToWorld(event, widget,
                                                               true, height);
                vertex[vertexSize - 2].setPosition(worldCoordinates);
                diagonal = QVector2D(vertex[vertexSize - 2].getPosition() -
                        vertex[vertexSize - 4].getPosition());
                if(square)
                {
                    countDiagonalForSquare(&diagonal);
                    vertex[vertexSize - 2].setPosition(vertex[vertexSize - 4].
                            getPosition() + QVector3D(diagonal, 0));
                }
                posB = QVector3D(vertex[vertexSize - 4].getPosition().x(), vertex[
                        vertexSize - 2].getPosition().y(), height);
                posA = QVector3D(vertex[vertexSize - 2].getPosition().x(), vertex[
                        vertexSize - 4].getPosition().y(), height);
            }
            else
            {
                widget->countFinalInverseMatrix(false);
                QVector2D startPosition = QVector2D(widget->getFinalMatrix() *
                           QVector4D(vertex[vertexSize - 4].getPosition(), 1));
                QVector2D currentPosition = QVector2D(event->x() - widget->
                           getHalfWidth(), widget->getHalfHeight() - event->y());
                diagonal = currentPosition - startPosition;
                if(square)
                {
                    countDiagonalForSquare(&diagonal);
                    currentPosition = startPosition + diagonal;
                }
                vertex[vertexSize - 2].setPosition(fromScreenToWorld_vector(
                                                       currentPosition, widget));
                posB = fromScreenToWorld_xy(startPosition.x(),
                                            currentPosition.y(), widget);
                posA = fromScreenToWorld_xy(currentPosition.x(),
                                            startPosition.y(), widget);
            }
            //flipping the cap (or not)
            int a = (diagonal.x() * diagonal.y() > 0) ? 1 : 3;
            vertex[vertexSize - a].setPosition(posA);
            vertex[vertexSize - 4 + a].setPosition(posB);
        }
    }
    //last click in viewport for this tool
    if(action == STOP)
    {
        vertexSize = vertex.size();
        //if height == 0
        if(vertex[vertexSize - 1] == vertex[vertexSize - 5])
        {
            //remove box - last 8 vertices and 12 triangles
            vertex.resize(vertexSize - 8);
            triangle.resize(triangle.size() - 12);
        }
        else
        {
            for(i = 1; i < 9; i++)
            {
                //deselect with "blue", select with "red"
                vertex[vertexSize - i].setNewSelected(false);
                vertex[vertexSize - i].setSelected(true);
            }
        }
        widget->setToolIsOn(false);
        setStage2(false);
        widget->setMouseTracking(false);
    }
    //"stage1" was dragging pressed mouse (and drawing the cap of box), stage2
    //    is moving released mouse in vertical direction (changing the height
    //    of box);    STAGE2 is ran once, like START or STOP, EXECUTE is ran
    //    every time GLWidget recieves mouse move event
    if(action == STAGE2)
    {
        //if cap's height or width == 0
        if(vertex[vertexSize - 1] == vertex[vertexSize - 2] || vertex[
                vertexSize - 3] == vertex[vertexSize - 2] )
        {
            //remove cap - last 4 vertices and 2 triangles
            vertex.resize(vertexSize - 4);
            triangle.resize(triangle.size() - 2);
            return;
        }
        widget->setToolIsOn(true);
        setStage2(true);

        //adding elements to create box from it's cap (4 vertices and 10
        //    triangles)
        vertex.resize(vertexSize + 4);
        triangle.resize(triangleSize + 10);

        triangle[triangleSize    ].setIndices(vertexSize - 4, vertexSize + 1,
                                              vertexSize - 3);
        triangle[triangleSize + 1].setIndices(vertexSize - 4, vertexSize,
                                              vertexSize + 1);
        triangle[triangleSize + 2].setIndices(vertexSize - 3, vertexSize + 2,
                                              vertexSize - 2);
        triangle[triangleSize + 3].setIndices(vertexSize - 3, vertexSize + 1,
                                              vertexSize + 2);
        triangle[triangleSize + 4].setIndices(vertexSize - 2, vertexSize + 3,
                                              vertexSize - 1);
        triangle[triangleSize + 5].setIndices(vertexSize - 2, vertexSize + 2,
                                              vertexSize + 3);
        triangle[triangleSize + 6].setIndices(vertexSize - 1, vertexSize,
                                              vertexSize - 4);
        triangle[triangleSize + 7].setIndices(vertexSize - 1, vertexSize + 3,
                                              vertexSize    );
        triangle[triangleSize + 8].setIndices(vertexSize, vertexSize + 2,
                                              vertexSize + 1);
        triangle[triangleSize + 9].setIndices(vertexSize, vertexSize + 3,
                                              vertexSize + 2);


        for(i = 0; i < 4; i++)
        {
            //the second cap is the same as first
            vertex[vertexSize + i] = vertex[vertexSize - 4 + i].getPosition();
            vertex[vertexSize + i].setNewSelected(true);
        }
        widget->setMouseTracking( true );
    }
}

void TBox::countDiagonalForSquare(QVector2D *diagonal)
{
    //square diagonal length is equal to length of projection (of
    //    line we draw) to this diagonal
    QVector2D squareDiagonal(sign(diagonal->x()), sign(diagonal->y()));
    double length = QVector2D::dotProduct(*diagonal, squareDiagonal) / double(2);
    *diagonal = squareDiagonal * length;
}
