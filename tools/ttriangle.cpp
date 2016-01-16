#include "ttriangle.h"
#include "glwidget.h"
#include "mainwindow.h"
#include "journal.h"

using namespace Model;

TTriangle::TTriangle() : CreatingTool()
{
    button->setText("Triangle");
    finalButton = new QPushButton("Cancel");
    finalButton->setMaximumWidth(150);

    layout->addWidget(finalButton, 0, 0, 1, 2 );
    connect(finalButton, SIGNAL(clicked()), this, SLOT(final()));
    _widget->hide();
}

void TTriangle::function(Action action, QMouseEvent *event)
{
    if(action != START && action != FINAL) return;

    int i;

    if(action == START)
    {
        GLWidget *widget = Workspace::activeWidget();

        for(i = 0; i < newTriangle.size(); i++) vertex()[newTriangle[i]].setNewSelected(true);

        QVector2D min, max;
        const QVector2D &startPosition = widget->getStartPosition();
        QVector2D currentPosition(event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y());
        //pressing mouse is equal to selecting with square with side 10px
        min.setX(qMin(startPosition.x(), currentPosition.x()) - 5);
        min.setY(qMin(startPosition.y(), currentPosition.y()) - 5);
        max.setX(qMax(startPosition.x(), currentPosition.x()) + 5);
        max.setY(qMax(startPosition.y(), currentPosition.y()) + 5);

        widget->countFinalMatrix();
        int j;
        for(i = 0; i < vertex().size(); i++)
        {
            //check if this vertex is selected
            if(!widget->isSelected(vertex()[i].position(), min, max)) continue;

            //check if it is already in newTriangle
            bool selectedBefore = false;
            for(j = 0; j < newTriangle.size(); j++)
            {
                if(i != newTriangle[j]) continue;

                selectedBefore = true;
                break;
            }
            if(selectedBefore) continue;
            
            newTriangle.push_back(i);
            vertex()[i].setNewSelected(true);
        }
        //if it's first vertex
        if(newTriangle.size() == 1) _busy = true;
        //if last
        if(newTriangle.size() == 3)
        {
            _busy = false;
            Journal::newRecord(CREATE);
            tri.push_back(triangle().push({newTriangle.data(), 0}));
            Journal::submit();
            for(i = 0; i < 3; i++) vertex()[newTriangle[i]].setSelected(true, false);
            newTriangle.clear();
            updateNormals();
        }
    }
    //if "cancel" was pressed
    else
    {
        _busy = false;
        for(i = 0; i < newTriangle.size(); i++) vertex()[newTriangle[i]].setNewSelected(false);
        newTriangle.clear();
    }
}

void TTriangle::setActive(bool value)
{
    if(!value) function(FINAL, 0);
    ToolWithWidget::setActive(value);
}
