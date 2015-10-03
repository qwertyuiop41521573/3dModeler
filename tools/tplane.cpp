#include "tplane.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"
#include "gui/mylabel.h"
#include "gui/myframe.h"

TPlane::TPlane(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Plane");
    checkBoxSquare = new MyCheckBoxMW;
    checkBoxSquare->setText("Square");
    layout->addWidget(checkBoxSquare, 0, 0, 1, 2);
}

void TPlane::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int i;

    if(action == START)
    {
        //create the cap without other parts
        vertex.resize(vertexSize + 4);
        triangle.resize(triangleSize + 2);
        triangle[triangleSize    ].setIndices(vertexSize, vertexSize + 1,
                                              vertexSize + 2);
        triangle[triangleSize + 1].setIndices(vertexSize, vertexSize + 2,
                                              vertexSize + 3);

        QVector3D worldCoordinates = fromScreenToWorld(event, widget);
        //all cap vertices are in 1 point
        for(i = 0; i < 4; i++)
        {
            vertex[vertexSize + i] = worldCoordinates;
            vertex[vertexSize + i].setNewSelected(true);
        }
    }
    if(action == EXECUTE && (_hasStage2 ? _stage2 == false : true))
    {
        //drawing the cap
        QVector2D diagonal;
        //with posA and posB we track if we need to flip cap
        QVector3D posA, posB;
        //drawing square if "square" or "cube" options set
        bool square = checkBoxSquare->isChecked();
        Projection projection = widget->getProjection();
        if(projection == PERSPECTIVE)
        {

            //height can be 0 or 1 (which point will be returned by
            //    fromScreenToWorld(event, widget) at action == START,
            //    depends on camera angle
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
    if( action == STOP )
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
        for(i = 1; i < 5; i++)
        {
            vertex[vertexSize - i].setNewSelected(false);
            vertex[vertexSize - i].setSelected(true);
        }
    }
}

void TPlane::countDiagonalForSquare(QVector2D *diagonal)
{
    //square diagonal length is equal to length of projection (of
    //    line we draw) to this diagonal
    QVector2D squareDiagonal(sign(diagonal->x()), sign(diagonal->y()));
    double length = QVector2D::dotProduct(*diagonal, squareDiagonal) /
            double(2);
    *diagonal = squareDiagonal * length;
}
