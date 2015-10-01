#include "tellipse.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TEllipse::TEllipse(MainWindow *mainWindow) : ToolWithWidget(
                                                 mainWindow)
{
    button->setText("Ellipse");
    finalButton = new QPushButton("Create Ellipse");

    int i;
    for(i = 0; i < 8; i++) spinBox[i] = new MySpinBox;
    spinBox[0]->setMinimum(3);
    spinBox[0]->setValue(18);

    MyLabel *segments = new MyLabel("Segments:", 70);

    checkBox = new MyCheckBoxMW;
    checkBox->setText("Circle");

    QLabel *center = new QLabel("Center");
    QLabel *normal = new QLabel("Normal");
    MyLabel *label[6];
    for(int i = 0; i < 6; i++) label[i] = new MyLabel(QString( 'X'
                                              + i % 3 ) + ':', 25);
    MyLabel *radius = new MyLabel("Radius:", 70);

    spinBox[7]->setMinimum(0);
    spinBox[7]->setValue(1);

    layout->addWidget(segments, 0, 0, 1, 2);
    layout->addWidget(spinBox[0], 0, 2, 1, 2);
    layout->addWidget(checkBox, 1, 0, 1, 4);
    layout->addWidget(center, 2, 0, 1, 4);
    layout->addWidget(normal, 2, 2, 1, 4);
    for(int i = 0; i < 3; i++)
    {
        layout->addWidget(label[i], 3 + i, 0);
        layout->addWidget(spinBox[1 + i], 3 + i, 1);
        layout->addWidget(label[i + 3], 3 + i, 2);
        layout->addWidget(spinBox[4 + i], 3 + i, 3);
    }
    layout->addWidget(radius, 6, 0, 1, 2);
    layout->addWidget(spinBox[7], 6, 2, 1, 2);
    layout->addWidget(finalButton, 7, 0, 1, 4);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(
                final()));
    _widget->hide();
}

void TEllipse::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
        int vertexSize = model->vertexNumber;
        int triangleSize = model->triangleNumber;
        int segments = spinBox[0]->value();
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
                for( i = 0; i < 3; i++ ) normal[ i ] = spinBox[4 + i]->value();
                if( normal.length() == 0 ) return;
                normal.normalize();
                double radius = spinBox[7]->value();
                if( radius == 0 ) return;
                QVector3D center;
                for( i = 0; i < 3; i++ ) center[ i ] = spinBox[1 + i]->value();

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
            bool circle = checkBox->isChecked();
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

