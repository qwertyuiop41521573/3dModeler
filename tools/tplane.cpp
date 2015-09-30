#include "tplane.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TPlane::TPlane(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Plane");
    finalButton = new QPushButton("Create Plane");

    elements = new WidgetElements( 0, 12, 0, 0, 1 );
    elements->getCheckBox( 0 )->setText( "Square" );
    elements->getCheckBox( 0 )->setMaximumWidth( 130 );
    layout->addWidget( elements->getCheckBox( 0 ), 0, 0, 1, 6 );
    QLabel *toolPlaneLabels[ 12 ];
    for(int i = 0; i < 12; i++ )
    {
        toolPlaneLabels[ i ] = new QLabel( QString( 'X' + i % 3 )
                                           + ':' );
        toolPlaneLabels[ i ]->setMaximumWidth( 25 );
        toolPlaneLabels[ i ]->setAlignment( Qt::AlignRight );
        int x = 4 * ( ( i % 6 ) / 3 ), y = 4 * ( i / 6 ) + i % 3 + 1;
        layout->addWidget( toolPlaneLabels[ i ], y, x );
        elements->getSpinBox( i )->setMaximumWidth( 50 );
        layout->addWidget( elements->getSpinBox( i ), y, x + 1 );
    }
    QFrame *toolPlaneHLine = new QFrame();
    toolPlaneHLine->setFrameShape( QFrame::HLine );
    toolPlaneHLine->setFrameShadow( QFrame::Sunken );
    toolPlaneHLine->setMaximumWidth( 150 );
    toolPlaneHLine->setFixedHeight( 5 );
    layout->addWidget( toolPlaneHLine, 4, 0, 1, 6 );
    QFrame *toolPlaneVLine = new QFrame();
    toolPlaneVLine->setFrameShape( QFrame::VLine );
    toolPlaneVLine->setFrameShadow( QFrame::Sunken );
    toolPlaneVLine->setMaximumHeight( 200 );
    toolPlaneVLine->setFixedWidth( 5 );
    layout->addWidget( toolPlaneVLine, 1, 3, 7, 1 );
    layout->addWidget(finalButton, 9,
                                0, 1, 6 );

    connect(finalButton, SIGNAL( clicked() ),
             _mainWindow, SLOT( final() ) );
    //_widget->hide();
}

void TPlane::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
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
                WidgetElements *elements = elements;
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
            bool square = elements->getCheckBox( 0 )->isChecked();
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
        }
}
