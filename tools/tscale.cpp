#include "tscale.h"
#include "glwidget.h"
#include "mainwindow.h"

TScale::TScale(MainWindow *mainWindow) : ToolWithPivot(mainWindow)
{
    button->setText("Scale");
    finalButton = new QPushButton("Scale");

    for(int i = 0; i < 3; i++)
    {
        spinBox[i]->setMinimum(0);
        spinBox[i]->setValue(1);
        pushButton[i] = new MyPushButtonMW(QString('X' + i));
        layout->addWidget(pushButton[i], i + 1, 0);
    }

    layout->addWidget(finalButton, 4, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(final()));
    _widget->hide();

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

    ToolWithPivot::function(action, event);
    if(action == START) return;

    QVector3D scaleFactor;
    if(action == FINAL) for(i = 0; i < 3; i++) scaleFactor[i] = spinBox[i]->
            value();
    else
    {
        bool execute = action == EXECUTE;
        const QVector2D &lastPosition = execute ? widget->getLastPosition() :
                                           widget->getStartPosition();
        QVector2D currentPosition = execute ? QVector2D( event->x() -
            widget->getHalfWidth(), widget->getHalfHeight() - event->y()) :
                                              widget->getCurrentPosition();
        QVector2D dr = currentPosition - lastPosition;
        if(widget->getProjection() != PERSPECTIVE) dr *= 3;

        QVector2D temp = QVector2D(widget->getStartPosition()) -
                pivotOnScreen;

        QVector2D e[3];
        double drTransformed[3];
        widget->countFinalMatrix();
        for(i = 0; i < 3; i++)
        {
            widget->fromWorldToScreen(e[i], QVector3D(i == 0, i == 1, i ==
                                                       2), false);
            drTransformed[i] = QVector2D::dotProduct(e[i], dr);
            if(QVector2D::dotProduct(e[i], temp) < 0) drTransformed[i] *= -1;
        }

        for(i = 0; i < 3; i++) scaleFactor[i] = exp(drTransformed[i] / 50000);
        if(action == DRAW)
        {
            VertexAndIndexData &data = widget->getToolData();
            data.vertices.resize(2);
            data.vertices[0].position = lastPosition;
            data.vertices[1].position = currentPosition;

            data.indices.resize(2);
            for(i = 0; i < 2; i++) data.indices[i] = i;

            for(i = 0; i < 3; i++) spinBox[i]->setValue(scaleFactor[i]);
                return;
        }
    }

    for(i = 0; i < 3; i++) if(pushButton[i]->isChecked()) scaleFactor[i] = 1;

    transformation.setToIdentity();
    transformation.translate(pivot);
    transformation.scale(scaleFactor);
    transformation.translate(-pivot);

    transform();
}
