#include "tellipse.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TEllipse::TEllipse(MainWindow *mainWindow) : ToolWithWidget(
                                                 mainWindow)
{
    button->setText("Ellipse");
    finalButton = new QPushButton("Create Ellipse");

    elements = new WidgetElements( 0, 8, 0, 0, 1 );
    elements->getSpinBox( 0 )->setMaximumWidth( 50 );
    elements->getSpinBox( 0 )->setMinimum( 3 );
    elements->getSpinBox( 0 )->setValue( 18 );
    QLabel *toolEllipseSegments = new QLabel( "Segments:" );
    toolEllipseSegments->setAlignment( Qt::AlignRight );
    toolEllipseSegments->setMaximumWidth( 70 );

    elements->getCheckBox( 0 )->setText( "Circle" );
    elements->getCheckBox( 0 )->setMaximumWidth( 130 );

    QLabel *toolEllipseCenter = new QLabel( "Center" );
    QLabel *toolEllipseNormal = new QLabel( "Normal" );
    QLabel *toolEllipseLabels[ 6 ];
    for(int i = 0; i < 6; i++ )
    {
        toolEllipseLabels[ i ] = new QLabel( QString( 'X' + i % 3 ) + ':' );
        toolEllipseLabels[ i ]->setMaximumWidth( 25 );
        toolEllipseLabels[ i ]->setAlignment( Qt::AlignRight );
        elements->getSpinBox( 1 + i )->setMaximumWidth( 50 );
    }
    QLabel *toolEllipseRadius = new QLabel( "Radius:" );
    toolEllipseRadius->setAlignment( Qt::AlignRight );
    toolEllipseRadius->setMaximumWidth( 70 );

    elements->getSpinBox( 7 )->setMinimum( 0 );
    elements->getSpinBox( 7 )->setValue( 1 );

    layout->addWidget( toolEllipseSegments, 0, 0, 1, 2 );
    layout->addWidget( elements->getSpinBox( 0 ), 0, 2, 1, 2 );
    layout->addWidget( elements->getCheckBox( 0 ), 1, 0, 1, 4 );
    layout->addWidget( toolEllipseCenter, 2, 0, 1, 4 );
    layout->addWidget( toolEllipseNormal, 2, 2, 1, 4 );
    for(int i = 0; i < 3; i++ )
    {
        layout->addWidget( toolEllipseLabels[ i ], 3 + i, 0 );
        layout->addWidget( elements->getSpinBox( 1 + i ), 3 + i, 1 );
        layout->addWidget( toolEllipseLabels[ i + 3 ], 3 + i, 2 );
        layout->addWidget( elements->getSpinBox( 4 + i ), 3 + i, 3 );
    }
    layout->addWidget( toolEllipseRadius, 6, 0, 1, 2 );
    layout->addWidget( elements->getSpinBox( 7 ), 6, 2, 1, 2 );
    layout->addWidget(finalButton, 7, 0, 1, 4 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT( final() ) );
    _widget->hide();
}

void TEllipse::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
        int vertexSize = model->vertexNumber;
        int triangleSize = model->triangleNumber;
        int segments = elements->getSpinBox( 0 )->value();
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
                for( i = 0; i < 3; i++ ) normal[ i ] = elements->getSpinBox( 4 + i )->value();
                if( normal.length() == 0 ) return;
                normal.normalize();
                double radius = elements->getSpinBox( 7 )->value();
                if( radius == 0 ) return;
                QVector3D center;
                for( i = 0; i < 3; i++ ) center[ i ] = elements->getSpinBox( 1 + i )->value();

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
            bool circle = elements->getCheckBox( 0 )->isChecked();
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
        }
}

