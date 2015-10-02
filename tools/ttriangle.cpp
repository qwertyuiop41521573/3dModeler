#include "ttriangle.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TTriangle::TTriangle(MainWindow *mainWindow) : ToolWithWidget(
                                                   mainWindow)
{
    button->setText("Triangle");
    finalButton = new QPushButton("Cancel");

    layout->addWidget(finalButton, 0, 0 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT(
                final() ) );
}

void TTriangle::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action != START && action != FINAL ) return;
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    if( action == START )
    {
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
        int triangleSize = triangle.size();
        if( triangleSize == triangleSize)
        {
            triangle.push_back( {-1,-1,-1 } );
            triangleSize++;
        }
        for( i = 0; i < vertexSize; i++ )
        {
            if( !vertex[ i ].newSelected() && isSelected(
                        widget->getFinalMatrix(), vertex[ i ].
                           getPosition(), perspective, min, max ) )
            {
                vertex[ i ].setNewSelected( true );
                for( j = 0; j < 3; j++ )
                {
                    if( triangle[ triangleSize - 1 ].
                            getIndex( j ) == -1 )
                    {
                        triangle[ triangleSize - 1 ].
                                setIndex( j, i );
                        break;
                    }
                }
            }
        }
        if( triangle[ triangleSize - 1 ].getIndex( 2 )
                != -1 )
        {
            for( i = 0; i < 3; i++ ) vertex[ triangle[
                triangleSize - 1 ].getIndex( i ) ].setNewSelected(
                        false );
        }
    }
    else if( triangle.size() > triangleSize )
    {
        for( i = 0; i < 3; i++ )
        {
            int index = triangle[ triangleSize ].
                    getIndex( i );
            if( index ==-1 ) break;
            else vertex[ index ].setNewSelected( false );
        }
        triangle.resize( triangleSize );
    }
}
