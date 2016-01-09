#include "tplane.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"
#include "model.h"
#include "journal.h"

using namespace Model;
using namespace Journal;

TPlane::TPlane() : CreatingTool()
{
    button->setText("Plane");
    checkBoxSquare = new MyCheckBoxMW;
    checkBoxSquare->setText("Square");
    layout->addWidget(checkBoxSquare, 0, 0, 1, 4);
    _widget->hide();

    _shift = checkBoxSquare;
}

void TPlane::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = Workspace::activeWidget();
    int i;
    widget->countFinalInverseMatrix();

    switch(action)
    {
    case START:
    {
        _busy = true;
        Journal::newRecord(CREATE);
        //create the cap without other parts    
        //all cap vertices are in 1 point

        QVector3D worldCoordinates;
        widget->fromScreenToWorld(&worldCoordinates, event);
        ver.clear();
        tri.clear();
        for(int i = 0; i < 4; i++)
        {
            ver.push_back(vertex().push(worldCoordinates));
            vertex()[ver[i]].setNewSelected(true);
        }
        bool flip = _hasStage2;
        tri.push_back(triangle().push({ver[!flip], ver[flip], ver[2]}));
        tri.push_back(triangle().push({ver[2 * !flip], ver[2 * flip], ver[3]}));
        break;
    }
    case EXECUTE:
    {
        if(_hasStage2 ? _stage2 == true : false) break;

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
            double height = vertex()[ver[0]].position().z();
            QVector3D worldCoordinates;
            widget->fromScreenToWorld(&worldCoordinates, event, true, height);
            vertex()[ver[2]].setPosition(worldCoordinates);
            diagonal = QVector2D(vertex()[ver[2]].position() - vertex()[ver[0]].position());
            if(square)
            {
                countDiagonalForSquare(&diagonal);
                vertex()[ver[2]].setPosition(vertex()[ver[0]].position() + QVector3D(diagonal, 0));
            }
            posB = QVector3D(vertex()[ver[0]].position().x(), vertex()[ver[2]].position().y(), height);
            posA = QVector3D(vertex()[ver[2]].position().x(), vertex()[ver[0]].position().y(), height);
        }
        else
        {
            QVector2D startPosition = QVector2D(widget->getFinalMatrix() * QVector4D(vertex()[ver[0]].position(), 1));
            QVector2D currentPosition = QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
            diagonal = currentPosition - startPosition;
            if(square)
            {
                countDiagonalForSquare(&diagonal);
                currentPosition = startPosition + diagonal;
            }
            widget->_fromScreenToWorld(&vertex()[ver[2]].position(), QVector4D(currentPosition, 0, 1));
            widget->_fromScreenToWorld(&posB, QVector4D(startPosition.x(), currentPosition.y(), 0, 1));
            widget->_fromScreenToWorld(&posA, QVector4D(currentPosition.x(), startPosition.y(), 0, 1));
        }
        //flipping the cap (or not)
        int a = (diagonal.x() * diagonal.y() > 0) ? 1 : 3;
        vertex()[ver[4 - a]].setPosition(posA);
        vertex()[ver[a]].setPosition(posB);
        break;
    }
    case STOP:
    {
        //if plane's height or width == 0
        if(vertex()[ver[3]] == vertex()[ver[2]] || vertex()[ver[1]] == vertex()[ver[2]])
        {
            //remove cap - last 4 vertices and 2 triangles
            removeAll();
            planeFailed = true;
            _busy = false;
            return;
        }
        for(i = 0; i < 4; i++) vertex()[ver[i]].setSelected(true, false);

        if(_hasStage2) break;
        _busy = false;
        Journal::submit();
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
