#include "tselect.h"
#include "glwidget.h"
#include "mainwindow.h"

TSelect::TSelect(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Select");
    int i;
    for(i = 0; i < 2; i++) checkBox[i] = new MyCheckBox;
    checkBox[0]->setText("Add");
    checkBox[1]->setText("Remove");
    for(i = 0; i < 2; i++)
    {
        checkBox[i]->setNeighbor(checkBox[!i]);
        layout->addWidget(checkBox[i], 0, i, 1, 2);
    }

    _widget->hide();
}

void TSelect::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;

    switch(action)
    {
    case START:
    {
        QVector2D min, max;
        QVector2D currentPosition(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
        countMinAndMax(&min, &max, currentPosition);

        _select(min, max);
        break;
    }
    case DRAW:
    {
        VertexAndIndexData &data = widget->getToolData();
        vector <VertexData_Color> &vertices = data.vertices;
        vector <GLuint> &indices = data.indices;
        vertices.resize(4);

        QVector2D min, max;
        countMinAndMax(&min, &max, widget->getCurrentPosition());

        vertices[0].position = min;
        vertices[1].position.setX(max.x());
        vertices[1].position.setY(min.y());
        vertices[2].position = max;
        vertices[3].position.setX(min.x());
        vertices[3].position.setY(max.y());

        indices.resize(8);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 1;
        indices[3] = 2;
        indices[4] = 2;
        indices[5] = 3;
        indices[6] = 3;
        indices[7] = 0;

        //exept drawing here we check is vertices are selected (inside rectangle)
        //this should be in EXECUTE, but here "min" and "max" are counted already
        _select(min, max);
        break;
    }
    case STOP:
    {
        vector <Vertex> &vertex = model->getVertex();
        int vertexSize = vertex.size();
        vector <Triangle> &triangle = model->getTriangle();
        int triangleSize = triangle.size();
        bool workWithVert = workWithElements[0]->isChecked();
        int i;

        vector <SelectableObject*> selObj;
        selObj.resize(workWithVert ? vertexSize : triangleSize);
        for(i = 0; i < selObj.size(); i++)
        {
            if(workWithVert) selObj[i] = &vertex[i];
            else selObj[i] = &triangle[i];
        }
        for(i = 0; i < selObj.size(); i++)
        {
            if(!checkBox[0]->isChecked() && !checkBox[1]->isChecked()) selObj[i]->setSelected(selObj[i]->newSelected(), false);
            if(checkBox[0]->isChecked()  && selObj[i]->newSelected())  selObj[i]->setSelected(true, false);
            if(checkBox[1]->isChecked()  && selObj[i]->newSelected())  selObj[i]->setSelected(false, false);
        }
        break;
    }
    }
}

void TSelect::countMinAndMax(QVector2D *min, QVector2D *max, const QVector2D &currentPosition)
{
    GLWidget *widget = *_activeWidget;
    const QVector2D &startPosition = widget->getStartPosition();

    min->setX(qMin(startPosition.x(), currentPosition.x()) - 2);
    min->setY(qMin(startPosition.y(), currentPosition.y()) - 2);
    max->setX(qMax(startPosition.x(), currentPosition.x()) + 2);
    max->setY(qMax(startPosition.y(), currentPosition.y()) + 2);
}

void TSelect::_select(const QVector2D &min, const QVector2D &max)
{
    GLWidget *widget = *_activeWidget;
    //work with vertices
    bool workWithVert = workWithElements[0]->isChecked();
    vector <Vertex> &vertex = model->getVertex();
    int vertexSize = vertex.size();
    vector <Triangle> &triangle = model->getTriangle();
    int triangleSize = triangle.size();
    int i;

    widget->countFinalMatrix();
    int j;
    if(workWithVert)
    {
        for(i = 0; i < vertexSize; i++) vertex[i].setNewSelected(widget->isSelected(vertex[i].getPosition(), min, max));
    }
    else
    {
        vector <QVector2D> vertexOnScreen(vertexSize);
        for(i = 0; i < vertexSize; i++) widget->fromWorldToScreen(&vertexOnScreen[i], vertex[i].getPosition());


        bool selected[vertexSize];
        for(i = 0; i < vertexSize; i++) selected[i] = widget->isSelected(vertex[i].getPosition(), min, max);

        int k;
        QVector2D rectanglePoints[4];
        for(i = 0; i < 4; i++) rectanglePoints[i] = QVector2D(i % 2 ? max.x() : min.x(), i / 2 ? max.y() : min.y());
        QVector2D edge;
        QVector2D fromVertexToCurrent;

        QVector2D minT, maxT;

        for(i = 0; i < triangleSize; i++)
        {
            triangle[i].setNewSelected(false);

            //check if triangle and rectangle can have common points
            minT = maxT = vertexOnScreen[triangle[i].getIndex(0)];
            for(j = 1; j < 3; j++)
            {
                const QVector2D &v = vertexOnScreen[triangle[i].getIndex(j)];
                if(v.x() < minT.x()) minT.setX(v.x());
                if(v.y() < minT.y()) minT.setY(v.y());
                if(v.x() > maxT.x()) maxT.setX(v.x());
                if(v.y() > maxT.y()) maxT.setY(v.y());
            }
            if(min.x() > maxT.x() || minT.x() > max.x() || min.y() > maxT.y() || minT.y() > max.y()) continue;

            //check if some vertex of triangle is inside rectangle
            for(j = 0; j < 3; j++)
            {
                if(selected[triangle[i].getIndex(j)])
                {
                    triangle[i].setNewSelected(true);
                    break;
                }
            }
            if(triangle[i].newSelected()) continue;

            //check if some point of rectangle (like vertex) is inside triangle
            for(j = 0; j < 4; j++)
            {
                for(k = 0; k < 3; k++)
                {
                    edge = vertexOnScreen[triangle[i].getIndex((k + 1) % 3)] - vertexOnScreen[triangle[i].getIndex(k)];
                    fromVertexToCurrent = rectanglePoints[j] - vertexOnScreen[triangle[i].getIndex(k)];
                    if(QVector3D::crossProduct(edge, fromVertexToCurrent).z() <= 0) break;
                }
                if(k == 3) triangle[i].setNewSelected(true);
            }
            if(triangle[i].newSelected()) continue;

            //check if edges of triangle and rectangle intersect
            //loop through triangle edges
            for(j = 0; j < 3; j++)
            {
                // edge - j, (j + 1) % 3
                minT = maxT = vertexOnScreen[triangle[i].getIndex(j)];
                const QVector2D &v = vertexOnScreen[triangle[i].getIndex((j + 1) % 3)];
                if(v.x() < minT.x()) minT.setX(v.x());
                if(v.y() < minT.y()) minT.setY(v.y());
                if(v.x() > maxT.x()) maxT.setX(v.x());
                if(v.y() > maxT.y()) maxT.setY(v.y());

                if(minT.x() > max.x() || maxT.x() < min.x() || minT.y() > max.y() || maxT.y() < min.y()) continue;

                if(
                           (minT.y() < rectanglePoints[0].y() && maxT.y() > rectanglePoints[0].y())
                    //    || (minT.y() < rectanglePoints[2].y() && maxT.y() > rectanglePoints[2].y())
                     //   || (minT.x() < rectanglePoints[0].x() && maxT.x() > rectanglePoints[0].x())
                      //  || (minT.x() < rectanglePoints[1].x() && maxT.x() > rectanglePoints[1].x())
                        )
                {
                    triangle[i].setNewSelected(true);
                    break;
                }
            }
        }
    }
}
