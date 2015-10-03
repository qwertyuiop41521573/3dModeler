#include "tselect.h"
#include "glwidget.h"
#include "functions.h"

#include "mainwindow.h"

TSelect::TSelect(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Select");
    int i;
    for(i = 0; i < 2; i++) checkBox[i] = new MyCheckBox;
    for(i = 0; i < 2; i++)
    {
        checkBox[i]->setNeighbor(checkBox[!i]);
        layout->addWidget(checkBox[i], 0, i);
    }
    checkBox[0]->setText("Add");
    checkBox[1]->setText("Remove");
}

void TSelect::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    Projection projection = widget->getProjection();
    if(action == START)
    {
        QVector2D min, max;
        QVector2D startPosition = widget->getStartPosition();
        QVector2D currentPosition(event->x() - widget->
             getHalfWidth(), widget->getHalfHeight() - event->y());

        min.setX(qMin(startPosition.x(), currentPosition.x()) - 1);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 1);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 1);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 1);

        bool perspective = projection == PERSPECTIVE;
        widget->countFinalMatrix(perspective);
        int j;
        if(workWithElements[0]->isChecked())
        {
            for(i = 0; i < vertexSize; i++)
                vertex[i].setNewSelected(isSelected(widget->
                    getFinalMatrix(), vertex[i].getPosition(),
                                             perspective, min, max));
        }
        else
        {
         /*   for( i = 0; i < model->triangleNumber; i++ )
            {
                triangle[ i ].setNewSelected( false );
                for( j = 0; j < 3; j++ )
                {
                    if( isSelected( widget->getFinalMatrix(),
                        vertex[ triangle[ i ].
                        getIndex( j ) ].getPosition(), perspective,
                                    min, max ) )
                    {
                        triangle[ i ].setNewSelected( true );
                        break;
                    }
                }
            }*/
        }
        return;
    }
    if(action == DRAW)
    {
        VertexAndIndexData *data = widget->getToolData();
        vector <VertexData_Color> &vertices = data->vertices;
        vector <GLushort> &indices = data->indices;
        vertices.resize(4);

        QVector2D min, max;
        QVector2D startPosition = widget->getStartPosition();
        QVector2D currentPosition = widget->getCurrentPosition();
        min.setX(qMin(startPosition.x(), currentPosition.x()) - 1);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 1);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 1);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 1);

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

        bool perspective = projection == PERSPECTIVE;
        widget->countFinalMatrix(perspective);
        int j;
        if(workWithElements[0]->isChecked())
        {
            for(i = 0; i < vertexSize; i++)
                vertex[i].setNewSelected(isSelected(widget->
                    getFinalMatrix(), vertex[i].getPosition(), perspective,
                                                    min, max));
        }
        else
        {
            for(i = 0; i < triangleSize; i++)
            {
                triangle[i].setNewSelected(false);
                for(j = 0; j < 3; j++)
                {
                    if(isSelected(widget->getFinalMatrix(), vertex[
                        triangle[i].getIndex(j)].getPosition(), perspective,
                                    min, max))
                    {
                        triangle[i].setNewSelected(true);
                        break;
                    }
                }
            }
        }
    }
    if(action == STOP)
    {
        if(workWithElements[0]->isChecked())
        {
            int amount = 0;
            for(i = 0; i < vertexSize; i++)
            {
                amount += vertex[i].newSelected();
                if(!checkBox[0]->isChecked() && !checkBox[1]->isChecked())
                    vertex[ i ].setSelected(vertex[i].newSelected());
                if(checkBox[0]->isChecked() && !vertex[i].isSelected())
                    vertex[i].setSelected(vertex[i].newSelected());
                if(checkBox[1]->isChecked() && vertex[i].isSelected())
                    vertex[i].setSelected(!vertex[i].newSelected());
                vertex[i].setNewSelected(false);
            }
        }
        else
        {
            for(i = 0; i < triangleSize; i++)
            {
                if(!checkBox[0]->isChecked() && !checkBox[1]->isChecked())
                    triangle[i].setSelected(triangle[i].newSelected());
                if(checkBox[0]->isChecked() && !triangle[i].isSelected())
                    triangle[i].setSelected(triangle[i].newSelected());
                if(checkBox[1]->isChecked() && triangle[i].isSelected())
                    triangle[i].setSelected(!triangle[i].newSelected());
                triangle[i].setNewSelected(false);
            }
        }
    }
}
