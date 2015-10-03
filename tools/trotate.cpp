#include "trotate.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TRotate::TRotate(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Rotate");
    finalButton = new QPushButton("Rotate");

    int i;
    MyLabel *label[4];
    label[0] = new MyLabel("Angle:", 50);
    for(i = 1; i < 4; i++) label[i] = new MyLabel(QString('W' + i) + ':', 50);
    for(i = 0; i < 4; i++)
    {
        spinBox[i] = new MySpinBox;
        layout->addWidget(spinBox[i], i, 1);
        layout->addWidget(label[i], i, 0);
    }
    spinBox[0]->setMaximum(360);
    spinBox[0]->setMinimum(-360);

    checkBoxCustomAxis = new MyCheckBoxMW;
    checkBoxCustomAxis->setText("Custom Axis");
    checkBoxCustomAxis->setChecked(true);
    layout->addWidget(checkBoxCustomAxis, 5, 0, 1, 2);
    layout->addWidget(finalButton, 4, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
}

void TRotate::function(Action action, QMouseEvent *event)
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
            for(i++; i < vertexSize; i++)
            {
                if(vertex[i].isSelected())
                {
                    for(j = 0; j < 3; j++)
                    {
                        if(vertex[i].getPosition()[j] > max[j]) max[j] =
                                    vertex[i].getPosition()[ j ];
                        if(vertex[i].getPosition()[j] < min[j]) min[j] =
                                    vertex[i].getPosition()[j];
                    }
                }
            }
        }
        else
        {
            int index;
            bool *checked = new bool[vertexSize];
            for(i = 0; i < vertexSize; i++) checked[i] = false;
            for(i = 0; i < triangleSize; i++)
            {
                if(triangle[i].isSelected())
                {
                    min = max = vertex[triangle[i].getIndex(0)].getPosition();
                    break;
                }
            }

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
        }
        center = (min + max);
        for(i = 0; i < 3; i++) center[i] /= 2;
        pivot = center;
        pivotOnScreen = widget->fromWorldToScreen(pivot, true);
        if(action == START) return;
    }

    double angle;
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    if(action == FINAL)
    {
        angle = spinBox[0]->value();
        if(!getAxis(spinBox, &rotation, angle)) return;
    }
    else
    {
        bool execute = action == EXECUTE;
        QVector2D lastPosition = execute ? widget->getLastPosition() :
                                           widget->getStartPosition();
        QVector2D currentPosition = execute ? QVector2D(event->x() -
            widget->getHalfWidth(), widget->getHalfHeight() - event->y()) :
                                              widget->getCurrentPosition();
        QVector2D toCurrent = (currentPosition - pivotOnScreen).normalized();
        QVector2D toLast = (lastPosition - pivotOnScreen).normalized();

        angle = qAcos(QVector2D::dotProduct(toCurrent, toLast)) * 180 / M_PI;
        if(toCurrent.x() * toLast.y() > toCurrent.y() * toLast.x())
            angle *= -1;
        if(action == DRAW)
        {
            VertexAndIndexData *data = widget->getToolData();
            vector <VertexData_Color> &vertices = data->vertices;
            vector <GLushort> &indices = data->indices;
            vertices.resize(3);
            vertices[0].position = lastPosition;
            vertices[1].position = pivotOnScreen;
            vertices[2].position = currentPosition;

            indices.resize(4);
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 1;
            indices[3] = 2;

            spinBox[0]->setValue(angle);
            return;
        }
        if(checkBoxCustomAxis->isChecked())
        {
            widget->countRotationMatrices();
            rotation *= widget->getRotationMatrixInverse();
            rotation.rotate(angle, 0, 0, 1);
            rotation *= widget->getRotationMatrix();
        }
        else if( !getAxis( spinBox, &rotation, angle ) ) return;
    }

    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.translate(pivot);
    transformation *= rotation;
    transformation.translate(-pivot);

    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertexSize; i++) if(vertex[i].isSelected())
            vertex[i].multiplyPosition(transformation);
    }
    else
    {
        int index;
        bool *checked = new bool[vertexSize];
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
    }
}
