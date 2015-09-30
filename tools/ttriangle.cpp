#include "ttriangle.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TTriangle::TTriangle(MainWindow *mainWindow) : ToolWithWidget(
                                                   mainWindow)
{
    button->setText("Triangle");
    finalButton = new QPushButton("Cancel");

    elements = new WidgetElements( 0, 0, 0, 0, 0 );
    layout->addWidget(finalButton, 0, 0 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT(
                final() ) );
    _widget->hide();
}

void TTriangle::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action != START && action != FINAL ) return;
    int i;
    if( action == START )
    {

        WidgetElements *workWithElements = widget->
                getWorkWithElements();

        QVector2D min, max;
        QVector2D startPosition = widget->getStartPosition();
        QVector2D currentPosition( event->x() - widget->
            getHalfWidth(), widget->getHalfHeight() - event->y() );
        min.setX( qMin( startPosition.x(),
                        currentPosition.x() ) - 5 );
        min.setY( qMin( startPosition.y(),
                        currentPosition.y() ) - 5 );
        max.setX( qMax( startPosition.x(),
                        currentPosition.x() ) + 5 );
        max.setY( qMax( startPosition.y(),
                        currentPosition.y() ) + 5 );

        bool perspective = widget->getProjection() == PERSPECTIVE;
        widget->countFinalMatrix( perspective );
        int j;
        int triangleSize = model->triangle.size();
        if( triangleSize == model->triangleNumber )
        {
            model->triangle.push_back( {-1,-1,-1 } );
            triangleSize++;
        }
        for( i = 0; i < model->vertexNumber; i++ )
        {
            if( !model->vertex[ i ].newSelected() && isSelected(
                        widget->getFinalMatrix(), model->vertex[ i ].
                           getPosition(), perspective, min, max ) )
            {
                model->vertex[ i ].setNewSelected( true );
                for( j = 0; j < 3; j++ )
                {
                    if( model->triangle[ triangleSize - 1 ].
                            getIndex( j ) == -1 )
                    {
                        model->triangle[ triangleSize - 1 ].
                                setIndex( j, i );
                        break;
                    }
                }
            }
        }
        if( model->triangle[ triangleSize - 1 ].getIndex( 2 )
                != -1 )
        {
            for( i = 0; i < 3; i++ ) model->vertex[ model->triangle[
                triangleSize - 1 ].getIndex( i ) ].setNewSelected(
                        false );
            model->triangleNumber++;
        }
    }
    else if( model->triangle.size() > model->triangleNumber )
    {
        for( i = 0; i < 3; i++ )
        {
            int index = model->triangle[ model->triangleNumber ].
                    getIndex( i );
            if( index ==-1 ) break;
            else model->vertex[ index ].setNewSelected( false );
        }
        model->triangle.resize( model->triangleNumber );
    }
}
