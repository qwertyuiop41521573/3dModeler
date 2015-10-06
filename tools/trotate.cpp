#include "trotate.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TRotate::TRotate(MainWindow *mainWindow) : ToolWithPivot(mainWindow)
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

    ToolWithPivot::function(action, event);
    if(action == START) return;

    double angle;
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    if(action == FINAL)
    {
        angle = spinBox[0]->value();
        if(!getAxis(rotation, angle)) return;
    }
    else
    {
        bool execute = action == EXECUTE;
        const QVector2D &lastPosition = execute ? widget->getLastPosition() :
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
            VertexAndIndexData &data = widget->getToolData();
            vector <VertexData_Color> &vertices = data.vertices;
            vector <GLushort> &indices = data.indices;
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
        else if(!getAxis(rotation, angle)) return;
    }

    //create transformation matrix
    transformation.setToIdentity();
    transformation.translate(pivot);
    transformation *= rotation;
    transformation.translate(-pivot);

    transform();
}

bool TRotate::getAxis(QMatrix4x4 &rotation, double angle)
{
    QVector3D axis;
    for(int i = 0; i < 3; i++) axis[i] = spinBox[i + 1]->value();
    if(axis.isNull()) return false;
    axis.normalize();
    rotation.rotate(angle, axis);
    return true;
}
