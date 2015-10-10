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
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    Projection projection = widget->getProjection();
    //work with vertices
    bool workWithVert = workWithElements[0]->isChecked();

    switch(action)
    {
    case START:
    {
        QVector2D min, max;
        QVector2D currentPosition(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
        countMinAndMax(&min, &max, currentPosition);

        widget->countFinalMatrix();
        int j;
        if(workWithVert)
        {
            for(i = 0; i < vertexSize; i++) if(widget->isSelected(vertex[i].getPosition(), min, max)) vertex[i].setNewSelected(true);
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
        widget->countFinalMatrix();
        int j;
        if(workWithVert)
        {
            for(i = 0; i < vertexSize; i++) vertex[i].setNewSelected(widget->isSelected(vertex[i].getPosition(), min, max));
        }
        else
        {
            for(i = 0; i < triangleSize; i++)
            {
                triangle[i].setNewSelected(false);
                for(j = 0; j < 3; j++)
                {
                    if(widget->isSelected(vertex[triangle[i].getIndex(j)].getPosition(), min, max))
                    {
                        triangle[i].setNewSelected(true);
                        break;
                    }
                }
            }
        }
        break;
    }
    case STOP:
    {
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
            if(checkBox[0]->isChecked()  && selObj[i]->newSelected()) selObj[i]->setSelected(true, false);
            if(checkBox[1]->isChecked()  && selObj[i]->newSelected()) selObj[i]->setSelected(false, false);
        }
        break;
    }
    }
}

void TSelect::countMinAndMax(QVector2D *min, QVector2D *max, const QVector2D &currentPosition)
{
    GLWidget *widget = *_activeWidget;
    const QVector2D &startPosition = widget->getStartPosition();

    min->setX(qMin(startPosition.x(), currentPosition.x()) - 5);
    min->setY(qMin(startPosition.y(), currentPosition.y()) - 5);
    max->setX(qMax(startPosition.x(), currentPosition.x()) + 5);
    max->setY(qMax(startPosition.y(), currentPosition.y()) + 5);
}
