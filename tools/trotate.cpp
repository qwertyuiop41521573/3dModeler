#include "trotate.h"
#include "glwidget.h"
#include "mainwindow.h"

#include "gui/mylabel.h"

TRotate::TRotate() : ToolWithPivot()
{
    button->setText("Rotate");
    finalButton = new QPushButton("Rotate");

    int i;
    MyLabel *label[4];
    label[0] = new MyLabel("Angle:", 50);
    spinBoxAngle = new MySpinBox;
    layout->addWidget(spinBoxAngle, 0, 1);
    for(i = 1; i < 4; i++) label[i] = new MyLabel(QString('W' + i) + ':', 50);
    for(i = 0; i < 4; i++) layout->addWidget(label[i], i, 0);
    spinBox[0]->setMaximum(360);
    spinBox[0]->setMinimum(-360);

    checkBoxCustomAxis = new MyCheckBoxMW;
    checkBoxCustomAxis->setText("Custom Axis");
    checkBoxCustomAxis->setChecked(true);
    layout->addWidget(checkBoxCustomAxis, 4, 0, 1, 2);
    layout->addWidget(finalButton, 5, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _widget->hide();
}

void TRotate::function(Action action, QMouseEvent *event)
{
    if(action == STOP)
    {
        TransformingTool::function(action, 0);
        return;
    }

    GLWidget *widget = Workspace::activeWidget();
    int i, j, k;

    ToolWithPivot::function(action, event);
    if(action == START)
    {
        TransformingTool::function(action, 0);
        return;
    }

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    if(action == FINAL)
    {
        if(!createRotationMatrix(&rotation, spinBoxAngle->value())) return;
    }
    else
    {
        bool execute = action == EXECUTE;
        const QVector2D &lastPosition = execute ? widget->getLastPosition() : widget->getStartPosition();
        QVector2D currentPosition = execute ? QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y()) : widget->getCurrentPosition();
        QVector2D toCurrent = (currentPosition - pivotOnScreen).normalized();
        QVector2D toLast = (lastPosition - pivotOnScreen).normalized();

        double angle = qAcos(QVector2D::dotProduct(toCurrent, toLast)) * 180 / M_PI;
        if(toCurrent.x() * toLast.y() > toCurrent.y() * toLast.x())
            angle *= -1;
        if(action == DRAW)
        {
            VertexAndIndexData &data = widget->getToolData();
            vector <VertexData_Color> &vertices = data.vertices;
            vector <GLuint> &indices = data.indices;
            vertices.resize(3);
            vertices[0].position = lastPosition;
            vertices[1].position = pivotOnScreen;
            vertices[2].position = currentPosition;

            indices.resize(4);
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 1;
            indices[3] = 2;

            spinBoxAngle->setValue(angle);
            return;
        }
        if(checkBoxCustomAxis->isChecked())
        {
            widget->countRotationMatrices();
            rotation *= widget->getRotationMatrixInverse();
            rotation.rotate(angle, 0, 0, 1);
            rotation *= widget->getRotationMatrix();
        }
        else if(!createRotationMatrix(&rotation, angle)) return;
    }

    //create transformation matrix
    transformation.setToIdentity();
    transformation.translate(pivot);
    transformation *= rotation;
    transformation.translate(-pivot);

    TransformingTool::function(action, 0);
}

bool TRotate::createRotationMatrix(QMatrix4x4 *rotation, double angle)
{
    GLWidget *widget = Workspace::activeWidget();

    if(checkBoxCustomAxis->isChecked())
    {
        widget->countRotationMatrices();
        *rotation *= widget->getRotationMatrixInverse();
        rotation->rotate(angle, 0, 0, 1);
        *rotation *= widget->getRotationMatrix();
        return true;
    }
    else
    {
        QVector3D axis;
        for(int i = 0; i < 3; i++) axis[i] = spinBox[i]->value();
        if(axis.isNull()) return false;
        axis.normalize();
        rotation->rotate(angle, axis);
        return true;
    }
}
