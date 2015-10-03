#include "tvertex.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"
#include "gui/mylabel.h"

TVertex::TVertex(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
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
}

void TVertex::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    int vertexSize = vertex.size();
    if(action == DRAW) return;
        if(action == START || action == FINAL)
        {
            QVector3D newVertex;
            if(action == START) newVertex = fromScreenToWorld(event, widget);
            else for(int i = 0; i < 3; i++) newVertex[i] = spinBox[i]->value();
            vertex.push_back(newVertex);
            vertex[vertexSize].setNewSelected(true);
            if(action == FINAL) action = STOP;
        }
        if(action == EXECUTE) vertex[vertexSize - 1].setPosition(
                    fromScreenToWorld(event, widget));
        if(action == STOP)
        {
            vertexSize = vertex.size();
            vertex[vertexSize - 1].setNewSelected(false);
            vertex[vertexSize - 1].setSelected(true);
        }
}
