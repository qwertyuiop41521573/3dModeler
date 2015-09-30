#include "tcylinder.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TCylinder::TCylinder(MainWindow *mainWindow) : ToolWithWidget(
                                                   mainWindow)
{
    button->setText("Cylinder");
    finalButton = new QPushButton("Create Cylinder");

    elements = new WidgetElements( 0, 8, 0, 0, 1 );
    elements->getSpinBox( 0 )->setMaximumWidth( 50 );
    elements->getSpinBox( 0 )->setMinimum( 3 );
    elements->getSpinBox( 0 )->setValue( 18 );
    QLabel *toolCylinderSegments = new QLabel( "Segments:" );
    toolCylinderSegments->setAlignment( Qt::AlignRight );
    toolCylinderSegments->setMaximumWidth( 70 );

    elements->getCheckBox( 0 )->setText( "Circle" );
    elements->getCheckBox( 0 )->setMaximumWidth( 130 );

    QLabel *toolCylinderCenter = new QLabel( "Center" );
    QLabel *toolCylinderNormal = new QLabel( "Normal" );
    QLabel *toolCylinderLabels[ 6 ];
    for(int i = 0; i < 6; i++ )
    {
        toolCylinderLabels[ i ] = new QLabel( QString( 'X' + i % 3 ) + ':' );
        toolCylinderLabels[ i ]->setMaximumWidth( 25 );
        toolCylinderLabels[ i ]->setAlignment( Qt::AlignRight );
        elements->getSpinBox( 1 + i )->setMaximumWidth( 50 );
    }
    QLabel *toolCylinderRadius = new QLabel( "Radius:" );
    toolCylinderRadius->setAlignment( Qt::AlignRight );
    toolCylinderRadius->setMaximumWidth( 70 );

    elements->getSpinBox( 7 )->setMinimum( 0 );
    elements->getSpinBox( 7 )->setValue( 1 );

    layout->addWidget( toolCylinderSegments, 0, 0, 1, 2 );
    layout->addWidget( elements->getSpinBox( 0 ), 0, 2, 1, 2 );
    layout->addWidget( elements->getCheckBox( 0 ), 1, 0, 1, 4 );
    layout->addWidget( toolCylinderCenter, 2, 0, 1, 4 );
    layout->addWidget( toolCylinderNormal, 2, 2, 1, 4 );
    for(int i = 0; i < 3; i++ )
    {
        layout->addWidget( toolCylinderLabels[ i ], 3 + i, 0 );
        layout->addWidget( elements->getSpinBox( 1 + i ), 3 + i, 1 );
        layout->addWidget( toolCylinderLabels[ i + 3 ], 3 + i, 2 );
        layout->addWidget( elements->getSpinBox( 4 + i ), 3 + i, 3 );
    }
    layout->addWidget( toolCylinderRadius, 6, 0, 1, 2 );
    layout->addWidget( elements->getSpinBox( 7 ), 6, 2, 1, 2 );
    layout->addWidget(finalButton, 7, 0, 1, 4 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT( final() ) );
    _widget->hide();

    _hasStage2 = true;
}

void TCylinder::function(Action action, QMouseEvent *event, VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
        Model *model = widget->getModel();
        int vertexSize = model->vertexNumber;
        int triangleSize = model->triangleNumber;
        int segments = getElements()->getSpinBox( 0 )->value();
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
                for( i = 0; i < 3; i++ ) normal[ i ] = getElements()->getSpinBox( 4 + i )->value();
                if( normal.length() == 0 ) return;
                normal.normalize();
                double radius = getElements()->getSpinBox( 7 )->value();
                if( radius == 0 ) return;
                QVector3D center;
                for( i = 0; i < 3; i++ ) center[ i ] = getElements()->getSpinBox( 1 + i )->value();

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
                bool circle = getElements()->getCheckBox( 0 )->isChecked();
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
            setStage2( false );
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
            setStage2( true );
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
        }
}
