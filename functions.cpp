#include "functions.h"

#include <QLayout>
#include <QDoubleSpinBox>
#include <iostream>

using namespace std;

void setupButton( QPushButton *button )
{
    button->setCheckable( true );
    button->setMaximumWidth( 70 );
    button->setFixedHeight( 20 );
}

void setupSpinBox( QDoubleSpinBox *spinBox, double defaultValue )
{
    spinBox->setMaximumWidth( 70 );
    spinBox->setFixedHeight( 20 );
    spinBox->setMaximum( 1000 );
    spinBox->setMinimum(-1000 );
    spinBox->setValue( defaultValue );
}





void toolVertexFunction( GLWidget *widget, Action action, QMouseEvent
                                *event, VertexAndIndexData *data )
{
  /*  if( action == DRAW ) return;
    Model *model = widget->getModel();
    if( action == START || action == FINAL )
    {
        QVector3D newVertex;
        if( action == START ) newVertex = fromScreenToWorld( event, widget );
        else for( int i = 0; i < 3; i++ ) newVertex[ i ]
            = widget->getActiveTool()->getElements()->getSpinBox( i )->
                value();
        model->vertex.push_back( newVertex );
        model->vertex[ model->vertexNumber ].setNewSelected( true );
        model->vertexNumber++;
        if( action == FINAL ) action = STOP;
    }
    if( action == EXECUTE ) model->vertex[ model->vertexNumber - 1 ].setPosition(
            fromScreenToWorld( event, widget ) );
    if( action == STOP )
    {
        int vertexNumber = model->vertexNumber;
        model->vertex[ vertexNumber - 1 ].setNewSelected( false );
        model->vertex[ vertexNumber - 1 ].setSelected( true );
    }*/
}

