#include "functions.h"

#include <QLayout>
#include <QDoubleSpinBox>
#include <iostream>

using namespace std;

bool isSelected(QMatrix4x4 finalMatrix, QVector3D vertex, bool perspective,
                QVector2D min, QVector2D max)
{
    QVector4D result = finalMatrix * QVector4D(vertex, 1);
    if(perspective) for(int i = 0; i < 2; i++) result[i] /= result[3];
    return result.x() > min.x() && result.x() < max.x() && result.y() >
            min.y() && result.y() < max.y();
}

QVector3D fromScreenToWorld( QMouseEvent *event, GLWidget *widget, bool forcedHeight, double height )
{
    return _fromScreenToWorld( QVector4D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y(),
                                                                   0, 1 ), widget, forcedHeight, height );
}

QVector3D _fromScreenToWorld( QVector4D screenCoordinates, GLWidget *widget, bool forcedHeight, double height )
{
    bool perspective = widget->getProjection() == PERSPECTIVE;
    widget->countFinalInverseMatrix( perspective );
    QVector4D worldCoordinates;
    if( perspective )
    {
        QMatrix4x4 finalInverseMatrix = widget->
                getFinalInverseMatrix();
        double a[ 4 ][ 4 ];
        int i, j;
        for( i = 0; i < 4; i++ ) for( j = 0; j < 4; j++ ) a[ i ][ j ] = finalInverseMatrix.data()[ 4 * j + i ];
        Camera *camera = widget->getCamera();
        QVector4D screenCoordPersp = screenCoordinatesPerspective( a, height, screenCoordinates );
        if( !forcedHeight && screenCoordPersp.z() < 0 )
        {
            height = int( camera->position().z() ) + 2 * ( camera->position().z() > 0 ) - 1;
            screenCoordPersp = screenCoordinatesPerspective( a, height, screenCoordinates );
        }
        worldCoordinates = finalInverseMatrix * screenCoordPersp;
        worldCoordinates.setZ( height );
    }
    else worldCoordinates = widget->getFinalInverseMatrix() *
            screenCoordinates;
    return QVector3D( worldCoordinates );
}

QVector4D screenCoordinatesPerspective( double a[ 4 ][ 4 ], double h, QVector4D screenCoordinates )
{
    QVector4D answer = screenCoordinates;
    double x = screenCoordinates.x(), y = screenCoordinates.y();
    double w = ( a[ 2 ][ 2 ] - a[ 3 ][ 2 ] * h ) / ( ( a[ 3 ][ 0 ] * a[ 2 ][ 2 ] - a[ 3 ][ 2 ] * a[ 2 ][ 0 ] ) * x + ( a[ 3 ][ 1 ] * a[ 2 ][ 2 ] - a[ 3 ][ 2 ] *
                    a[ 2 ][ 1 ] ) * y + a[ 3 ][ 3 ] * a[ 2 ][ 2 ] - a[ 3 ][ 2 ] * a[ 2 ][ 3 ] );
    answer.setZ( ( h - ( a[ 2 ][ 0 ] * x + a[ 2 ][ 1 ] * y + a[ 2 ][ 3 ] ) * w ) / a[ 2 ][ 2 ] );
    answer.setW( w );
    for( int i = 0; i < 2; i++ ) answer[ i ] *= w;
    return answer;
}

double inRadians( double value )
{
    return value * M_PI / double( 180 );
}

int sign( double number )
{
    return ( number > 0 ) ? 1 : -1;
}

