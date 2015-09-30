#include "tbox.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TBox::TBox(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Box");
    finalButton = new QPushButton("Create Box");

    elements = new WidgetElements( 0, 6, 0, 0, 2 );
    elements->getCheckBox( 0 )->setText( "Square" );
    elements->getCheckBox( 0 )->setMaximumWidth( 130 );
    elements->getCheckBox( 1 )->setText( "Cube" );
    elements->getCheckBox( 1 )->setMaximumWidth( 130 );
    QLabel *toolBoxCenter = new QLabel( "Center" );
    QLabel *toolBoxSize = new QLabel( "Size" );
    QLabel *toolBoxLabels[ 6 ];
    for(int i = 0; i < 6; i++ )
    {
        toolBoxLabels[ i ] = new QLabel( QString( 'X' + i % 3 )
                                           + ':' );
        toolBoxLabels[ i ]->setMaximumWidth( 25 );
        toolBoxLabels[ i ]->setAlignment( Qt::AlignRight );
        elements->getSpinBox( i )->setMaximumWidth( 50 );
    }
    layout->addWidget( elements->getCheckBox( 0 ), 0, 0, 1, 4 );
    layout->addWidget( elements->getCheckBox( 1 ), 1, 0, 1, 4 );
    layout->addWidget( toolBoxCenter, 2, 0, 1, 4 );
    layout->addWidget( toolBoxSize, 2, 2, 1, 4 );
    for(int i = 0; i < 3; i++ )
    {
        layout->addWidget( toolBoxLabels[ i ], 3 + i, 0 );
        layout->addWidget( elements->getSpinBox( i ), 3 + i, 1 );
        layout->addWidget( toolBoxLabels[ i + 3 ], 3 + i, 2 );
        layout->addWidget( elements->getSpinBox( i + 3 ), 3 + i, 3 );
    }
    layout->addWidget(finalButton, 6, 0, 1, 4 );

    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT(
                final() ) );
    _widget->hide();

    _hasStage2 = true;
}

void TBox::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
        int vertexSize = model->vertexNumber;
        int triangleSize = model->triangleNumber;
        if( action == FINAL )
        {
            int i;
            for( i = 0; i < 3; i++ )
            {
                if(elements->getSpinBox( i + 3 )->value() <= 0 )
                {
                    return;
                }
            }
            model->vertex.resize( vertexSize + 8 );

            QVector3D center, halfDiagonal;
            for( i = 0; i < 3; i++ )
            {
                center[ i ] = elements->getSpinBox( i )->value();
                halfDiagonal[ i ] = elements->getSpinBox( i + 3 )->value();
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
            if(_stage2)
            {
                double dy = ( widget->getHalfHeight() - event->y() - widget->getLastPosition().y() ) / double( 100 );
                Camera *camera = widget->getCamera();
                QVector3D normal = ( widget->getProjection() == PERSPECTIVE ) ? QVector3D( 0, 0, 1 ) :
                       QVector3D( cos( inRadians( camera->rotation().x() ) ) * cos( inRadians( camera->rotation().z() ) ),
                                  cos( inRadians( camera->rotation().x() ) ) * sin( inRadians( camera->rotation().z() ) ),
                                  sin( inRadians( camera->rotation().x() ) ) );

                if( elements->getCheckBox( 1 )->isChecked() && dy != 0 )
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
                bool square = elements->getCheckBox( 0 )->isChecked() ||
                        elements->getCheckBox( 1 )->isChecked();
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
            setStage2( false );
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
            setStage2( true );
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
        }
}
