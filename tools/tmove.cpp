#include "tmove.h"
#include "glwidget.h"
#include "mainwindow.h"

TMove::TMove(MainWindow *mainWindow) : TransformingTool(mainWindow)
{
    button->setText("Move");
    finalButton = new QPushButton("Move");

    for(int i = 0; i < 3; i++)
    {
        pushButton[i] = new MyPushButtonMW(QString('X' + i));
        layout->addWidget(pushButton[i], i + 1, 0);
    }

    layout->addWidget(finalButton, 4, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();
}

void TMove::function(Action action, QMouseEvent *event)
{
    if(action == START || action == STOP)
    {
        _busy = action == START;
        return;
    }

    GLWidget *widget = *_activeWidget;
    int i;

    QVector3D drInWorld; //(not on screen)
    if(action == FINAL) for(i = 0; i < 3; i++) drInWorld[i] = spinBox[i]->value();
    else
    {
        bool execute = action == EXECUTE;
        const QVector2D &lastPosition = execute ? widget->getLastPosition() : widget->getStartPosition();
        QVector2D currentPosition = execute ? QVector2D(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y()) : widget->getCurrentPosition();
        QVector2D dr = currentPosition - lastPosition;
        if(widget->getProjection() == PERSPECTIVE) dr *= 10;
        widget->countFinalInverseMatrix();
        drInWorld = QVector3D(widget->getFinalInverseMatrix() * QVector4D(dr.x(), dr.y(), 0.f, 0.f));
        if(action == DRAW)
        {
            VertexAndIndexData &data = widget->getToolData();
            data.vertices.resize(2);
            data.vertices[0].position = lastPosition;
            data.vertices[1].position = currentPosition;
            data.indices.resize(2);
            for(i = 0; i < 2; i++) data.indices[i] = i;
            for(i = 0; i < 3; i++) spinBox[i]->setValue(drInWorld[i]);
            return;
        }
    }
    for(i = 0; i < 3; i++) drInWorld[i] *= !pushButton[i]->isChecked();

    transformation.setToIdentity();
    transformation.setToIdentity();
    transformation.translate(drInWorld[0], drInWorld[1], drInWorld[2]);

    transform();
}