void toolTriangleFunction( GLWidget *widget, Action action,
             QMouseEvent *event, VertexAndIndexData *data )
{
    if( action != START && action != FINAL ) return;
    Model *model = widget->getModel();
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

void toolPlaneFunction( GLWidget *widget, Action action, QMouseEvent
                      *event, VertexAndIndexData *data )
{
 /*   if( action == DRAW ) return;
    Model *model = widget->getModel();
    int vertexSize = model->vertexNumber;
    int triangleSize = model->triangleNumber;
    if( action == START || action == FINAL )
    {
        int i;
        model->vertex.resize( vertexSize + 4 );
        model->vertexNumber += 4;

        model->triangle.resize( triangleSize + 2 );
        model->triangle[ triangleSize ].setIndices( vertexSize, vertexSize + 1, vertexSize + 2 );
        model->triangle[ triangleSize + 1 ].setIndices( vertexSize, vertexSize + 2, vertexSize + 3 );
        model->triangleNumber += 2;

        if( action == START )
        {
            QVector3D worldCoordinates = fromScreenToWorld( event, widget );
            for( i = 0; i < 4; i++ ) model->vertex[ vertexSize + i ] = worldCoordinates;
        }
        else
        {
            WidgetElements *elements = widget->getActiveTool()->getElements();
            for( i = 0; i < 4; i++ )
                model->vertex[ vertexSize + i ].setPosition(
                    elements->getSpinBox( 3 * i )->value(),
                    elements->getSpinBox( 3 * i + 1 )->value(),
                    elements->getSpinBox( 3 * i + 2 )->value() );
            action = STOP;
        }
        for( i = 0; i < 4; i++ ) model->vertex[ vertexSize + i ].
                setNewSelected( true );

    }
    if( action == EXECUTE )
    {
        QVector2D diagonal;
        QVector3D posA, posB;
        bool square = widget->getActiveTool()->getElements()->getCheckBox( 0 )->isChecked();
        if( widget->getProjection() == PERSPECTIVE )
        {
            double height = model->vertex[ vertexSize - 4 ].getPosition().z();
            QVector3D worldCoordinates = fromScreenToWorld( event, widget, true, height );
            model->vertex[ vertexSize - 2 ].setPosition( worldCoordinates );
            diagonal = QVector2D( model->vertex[ vertexSize - 2 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition() );
            if( square )
            {
                QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                diagonal = squareDiagonal * length;
                model->vertex[ vertexSize - 2 ].setPosition( model->vertex[ vertexSize - 4 ].getPosition() + QVector3D( diagonal, 0 ) );
            }
            posA = QVector3D( model->vertex[ vertexSize - 4 ].getPosition().x(), model->vertex[ vertexSize - 2 ].getPosition().y(), height );
            posB = QVector3D( model->vertex[ vertexSize - 2 ].getPosition().x(), model->vertex[ vertexSize - 4 ].getPosition().y(), height );
        }
        else
        {
            widget->countFinalInverseMatrix( false );
            QVector2D startPosition = QVector2D( widget->getFinalMatrix() * QVector4D( model->vertex[ vertexSize - 4 ].getPosition(), 1 ) );
            QVector2D lastPosition = widget->getLastPosition();
            QVector2D currentPosition = QVector2D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y() );
            diagonal = currentPosition - startPosition;

            model->vertex[ vertexSize - 2 ].setPosition( fromScreenToWorld_vector( currentPosition, widget ) );
            diagonal = currentPosition - startPosition;
            if( square )
            {
                QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                diagonal = squareDiagonal * length;
                currentPosition = startPosition + diagonal;
            }

            model->vertex[ vertexSize - 2 ].setPosition( fromScreenToWorld_vector( currentPosition, widget ) );
            posA = fromScreenToWorld_xy( startPosition.x(), currentPosition.y(), widget );
            posB = fromScreenToWorld_xy( currentPosition.x(), startPosition.y(), widget );
        }
        int a = ( diagonal.x() * diagonal.y() > 0 ) ? 1 : 3;
        model->vertex[ vertexSize - a ].setPosition( posA );
        model->vertex[ vertexSize - 4 + a ].setPosition( posB );
    }
    if( action == STOP )
    {
        vertexSize = model->vertexNumber;
        if( model->vertex[ vertexSize - 1 ] == model->vertex[
                vertexSize - 2 ] ||
            model->vertex[ vertexSize - 3 ] == model->vertex[
                vertexSize - 2 ] )
        {
            model->vertex.resize( vertexSize - 4 );
            model->vertexNumber -= 4;
            model->triangle.resize( model->triangle.size() - 2 );
            model->triangleNumber -= 2;
        }
        else
        {
            for( int i = 1; i < 5; i++ )
            {
                model->vertex[ vertexSize - i ].setNewSelected(
                            false );
                model->vertex[ vertexSize - i ].setSelected( true );
            }
        }
    }*/
}

void toolBoxFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data )
{
  /*  if( action == DRAW ) return;
    Model *model = widget->getModel();
    int vertexSize = model->vertexNumber;
    int triangleSize = model->triangleNumber;
    if( action == FINAL )
    {
        int i;
        for( i = 0; i < 3; i++ )
        {
            if( widget->getActiveTool()->getElements()->getSpinBox( i + 3 )->value() <= 0 )
            {
                return;
            }
        }
        model->vertex.resize( vertexSize + 8 );

        QVector3D center, halfDiagonal;
        for( i = 0; i < 3; i++ )
        {
            center[ i ] = widget->getActiveTool()->getElements()->getSpinBox( i )->value();
            halfDiagonal[ i ] = widget->getActiveTool()->getElements()->getSpinBox( i + 3 )->value();
        }
        halfDiagonal /= 2;

        model->vertex[ vertexSize     ].setPosition( center + QVector3D(-halfDiagonal.x(),-halfDiagonal.y(),-halfDiagonal.z() ) );
        model->vertex[ vertexSize + 1 ].setPosition( center + QVector3D(-halfDiagonal.x(), halfDiagonal.y(),-halfDiagonal.z() ) );
        model->vertex[ vertexSize + 2 ].setPosition( center + QVector3D( halfDiagonal.x(), halfDiagonal.y(),-halfDiagonal.z() ) );
        model->vertex[ vertexSize + 3 ].setPosition( center + QVector3D( halfDiagonal.x(),-halfDiagonal.y(),-halfDiagonal.z() ) );
        model->vertex[ vertexSize + 4 ].setPosition( center + QVector3D(-halfDiagonal.x(),-halfDiagonal.y(), halfDiagonal.z() ) );
        model->vertex[ vertexSize + 5 ].setPosition( center + QVector3D(-halfDiagonal.x(), halfDiagonal.y(), halfDiagonal.z() ) );
        model->vertex[ vertexSize + 6 ].setPosition( center + QVector3D( halfDiagonal.x(), halfDiagonal.y(), halfDiagonal.z() ) );
        model->vertex[ vertexSize + 7 ].setPosition( center + QVector3D( halfDiagonal.x(),-halfDiagonal.y(), halfDiagonal.z() ) );

        for( i = 0; i < 8; i++ ) model->vertex[ vertexSize + i ].setSelected( true );

        model->vertexNumber += 8;

        model->triangle.resize( triangleSize + 12 );

        model->triangle[ triangleSize      ].setIndices( vertexSize,     vertexSize + 1, vertexSize + 2 );
        model->triangle[ triangleSize + 1  ].setIndices( vertexSize,     vertexSize + 2, vertexSize + 3 );
        model->triangle[ triangleSize + 2  ].setIndices( vertexSize,     vertexSize + 5, vertexSize + 1 );
        model->triangle[ triangleSize + 3  ].setIndices( vertexSize,     vertexSize + 4, vertexSize + 5 );
        model->triangle[ triangleSize + 4  ].setIndices( vertexSize + 1, vertexSize + 6, vertexSize + 2 );
        model->triangle[ triangleSize + 5  ].setIndices( vertexSize + 1, vertexSize + 5, vertexSize + 6 );
        model->triangle[ triangleSize + 6  ].setIndices( vertexSize + 2, vertexSize + 7, vertexSize + 3 );
        model->triangle[ triangleSize + 7  ].setIndices( vertexSize + 2, vertexSize + 6, vertexSize + 7 );
        model->triangle[ triangleSize + 8  ].setIndices( vertexSize + 3, vertexSize + 4, vertexSize     );
        model->triangle[ triangleSize + 9  ].setIndices( vertexSize + 3, vertexSize + 7, vertexSize + 4 );
        model->triangle[ triangleSize + 10 ].setIndices( vertexSize + 4, vertexSize + 6, vertexSize + 5 );
        model->triangle[ triangleSize + 11 ].setIndices( vertexSize + 4, vertexSize + 7, vertexSize + 6 );

        model->triangleNumber += 12;
    }
    if( action == START )
    {
        int i;
        model->vertex.resize( vertexSize + 4 );
        model->vertexNumber += 4;

        model->triangle.resize( triangleSize + 2 );
        model->triangle[ triangleSize ].setIndices( vertexSize, vertexSize + 1, vertexSize + 2 );
        model->triangle[ triangleSize + 1 ].setIndices( vertexSize, vertexSize + 2, vertexSize + 3 );
        model->triangleNumber += 2;

        QVector3D worldCoordinates = fromScreenToWorld( event, widget );
        for( i = 0; i < 4; i++ ) model->vertex[ vertexSize + i ] = worldCoordinates;
        for( i = 0; i < 4; i++ ) model->vertex[ vertexSize + i ].setNewSelected( true );

    }
    if( action == EXECUTE )
    {
        if( widget->getActiveTool()->stage2() )
        {
            double dy = ( widget->getHalfHeight() - event->y() - widget->getLastPosition().y() ) / double( 100 );
            Camera *camera = widget->getCamera();
            QVector3D normal = ( widget->getProjection() == PERSPECTIVE ) ? QVector3D( 0, 0, 1 ) :
                   QVector3D( cos( inRadians( camera->rotation().x() ) ) * cos( inRadians( camera->rotation().z() ) ),
                              cos( inRadians( camera->rotation().x() ) ) * sin( inRadians( camera->rotation().z() ) ),
                              sin( inRadians( camera->rotation().x() ) ) );

            if( widget->getActiveTool()->getElements()->getCheckBox( 1 )->isChecked() && dy != 0 )
            {
                QVector3D dh = normal *
               sign( dy ) * ( model->vertex[ vertexSize - 2 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition() ).length() / qSqrt( 2 );
                for( int i = -4; i < 0; i++ ) model->vertex[ vertexSize + i ].setPosition( model->vertex[ vertexSize - 4 + i ].getPosition() + dh );
            }
            else for( int i = -4; i < 0; i++ ) model->vertex[ vertexSize + i ].addToPosition( normal * dy );

            QVector3D diagonal = model->vertex[ vertexSize - 2 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition();
            QVector3D ey = model->vertex[ vertexSize - 3 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition();
            QVector3D ex = model->vertex[ vertexSize - 1 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition();

            if( QVector3D::dotProduct( normal, model->vertex[ vertexSize - 8 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition() ) *
                    QVector3D::dotProduct( normal, QVector3D::crossProduct( ex, diagonal ) ) > 0 )
            {
                QVector3D temp = model->vertex[ vertexSize - 7 ].getPosition();
                model->vertex[ vertexSize - 7 ] = model->vertex[ vertexSize - 5 ];
                model->vertex[ vertexSize - 5 ].setPosition( temp );
                temp = model->vertex[ vertexSize - 3 ].getPosition();
                model->vertex[ vertexSize - 3 ] = model->vertex[ vertexSize - 1 ];
                model->vertex[ vertexSize - 1 ].setPosition( temp );
            }
        }
        else
        {
            QVector2D diagonal;
            QVector3D posA, posB;
            bool square = widget->getActiveTool()->getElements()->getCheckBox( 0 )->isChecked() ||
                    widget->getActiveTool()->getElements()->getCheckBox( 1 )->isChecked();
            if( widget->getProjection() == PERSPECTIVE )
            {
                double height = model->vertex[ vertexSize - 4 ].getPosition().z();
                QVector3D worldCoordinates = fromScreenToWorld( event, widget, true, height );
                model->vertex[ vertexSize - 2 ].setPosition( worldCoordinates );
                diagonal = QVector2D( model->vertex[ vertexSize - 2 ].getPosition() - model->vertex[ vertexSize - 4 ].getPosition() );
                if( square )
                {
                    QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                    double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                    diagonal = squareDiagonal * length;
                    model->vertex[ vertexSize - 2 ].setPosition( model->vertex[ vertexSize - 4 ].getPosition() + QVector3D( diagonal, 0 ) );
                }
                posB = QVector3D( model->vertex[ vertexSize - 4 ].getPosition().x(), model->vertex[ vertexSize - 2 ].getPosition().y(), height );
                posA = QVector3D( model->vertex[ vertexSize - 2 ].getPosition().x(), model->vertex[ vertexSize - 4 ].getPosition().y(), height );
            }
            else
            {
                widget->countFinalInverseMatrix( false );
                QVector2D startPosition = QVector2D( widget->getFinalMatrix() * QVector4D( model->vertex[ vertexSize - 4 ].getPosition(), 1 ) );
                QVector2D lastPosition = widget->getLastPosition();
                QVector2D currentPosition = QVector2D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y() );
                diagonal = currentPosition - startPosition;
                if( square )
                {
                    QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                    double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                    diagonal = squareDiagonal * length;
                    currentPosition = startPosition + diagonal;
                }
                model->vertex[ vertexSize - 2 ].setPosition( fromScreenToWorld_vector( currentPosition, widget ) );
                posB = fromScreenToWorld_xy( startPosition.x(), currentPosition.y(), widget );
                posA = fromScreenToWorld_xy( currentPosition.x(), startPosition.y(), widget );
            }
            int a = ( diagonal.x() * diagonal.y() > 0 ) ? 1 : 3;
            model->vertex[ vertexSize - a ].setPosition( posA );
            model->vertex[ vertexSize - 4 + a ].setPosition( posB );
        }
    }
    if( action == STOP )
    {
        vertexSize = model->vertexNumber;
        if( model->vertex[ vertexSize - 1 ] == model->vertex[ vertexSize - 5 ] )
        {
            model->vertex.resize( vertexSize - 8 );
            model->vertexNumber -= 8;
            model->triangle.resize( model->triangle.size() - 12 );
            model->triangleNumber -= 12;
        }
        else
        {
            for( int i = 1; i < 9; i++ )
            {
                model->vertex[ vertexSize - i ].setNewSelected( false );
                model->vertex[ vertexSize - i ].setSelected( true );
            }
        }
        widget->setToolIsOn( false );
        widget->getActiveTool()->setStage2( false );
        widget->setMouseTracking( false );
    }
    if( action == STAGE2 )
    {
        if( model->vertex[ vertexSize - 1 ] == model->vertex[
                vertexSize - 2 ] ||
            model->vertex[ vertexSize - 3 ] == model->vertex[
                vertexSize - 2 ] )
        {
            model->vertex.resize( vertexSize - 4 );
            model->vertexNumber -= 4;
            model->triangle.resize( model->triangle.size() - 2 );
            model->triangleNumber -= 2;
            return;
        }
        widget->setToolIsOn( true );
        widget->getActiveTool()->setStage2( true );
        int i;
        model->vertex.resize( vertexSize + 4 );
        model->vertexNumber += 4;

        model->triangle.resize( triangleSize + 10 );

        model->triangle[ triangleSize ].setIndices( vertexSize - 4, vertexSize + 1, vertexSize - 3 );
        model->triangle[ triangleSize + 1 ].setIndices( vertexSize - 4, vertexSize, vertexSize + 1 );
        model->triangle[ triangleSize + 2 ].setIndices( vertexSize - 3, vertexSize + 2, vertexSize - 2 );
        model->triangle[ triangleSize + 3 ].setIndices( vertexSize - 3, vertexSize + 1, vertexSize + 2 );
        model->triangle[ triangleSize + 4 ].setIndices( vertexSize - 2, vertexSize + 3, vertexSize - 1 );
        model->triangle[ triangleSize + 5 ].setIndices( vertexSize - 2, vertexSize + 2, vertexSize + 3 );
        model->triangle[ triangleSize + 6 ].setIndices( vertexSize - 1, vertexSize, vertexSize - 4 );
        model->triangle[ triangleSize + 7 ].setIndices( vertexSize - 1, vertexSize + 3, vertexSize );
        model->triangle[ triangleSize + 8 ].setIndices( vertexSize, vertexSize + 2, vertexSize + 1 );
        model->triangle[ triangleSize + 9 ].setIndices( vertexSize, vertexSize + 3, vertexSize + 2 );

        model->triangleNumber += 10;

        for( i = 0; i < 4; i++ )
        {
            model->vertex[ vertexSize + i ] =
                    model->vertex[ vertexSize - 4 + i ].getPosition();// + QVector3D( 0, 0, 1 );
            model->vertex[ vertexSize + i ].setNewSelected( true );
        }
        widget->setMouseTracking( true );
    }*/
}

void toolEllipseFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data )
{
 /*   if( action == DRAW ) return;
    Model *model = widget->getModel();
    int vertexSize = model->vertexNumber;
    int triangleSize = model->triangleNumber;
    int segments = widget->getActiveTool()->getElements()->getSpinBox( 0 )->value();
    if( action == START || action == FINAL )
    {
        int i;
        model->vertex.resize( vertexSize + segments + 1 );
        model->vertexNumber += segments + 1;

        model->triangle.resize( triangleSize + segments );

        for( i = 0; i < segments - 1; i++ )
            model->triangle[ triangleSize + i ].setIndices( vertexSize + i, vertexSize + i + 1, vertexSize + segments );
        model->triangle[ triangleSize + i ].setIndices( vertexSize + segments - 1, vertexSize, vertexSize + segments );

        model->triangleNumber += segments;

        if( action == START )
        {
            QVector3D worldCoordinates = fromScreenToWorld( event, widget );
            widget->setStartPosition3D( worldCoordinates );
            for( i = 0; i <= segments; i++ )
            {
                model->vertex[ vertexSize + i ] = worldCoordinates;
                model->vertex[ vertexSize + i ].setNewSelected( true );
            }
        }
        else
        {
            QVector3D normal;
            for( i = 0; i < 3; i++ ) normal[ i ] = widget->getActiveTool()->getElements()->getSpinBox( 4 + i )->value();
            if( normal.length() == 0 ) return;
            normal.normalize();
            double radius = widget->getActiveTool()->getElements()->getSpinBox( 7 )->value();
            if( radius == 0 ) return;
            QVector3D center;
            for( i = 0; i < 3; i++ ) center[ i ] = widget->getActiveTool()->getElements()->getSpinBox( 1 + i )->value();

            widget->countFinalInverseMatrix( false );//?
            model->vertex[ vertexSize + segments ].setPosition( center );
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate( center );
            scaleAndTranslate.scale( radius, radius, radius );
            double angle = 360 / double( segments );
            //if( projection == TOP || projection == BOTTOM ) angle *= -1;
            QMatrix4x4 rotation;
            rotation.setToIdentity();
            QVector4D firstVertex;//( !front, front, 0.f, 1.f );
            firstVertex = ( normal.x() == 1 ) ? QVector4D( 0.f, 1.f, 0.f, 1.f ) :
                                                QVector4D( QVector3D::crossProduct( QVector3D( 1.f, 0.f, 0.f ), normal ).normalized(), 1.f );
            for( i = 0; i < segments; i++ )
            {
                rotation.rotate( angle, normal );
                model->vertex[ vertexSize + i ].setPosition( QVector3D( scaleAndTranslate * rotation * firstVertex ) );
            }
            for( i = 0; i <= segments; i++ ) model->vertex[ vertexSize + i ].setSelected( true );
        }   
    }
    if( action == EXECUTE )
    {
        Projection projection = widget->getProjection();
        bool circle = widget->getActiveTool()->getElements()->getCheckBox( 0 )->isChecked();
        if( projection == PERSPECTIVE )
        {
            QVector3D start = widget->startPosition3D();
            double height = start.z();
            QVector3D worldCoordinates = fromScreenToWorld( event, widget, true, height );
            QVector2D radius = QVector2D( worldCoordinates - start ) / double( 2 );
            QVector3D center = start + radius;
            model->vertex[ vertexSize - 1 ].setPosition( center );
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate( center );
            if( circle )
            {
                double length = radius.length();
                scaleAndTranslate.scale( length, length, 1.f );
            }
            else scaleAndTranslate.scale( radius.x(), radius.y(), 1.f );

            double angle = 360 / double( segments );
            if( !circle) angle *= sign( radius.x() * radius.y() );
            createEllipseCap( { 1.f, 0.f, 0.f, 1.f }, angle, { 0.f, 0.f, 1.f }, model, vertexSize, segments, scaleAndTranslate );
        }
        else
        {
            widget->countFinalInverseMatrix( false );
            QVector3D start = widget->startPosition3D();
            QVector2D currentPosition = QVector2D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y() );
            QVector3D worldCoordinates = fromScreenToWorld_vector( currentPosition, widget );
            QVector3D radius = ( worldCoordinates - start ) / double( 2 );
            QVector3D center = start + radius;
            model->vertex[ vertexSize - 1 ].setPosition( center );
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate( center );

            QVector3D cameraRotation = widget->getCamera()->rotation();
            QVector3D normal = ( widget->getProjection() == PERSPECTIVE ) ? QVector3D( 0, 0, 1 ) :
                   QVector3D( cos( inRadians( cameraRotation.x() ) ) * cos( inRadians( cameraRotation.z() ) ),
                              cos( inRadians( cameraRotation.x() ) ) * sin( inRadians( cameraRotation.z() ) ),
                              sin( inRadians( cameraRotation.x() ) ) );
            int i;

            if( circle )
            {
                double length = radius.length();
                scaleAndTranslate.scale( length, length, length );
            }
            else
            {
                for( i = 0; i < 3; i++ ) if( qAbs( normal[ i ] ) > 0.01 ) radius[ i ] = sign( radius[ i ] );
                scaleAndTranslate.scale( radius.x(), radius.y(), radius.z() );
            }
            double angle = 360 / double( segments ) * sign( radius.x() * radius.y() * radius.z() );
            if( projection == TOP || projection == BOTTOM ) angle *= -1;
            QMatrix4x4 rotation;
            rotation.setToIdentity();
            bool front = projection == FRONT || projection == BACK;
            createEllipseCap( { !front, front, 0.f, 1.f }, angle, normal, model, vertexSize, segments, scaleAndTranslate );
        }
    }
    if( action == STOP )
    {
        vertexSize = model->vertexNumber;
        if( QVector3D::crossProduct( model->vertex[ vertexSize - 1 ].getPosition(),
                                     model->vertex[ vertexSize - 2 ].getPosition() ).length() == 0 )
        {
            model->vertex.resize( vertexSize - segments - 1 );
            model->vertexNumber -= segments + 1;
            model->triangle.resize( model->triangle.size() - segments );
            model->triangleNumber -= segments;
        }
        else
        {
            for( int i = 1; i < segments + 2; i++ )
            {
                model->vertex[ vertexSize - i ].setNewSelected( false );
                model->vertex[ vertexSize - i ].setSelected( true );
            }
        }
    }*/
}

void toolCylinderFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data )
{
 /*   if( action == DRAW ) return;
    Model *model = widget->getModel();
    int vertexSize = model->vertexNumber;
    int triangleSize = model->triangleNumber;
    int segments = widget->getActiveTool()->getElements()->getSpinBox( 0 )->value();
    if( action == START || action == FINAL )
    {
        int i;
        bool start = ( action == START );
        int newVertices = start ? segments + 1 : 2 * segments + 2;
        model->vertex.resize( vertexSize + newVertices );
        model->vertexNumber += newVertices;

        int newTriangles = start ? segments : 4 * segments;
        model->triangle.resize( triangleSize + newTriangles );

        for( i = 0; i < segments - 1; i++ )
            model->triangle[ triangleSize + i ].setIndices( vertexSize + i + 1, vertexSize + i, vertexSize + segments );
        model->triangle[ triangleSize + i ].setIndices( vertexSize, vertexSize + segments - 1, vertexSize + segments );

        model->triangleNumber += newTriangles;

        if( action == START )
        {
            QVector3D worldCoordinates = fromScreenToWorld( event, widget );
            widget->setStartPosition3D( worldCoordinates );
            for( i = 0; i <= segments; i++ )
            {
                model->vertex[ vertexSize + i ] = worldCoordinates;
                model->vertex[ vertexSize + i ].setNewSelected( true );
            }
        }
        else
        {
            QVector3D normal;
            for( i = 0; i < 3; i++ ) normal[ i ] = widget->getActiveTool()->getElements()->getSpinBox( 4 + i )->value();
            if( normal.length() == 0 ) return;
            normal.normalize();
            double radius = widget->getActiveTool()->getElements()->getSpinBox( 7 )->value();
            if( radius == 0 ) return;
            QVector3D center;
            for( i = 0; i < 3; i++ ) center[ i ] = widget->getActiveTool()->getElements()->getSpinBox( 1 + i )->value();

            widget->countFinalInverseMatrix( false );//?
            model->vertex[ vertexSize + segments         ].setPosition( center          );
            model->vertex[ vertexSize + 2 * segments + 1 ].setPosition( center + normal );
            QMatrix4x4 scaleAndTranslate;
            scaleAndTranslate.setToIdentity();
            scaleAndTranslate.translate( center );
            scaleAndTranslate.scale( radius, radius, radius );
            double angle = 360 / double( segments );

            QMatrix4x4 rotation;
            rotation.setToIdentity();
            rotation.rotate( angle, normal );
            QVector4D rotatingVertex;
            rotatingVertex = ( normal.x() == 1 ) ? QVector4D( 0.f, 1.f, 0.f, 1.f ) :
                                                QVector4D( QVector3D::crossProduct( QVector3D( 1.f, 0.f, 0.f ), normal ).normalized(), 1.f );
            QVector3D currentVertex;
            for( i = 0; i < segments; i++ )
            {
                rotatingVertex = rotation * rotatingVertex;
                currentVertex = QVector3D( scaleAndTranslate * rotatingVertex );
                model->vertex[ vertexSize + i                ].setPosition( currentVertex );
                model->vertex[ vertexSize + segments + 1 + i ].setPosition( currentVertex + normal );
            }
            for( i = 0; i < 2 * segments + 2; i++ ) model->vertex[ vertexSize + i ].setSelected( true );
            for( i = 0; i < segments - 1; i++ )
            {
                model->triangle[ triangleSize + segments + i ].setIndices( vertexSize + segments + 1 + i, vertexSize + segments + 1 + i + 1,
                                                                               vertexSize + segments + 1 + segments );
                model->triangle[ triangleSize + 2 * segments + 2 * i     ].setIndices( vertexSize + segments + 1 + i + 1, vertexSize + segments + 1 + i,
                                                                                       vertexSize + i );
                model->triangle[ triangleSize + 2 * segments + 2 * i + 1 ].setIndices( vertexSize + i, vertexSize + 1 + i, vertexSize  + segments + 1 + i + 1 );
            }
            model->triangle[ triangleSize + segments + i ].setIndices( vertexSize + segments + 1 + segments - 1, vertexSize + segments + 1,
                                                                       vertexSize + segments + 1 + segments );
            model->triangle[ triangleSize + 2 * segments + 2 * i     ].setIndices( vertexSize + segments + 1, vertexSize + segments - 1 + segments + 1,
                                                                                   vertexSize + segments + 1 - 2 );
            model->triangle[ triangleSize + 2 * segments + 2 * i + 1 ].setIndices( vertexSize + segments + 1 - 2, vertexSize, vertexSize + segments + 1 );

        }
    }
    if( action == EXECUTE )
    {
        if( widget->getActiveTool()->stage2() )
        {
            double dy = ( widget->getHalfHeight() - event->y() - widget->getLastPosition().y() ) / double( 100 );
            Camera *camera = widget->getCamera();
            QVector3D normal = ( widget->getProjection() == PERSPECTIVE ) ? QVector3D( 0, 0, 1 ) :
                   QVector3D( cos( inRadians( camera->rotation().x() ) ) * cos( inRadians( camera->rotation().z() ) ),
                              cos( inRadians( camera->rotation().x() ) ) * sin( inRadians( camera->rotation().z() ) ),
                              sin( inRadians( camera->rotation().x() ) ) );

            int i;
            for( i = -segments - 1; i < 0; i++ ) model->vertex[ vertexSize + i ].addToPosition( normal * dy );


            QVector3D v1 = model->vertex[ vertexSize - segments - 1 ].getPosition() - model->vertex[ vertexSize - 1 ].getPosition();
            QVector3D v2 = model->vertex[ vertexSize - segments ].getPosition() - model->vertex[ vertexSize - 1 ].getPosition();

            if( QVector3D::dotProduct( normal, model->vertex[ vertexSize - segments - 2 ].getPosition() - model->vertex[ vertexSize - 1 ].getPosition() ) *
                    QVector3D::dotProduct( normal, QVector3D::crossProduct( v1, v2 ) ) > 0 )
            {
                QVector3D temp;
                for( i = 0; i < segments / 2; i++ )
                {
                    temp = model->vertex[ vertexSize - segments - 1 + i ].getPosition();
                    model->vertex[ vertexSize - segments - 1 + i ] = model->vertex[ vertexSize - 2 - i ];
                    model->vertex[ vertexSize - 2 - i ].setPosition( temp );
                    temp = model->vertex[ vertexSize - 2 * segments - 2 + i ].getPosition();
                    model->vertex[ vertexSize - 2 * segments - 2 + i ] = model->vertex[ vertexSize - segments - 3 - i ];
                    model->vertex[ vertexSize - segments - 3 - i ].setPosition( temp );
                }
            }
        }
        else
        {
            Projection projection = widget->getProjection();
            bool circle = widget->getActiveTool()->getElements()->getCheckBox( 0 )->isChecked();
            if( projection == PERSPECTIVE )
            {
                QVector3D start = widget->startPosition3D();
                double height = start.z();
                QVector3D worldCoordinates = fromScreenToWorld( event, widget, true, height );
                QVector2D radius = QVector2D( worldCoordinates - start ) / double( 2 );
                QVector3D center = start + radius;
                model->vertex[ vertexSize - 1 ].setPosition( center );
                QMatrix4x4 scaleAndTranslate;
                scaleAndTranslate.setToIdentity();
                scaleAndTranslate.translate( center );
                if( circle )
                {
                    double length = radius.length();
                    scaleAndTranslate.scale( length, length, 1.f );
                }
                else scaleAndTranslate.scale( radius.x(), radius.y(), 1.f );

                double angle = 360 / double( segments );
                if( !circle) angle *= sign( radius.x() * radius.y() );
                createEllipseCap( { 1.f, 0.f, 0.f, 1.f }, angle, { 0.f, 0.f, 1.f }, model, vertexSize, segments, scaleAndTranslate );
            }
            else
            {
                widget->countFinalInverseMatrix( false );
                QVector3D start = widget->startPosition3D();
                QVector2D currentPosition = QVector2D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y() );
                QVector3D worldCoordinates = fromScreenToWorld_vector( currentPosition, widget );
                QVector3D radius = ( worldCoordinates - start ) / double( 2 );
                QVector3D center = start + radius;
                model->vertex[ vertexSize - 1 ].setPosition( center );
                QMatrix4x4 scaleAndTranslate;
                scaleAndTranslate.setToIdentity();
                scaleAndTranslate.translate( center );

                QVector3D cameraRotation = widget->getCamera()->rotation();
                QVector3D normal = ( widget->getProjection() == PERSPECTIVE ) ? QVector3D( 0, 0, 1 ) :
                       QVector3D( cos( inRadians( cameraRotation.x() ) ) * cos( inRadians( cameraRotation.z() ) ),
                                  cos( inRadians( cameraRotation.x() ) ) * sin( inRadians( cameraRotation.z() ) ),
                                  sin( inRadians( cameraRotation.x() ) ) );
                int i;

                if( circle )
                {
                    double length = radius.length();
                    scaleAndTranslate.scale( length, length, length );
                }
                else
                {
                    for( i = 0; i < 3; i++ ) if( qAbs( normal[ i ] ) > 0.01 ) radius[ i ] = sign( radius[ i ] );
                    scaleAndTranslate.scale( radius.x(), radius.y(), radius.z() );
                }
                double angle = 360 / double( segments ) * sign( radius.x() * radius.y() * radius.z() );
                if( projection == TOP || projection == BOTTOM ) angle *= -1;
                bool front = projection == FRONT || projection == BACK;
                createEllipseCap( { !front, front, 0.f, 1.f }, angle, normal, model, vertexSize, segments, scaleAndTranslate );
            }
        }
    }
    if( action == STOP )
    {
        vertexSize = model->vertexNumber;
        if( model->vertex[ vertexSize - 1 ] == model->vertex[ vertexSize - segments - 2 ] )
        {
            model->vertex.resize( vertexSize - 2 * segments - 2 );
            model->vertexNumber -= 2 * segments + 2;
            model->triangle.resize( model->triangle.size() - 4 * segments );
            model->triangleNumber -= 4 * segments;
        }
        else
        {
            for( int i = 1; i < 2 * segments + 3; i++ )
            {
                model->vertex[ vertexSize - i ].setNewSelected( false );
                model->vertex[ vertexSize - i ].setSelected( true );
            }
        }
        widget->setToolIsOn( false );
        widget->getActiveTool()->setStage2( false );
        widget->setMouseTracking( false );
    }
    if( action == STAGE2 )
    {
        if( QVector3D::crossProduct( model->vertex[ vertexSize - 1 ].getPosition(),
                                     model->vertex[ vertexSize - 2 ].getPosition() ).length() == 0 )
        {
            model->vertex.resize( vertexSize - segments - 1 );
            model->vertexNumber -= segments + 1;
            model->triangle.resize( model->triangle.size() - segments );
            model->triangleNumber -= segments;
            return;
        }
        widget->setToolIsOn( true );
        widget->getActiveTool()->setStage2( true );
        int i;
        model->vertex.resize( vertexSize + segments + 1 );
        for( i = 0; i <= segments; i++ ) model->vertex[ vertexSize + i ] = model->vertex[ vertexSize - segments - 1 + i ];

        model->vertexNumber += segments + 1;

        model->triangle.resize( triangleSize + 3 * segments );
        for( i = 0; i < segments - 1; i++ )
        {
            model->triangle[ triangleSize + i ].setIndices( vertexSize + i, vertexSize + i + 1, vertexSize + segments );
            model->triangle[ triangleSize + segments + 2 * i     ].setIndices( vertexSize + i + 1, vertexSize + i, vertexSize - segments - 1 + i );
            model->triangle[ triangleSize + segments + 2 * i + 1 ].setIndices( vertexSize - segments - 1 + i, vertexSize - segments + i, vertexSize + i + 1 );
        }
        model->triangle[ triangleSize + i ].setIndices( vertexSize + segments - 1, vertexSize, vertexSize + segments );
        model->triangle[ triangleSize + segments + 2 * i     ].setIndices( vertexSize, vertexSize + segments - 1, vertexSize - 2 );
        model->triangle[ triangleSize + segments + 2 * i + 1 ].setIndices( vertexSize - 2, vertexSize - segments - 1, vertexSize );

        model->triangleNumber += 3 * segments;

        widget->setMouseTracking( true );
    }*/
}

