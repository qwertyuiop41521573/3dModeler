#include "tscale.h"
#include "glwidget.h"
#include "mainwindow.h"

TScale::TScale(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Scale");
    finalButton = new QPushButton("Scale");

    int i;
    for(i = 0; i < 3; i++)
    {
        pushButton[i] = new MyPushButtonMW(QString('X' + i));
        spinBox[i] = new MySpinBox;
        layout->addWidget(pushButton[i], i, 0);
        layout->addWidget(spinBox[i], i, 1);
    }

    layout->addWidget(finalButton, 3, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));

}

void TScale::function(Action action, QMouseEvent *event)
{
    if(action == STOP) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int i, j, k;

    if(action == START || action == FINAL)
    {
        QVector3D min, max;
        QVector3D center;
        if(workWithElements[0]->isChecked())
        {
            for(i = 0; i < vertexSize; i++)
            {
                if(vertex[i].isSelected())
                {
                    min = max = vertex[i].getPosition();
                    break;
                }
            }
            for( i++; i < vertexSize; i++)
            {
                if(vertex[i].isSelected())
                {
                    for(j = 0; j < 3; j++)
                    {
                        if(vertex[i].getPosition()[j] > max[j]) max[j] =
                                vertex[i].getPosition()[j];
                        if(vertex[i].getPosition()[j] < min[j]) min[j] =
                                vertex[i].getPosition()[j];
                    }
                }
            }
        }
        else
        {
            int index;
            for(i = 0; i < triangleSize; i++)
            {
                if(triangle[i].isSelected())
                {
                    min = max = vertex[triangle[i].getIndex(0)].getPosition();
                    break;
                }
            }
            checked.resize(vertexSize);
            for(i = 0; i < vertexSize; i++) checked[i] = false;
            for( ; i < triangleSize; i++)
            {
                if(triangle[i].isSelected())
                {
                    for(j = 0; j < 3; j++)
                    {
                        index = triangle[i].getIndex(j);
                        if(!checked[index])
                        {
                            for(k = 0; k < 3; k++)
                            {
                                checked[index] = true;
                                if(vertex[index].getPosition()[k] > max[k])
                                    max[k] = vertex[index].getPosition()[k];
                                if(vertex[index].getPosition()[k] < min[k])
                                    min[k] = vertex[index].getPosition()[k];
                            }
                        }
                    }
                }
            }
            checked.clear();
        }
        center = (min + max);
        for(i = 0; i < 3; i++) center[i] /= 2;
        pivot = center;
        pivotOnScreen = widget->fromWorldToScreen(pivot, true);
        if(action == START) return;
    }
    QVector3D scaleFactor;
    if(action == FINAL) for(i = 0; i < 3; i++) scaleFactor[i] = spinBox[i]->
            value();
    else
    {
        bool execute = action == EXECUTE;
        QVector2D lastPosition = execute ? widget->getLastPosition() :
                                           widget->getStartPosition();
        QVector2D currentPosition = execute ? QVector2D( event->x() -
            widget->getHalfWidth(), widget->getHalfHeight() - event->y()) :
                                              widget->getCurrentPosition();
        QVector2D dr = currentPosition - lastPosition;
        bool perspective = widget->getProjection() == PERSPECTIVE;
        if(!perspective) dr *= 3;

        QVector2D temp = QVector2D(widget->getStartPosition()) -
                pivotOnScreen;

        QVector2D e[3];
        double drTransformed[3];
        for(i = 0; i < 3; i++)
        {
            e[i] = widget->fromWorldToScreen(QVector3D(i == 0, i == 1, i ==
                                                       2), false);
            drTransformed[i] = QVector2D::dotProduct(e[i], dr);
            if(QVector2D::dotProduct(e[i], temp) < 0) drTransformed[i] *= -1;
        }

        widget->countFinalInverseMatrix(perspective);

        for(i = 0; i < 3; i++) scaleFactor[i] = exp(drTransformed[i] / 50000);
        if(action == DRAW)
        {
            VertexAndIndexData *data = widget->getToolData();
            data->vertices.resize(2);
            data->vertices[0].position = lastPosition;
            data->vertices[1].position = currentPosition;

            data->indices.resize(2);
            for(i = 0; i < 2; i++) data->indices[i] = i;

            for(i = 0; i < 3; i++) spinBox[i]->setValue(scaleFactor[i]);
                return;
        }
    }

    for(i = 0; i < 3; i++) if(pushButton[i]->isChecked()) scaleFactor[i] = 1;

    QVector3D vector;
    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.translate(pivot);
    transformation.scale(scaleFactor);
    transformation.translate(-pivot);

    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertexSize; i++) if(vertex[i].isSelected())
            vertex[i].multiplyPosition(transformation);
    }
    else
    {
        int index;
        checked.resize(vertexSize);
        for(i = 0; i < vertexSize; i++) checked[i] = false;

        for(i = 0; i < triangleSize; i++)
        {
            if(triangle[i].isSelected())
            {
                for(j = 0; j < 3; j++)
                {
                    index = triangle[i].getIndex(j);
                    if(!checked[index])
                    {
                        checked[index] = true;
                        vertex[index].multiplyPosition(transformation);
                    }
                }
            }
        }
        checked.clear();
    }
}
