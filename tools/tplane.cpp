#include "tplane.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"

TPlane::TPlane(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Plane");
    checkBoxSquare = new MyCheckBoxMW;
    checkBoxSquare->setText("Square");
    layout->addWidget(checkBoxSquare, 0, 0, 1, 4);
    _widget->hide();
}

void TPlane::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int i;
    widget->countFinalInverseMatrix();

    switch(action)
    {
    case START:
    {
        //create the cap without other parts
        allocateCap(_hasStage2);

        QVector3D worldCoordinates;
        widget->fromScreenToWorld(&worldCoordinates, event);
        //all cap vertices are in 1 point
        for(int i = 0; i < 4; i++)
        {
            vertex[vertexSize + i] = worldCoordinates;
            vertex[vertexSize + i].setNewSelected(true);
        }
        break;
    }
    case EXECUTE:
    {
        if(_hasStage2 ? _stage2 == false : true)
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
                QVector3D worldCoordinates;
                widget->fromScreenToWorld(&worldCoordinates, event, true, height);
                vertex[vertexSize - 2].setPosition(worldCoordinates);
                diagonal = QVector2D(vertex[vertexSize - 2].getPosition() - vertex[vertexSize - 4].getPosition());
                if(square)
                {
                    countDiagonalForSquare(&diagonal);
                    vertex[vertexSize - 2].setPosition(vertex[vertexSize - 4].getPosition() + QVector3D(diagonal, 0));
                }
                posB = QVector3D(vertex[vertexSize - 4].getPosition().x(), vertex[vertexSize - 2].getPosition().y(), height);
                posA = QVector3D(vertex[vertexSize - 2].getPosition().x(), vertex[vertexSize - 4].getPosition().y(), height);
            }
            else
            {
                QVector2D startPosition = QVector2D(widget->getFinalMatrix() * QVector4D(vertex[vertexSize - 4].getPosition(), 1));
                QVector2D currentPosition = QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
                diagonal = currentPosition - startPosition;
                if(square)
                {
                    countDiagonalForSquare(&diagonal);
                    currentPosition = startPosition + diagonal;
                }
                widget->_fromScreenToWorld(&vertex[vertexSize - 2].getEditablePosition(), QVector4D(currentPosition, 0, 1));
                widget->_fromScreenToWorld(&posB, QVector4D(startPosition.x(), currentPosition.y(), 0, 1));
                widget->_fromScreenToWorld(&posA, QVector4D(currentPosition.x(), startPosition.y(), 0, 1));
            }
            //flipping the cap (or not)
            int a = (diagonal.x() * diagonal.y() > 0) ? 1 : 3;
            vertex[vertexSize - a].setPosition(posA);
            vertex[vertexSize - 4 + a].setPosition(posB);
        }
        break;
    }
    case STOP:
    {
        //if plane's height or width == 0
        if(vertex[vertexSize - 1] == vertex[vertexSize - 2] || vertex[vertexSize - 3] == vertex[vertexSize - 2])
        {
            //remove cap - last 4 vertices and 2 triangles
            vertex.resize(vertexSize - 4);
            triangle.erase(triangle.end() - 2, triangle.end());
            planeFailed = true;
            return;
        }
        for(i = 1; i < 5; i++) vertex[vertexSize - i].setSelected(true, false);
    }
    }
}

void TPlane::countDiagonalForSquare(QVector2D *diagonal)
{
    //square diagonal length is equal to length of projection (of
    //    line we draw) to this diagonal
    QVector2D squareDiagonal(sign(diagonal->x()), sign(diagonal->y()));
    double length = QVector2D::dotProduct(*diagonal, squareDiagonal) / double(2);
    *diagonal = squareDiagonal * length;
}

void TPlane::allocateCap(bool flip)
{
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();

    vertex.resize(vertexSize + 4);
    triangle.push_back({vertexSize + !flip, vertexSize + flip, vertexSize + 2});
    triangle.push_back({vertexSize + 2 * !flip, vertexSize + 2 * flip, vertexSize + 3});
}