bool isSelected( QMatrix4x4 finalMatrix, QVector3D vertex,
                 bool perspective, QVector2D min, QVector2D max )
{
    QVector4D result = finalMatrix * QVector4D( vertex, 1 );
    if( perspective )
    {
        for( int j = 0; j < 2; j++ )
            result[ j ] /= result[ 3 ];
    }
    if( result.x() > min.x() && result.x() < max.x() &&
        result.y() > min.y() && result.y() < max.y() )
        return true;
    return false;
}

bool getAxis( WidgetElements *toolElements, QMatrix4x4 *rotation,
              double angle )
{
    QVector3D axis;
    for( int i = 0; i < 3; i++ ) axis[ i ] = toolElements->
            getSpinBox( i + 1 )->value();
    if( axis.isNull() ) return false;
    axis.normalize();
    rotation->rotate( angle, axis );
    return true;
}

QVector3D fromScreenToWorld_xy( double x, double y, GLWidget *widget )
{
    return _fromScreenToWorld( QVector4D( x, y, 0, 1 ), widget );
}

QVector3D fromScreenToWorld_vector( QVector2D vector, GLWidget *widget )
{
    return _fromScreenToWorld( QVector4D( vector.x(), vector.y(), 0, 1 ), widget );
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

void createEllipseCap( QVector4D rotatingVertex, double angle, QVector3D normal, Model *model, int vertexSize, int segments,
                       QMatrix4x4 scaleAndTranslate )
{
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate( angle, normal );
    for( int i = 0; i < segments; i++ )
    {
        rotatingVertex = rotation * rotatingVertex;
        model->vertex[ vertexSize - segments - 1 + i ].setPosition( QVector3D( scaleAndTranslate * rotatingVertex ) );
    }
}
