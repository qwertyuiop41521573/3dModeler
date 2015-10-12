#include "tvertex.h"
#include "glwidget.h"
#include "mainwindow.h"

#include "gui/mylabel.h"

TVertex::TVertex(MainWindow *mainWindow) : CreatingTool(mainWindow)
{
    button->setText("Vertex");
    finalButton = new QPushButton("Create Vertex");

    int i;
    MyLabel *label[3];
    for(i = 0; i < 3; i++ )
    {
        spinBox[i] = new MySpinBox;
        label[i] = new MyLabel(QString('X' + i) + ':', 50);
        layout->addWidget(label[i], i, 0);
        layout->addWidget(spinBox[i], i, 1);
    }
    layout->addWidget(finalButton, 3, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();
}

void TVertex::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    VertexContainer &vertex = model->getVertex();
    widget->countFinalInverseMatrix();
    if(action == START || action == FINAL)
    {
        QVector3D newVertex;
        if(action == START) widget->fromScreenToWorld(&newVertex, event);
        else for(int i = 0; i < 3; i++) newVertex[i] = spinBox[i]->value();
        ind.clear();
        ind.push_back(vertex.push(newVertex));
        vertex[ind[0]].setNewSelected(true);
        if(action == FINAL) action = STOP;
    }
    if(action == EXECUTE) widget->fromScreenToWorld(&vertex[ind[0]].getEditablePosition(), event);
    if(action == STOP) vertex[ind[0]].setSelected(true, false);
}
