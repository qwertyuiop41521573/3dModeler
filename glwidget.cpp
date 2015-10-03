#include "glwidget.h"
#include "mainwindow.h"
#include "functions.h"

#include <iostream>
#include <math.h>

#include <QMouseEvent>

using namespace std;

GLWidget::GLWidget(MainWindow *mainWindow, QWidget *parent) :
    QOpenGLWidget(parent)
{
    _mainWindow = mainWindow;
    model = _mainWindow->getModel();

    activeTool = _mainWindow->getActiveTool();
    workWithElements = _mainWindow->getWorkWithElements();



    timer = new QBasicTimer;
    programColor = new QGLShaderProgram;
    programTexture =new QGLShaderProgram;

    int i;
    camera[ TOP ].setRotation( 90, 0, 0 );
    camera[ BOTTOM ].setRotation(-90, 0, 0 );
    camera[ BACK ].setRotation( 0, 0, 180 );
    camera[ LEFT ].setRotation( 0, 0, 90 );
    camera[ RIGHT ].setRotation( 0, 0,-90 );
    const double rad15t = 15 * M_PI / double( 180 );
    const double rad30 = 30 * M_PI / double( 180 );
    camera[ PERSPECTIVE ].setPosition( 5 * sin( rad30 ), 5 * cos( rad30 ), 5 * tan( rad15t ) );
    camera[ PERSPECTIVE ].setRotation( 15, 0,-120 );//210 );

    grid.vertices.resize( 88 );

    for( i = 0; i < 10; i++ )
    {
        grid.vertices[ 4 * i ].position.setX(-10 );
        grid.vertices[ 4 * i ].position.setY( i - 10 );
        grid.vertices[ 4 * i ].color = darkGray;
        grid.vertices[ 4 * i + 1 ].position.setX( 10 );
        grid.vertices[ 4 * i + 1 ].position.setY( i - 10 );
        grid.vertices[ 4 * i + 1 ].color = darkGray;
        grid.vertices[ 4 * i + 2 ].position.setX( i - 10 );
        grid.vertices[ 4 * i + 2 ].position.setY(-10 );
        grid.vertices[ 4 * i + 2 ].color = darkGray;
        grid.vertices[ 4 * i + 3 ].position.setX( i - 10 );
        grid.vertices[ 4 * i + 3 ].position.setY( 10 );
        grid.vertices[ 4 * i + 3 ].color = darkGray;
    }

    grid.vertices[ 40 ].position.setX(-10 );
    grid.vertices[ 40 ].color = darkGray;
    grid.vertices[ 41 ].color = darkGray;
    grid.vertices[ 42 ].position.setX( 1 );
    grid.vertices[ 42 ].color = darkGray;
    grid.vertices[ 43 ].position.setX( 10 );
    grid.vertices[ 43 ].color = darkGray;
    grid.vertices[ 44 ].position.setY(-10 );
    grid.vertices[ 44 ].color = darkGray;
    grid.vertices[ 45 ].color = darkGray;
    grid.vertices[ 46 ].position.setY( 1 );
    grid.vertices[ 46 ].color = darkGray;
    grid.vertices[ 47 ].position.setY( 10 );
    grid.vertices[ 47 ].color = darkGray;

    for( i = 1; i < 11; i++ )
    {
        grid.vertices[ 4 * i + 44 ].position.setX(-10 );
        grid.vertices[ 4 * i + 44 ].position.setY( i );
        grid.vertices[ 4 * i + 44 ].color = darkGray;
        grid.vertices[ 4 * i + 45 ].position.setX( 10 );
        grid.vertices[ 4 * i + 45 ].position.setY( i );
        grid.vertices[ 4 * i + 45 ].color = darkGray;
        grid.vertices[ 4 * i + 46 ].position.setX( i );
        grid.vertices[ 4 * i + 46 ].position.setY(-10 );
        grid.vertices[ 4 * i + 46 ].color = darkGray;
        grid.vertices[ 4 * i + 47 ].position.setX( i );
        grid.vertices[ 4 * i + 47 ].position.setY( 10 );
        grid.vertices[ 4 * i + 47 ].color = darkGray;
    }

    grid.indices.resize( 88 );
    for( i = 0; i < 88; i++ ) grid.indices[ i ] = i;

    axis.vertices.resize( 6 );

    axis.vertices[ 0 ].color = red;
    axis.vertices[ 1 ].position.setX( 1 );
    axis.vertices[ 1 ].color = red;
    axis.vertices[ 2 ].color = green;
    axis.vertices[ 3 ].position.setY( 1 );
    axis.vertices[ 3 ].color = green;
    axis.vertices[ 4 ].color = blue;
    axis.vertices[ 5 ].position.setZ( 1 );
    axis.vertices[ 5 ].color = blue;

    axis.indices.resize( 6 );
    for( i = 0; i < 6; i++ ) axis.indices[ i ] = i;

    frame.vertices.resize( 4 );
    frame.indices.resize( 8 );
    frame.indices[ 0 ] = 0;
    frame.indices[ 1 ] = 1;
    frame.indices[ 2 ] = 1;
    frame.indices[ 3 ] = 2;
    frame.indices[ 4 ] = 2;
    frame.indices[ 5 ] = 3;
    frame.indices[ 6 ] = 3;
    frame.indices[ 7 ] = 0;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColorVector( gray );
    initShaders();
    //if( model->textured ) initTextures();
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glGenBuffers( 2, modelVboIds );
    timer->start( 0, this );
}

void GLWidget::resizeGL( int newWidth, int newHeight )
{
    width = newWidth;
    height = newHeight;
    halfWidth = width / 2;
    halfHeight = height / 2;
    aspect = qreal( width ) / qreal( height ? height : 1);
    glViewport( 0, 0, width, height );

    frame.vertices[ 0 ].position.setX(-halfWidth );
    frame.vertices[ 0 ].position.setY(-halfHeight );
    frame.vertices[ 1 ].position.setX( halfWidth );
    frame.vertices[ 1 ].position.setY(-halfHeight );
    frame.vertices[ 2 ].position.setX( halfWidth );
    frame.vertices[ 2 ].position.setY( halfHeight );
    frame.vertices[ 3 ].position.setX(-halfWidth );
    frame.vertices[ 3 ].position.setY( halfHeight );

    frameMatrix.setToIdentity();
    frameMatrix.ortho(-halfWidth, halfWidth,-halfHeight,
                           halfHeight,-1, 1 );
    toolMatrix = frameMatrix;
    toolMatrixInverse = toolMatrix.inverted();
    toolMatrixPerspectiveInverse.setToIdentity();
    toolMatrixPerspectiveInverse.scale( halfWidth, halfHeight, 1 );
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupProjection();
    draw();
    drawAdittional();
    if( wireframeOverlay )
    {
        glEnable( GL_CULL_FACE );
        glPolygonMode( GL_FRONT, GL_LINE );
        draw( true );
    }
}

void GLWidget::setupProjection()
{
    projectionMatrix.setToIdentity();
    if( projection == PERSPECTIVE ) projectionMatrix.perspective( fov, aspect, zNear, zFar );
    else
    {
        projectionMatrix.ortho(-halfWidth, halfWidth,-halfHeight,
                         halfHeight,-1000.f, 1000.f );
        projectionMatrix.scale( scale, scale, 1 );
    }
    projectionMatrix.rotate( camera[ projection ].rotation()[ 0 ] - 90, 1.f, 0.f, 0.f );
    projectionMatrix.rotate( camera[ projection ].rotation()[ 1 ], 0.f, 1.f, 0.f );
    projectionMatrix.rotate( 90 - camera[ projection ].rotation()[ 2 ], 0.f, 0.f, 1.f );
    projectionWithoutTranslation = projectionMatrix;
    projectionMatrix.translate(-camera[ projection ].position() );
}

void GLWidget::draw( bool wireframe )
{
    RenderingMode backup;
    if( wireframe )
    {
        backup = renderingMode;
        renderingMode = WIREFRAME;
    }

    QVector3D color;
    glBindBuffer( GL_ARRAY_BUFFER, modelVboIds[ 0 ] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, modelVboIds[ 1 ] );
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexNumber = vertex.size();
    int triangleNumber = triangle.size();

    switch( renderingMode )
    {
    case WIREFRAME:
    {
        glDisable( GL_CULL_FACE );
        color = black;
        if( workWithElements[0]->isChecked() )
        {
            glPointSize( 4 );
            programColor->bind();
            prepareProgramColor( projectionMatrix );
            VertexData_Color *vertices = new VertexData_Color[
                    vertexNumber ];
            GLushort *indices = new GLushort[vertexNumber ];

            for( i = 0; i < vertexNumber; i++ )
            {
                vertices[ i ].position = vertex[i].
                        getPosition();
                if(vertex[i].newSelected() ||
                        vertex[i].isSelected() )
                    vertices[ i ].color = vertex[i].
                            newSelected() ? blue : red;
                else vertices[ i ].color = black;
                indices[ i ] = i;
            }
            glBufferData( GL_ARRAY_BUFFER, vertexNumber *
                     vertexData_ColorSize, vertices, GL_STATIC_DRAW );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                vertexNumber * GLushortSize, indices,
                          GL_STATIC_DRAW );
            glDrawElements( GL_POINTS, vertexNumber,
                            GL_UNSIGNED_SHORT, 0 );
        }
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );  
        glLineWidth( 1 );
        break;
    }
    case FLAT_SHADED:
    {
        glEnable( GL_CULL_FACE );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        color = shadedColor;
        glShadeModel( GL_FLAT );
        break;
    }
    case SMOOTH_SHADED:
    {
        glEnable( GL_CULL_FACE );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        color = shadedColor;
        glShadeModel( GL_SMOOTH );
        break;
    }
    case TEXTURED:
    {
        glEnable( GL_CULL_FACE );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glShadeModel( GL_SMOOTH );
        if( !model->textured ) color = shadedColor;
    }
    }
    int j;
    int structSize;
    QString fragment;
    selectedFaces.clear();
    int verticesLength;
    if( renderingMode == TEXTURED && model->textured )
    {
        vertices_tex.clear();
        for( i = 0; i < triangleNumber; i++ )
        {
            if( workWithElements[1]->isChecked()
               && ( triangle[ i ].newSelected() ||
                    triangle[ i ].isSelected() ) )
                addSelectedFace( i );
            else for( j = 0; j < 3; j++ ) vertices_tex.push_back(
                { vertex[ triangle[ i ].getIndex( j )
                  ].getPosition(), { ( rand() % 10 ) / double( 10 ),
                        ( rand() % 10 ) / double( 10 ) } } );
        }
        structSize = vertexData_TextureSize;
        fragment = "a_texcoord";
        programTexture->setUniformValue( "texture", 0 );
        program = programTexture;
        verticesLength = vertices_tex.size();
        glBufferData( GL_ARRAY_BUFFER, verticesLength * structSize,
                      vertices_tex.data(), GL_STATIC_DRAW );
    }
    else
    {
        vertices_col.clear();
        for( i = 0; i < triangleNumber; i++ )
        {
            if( workWithElements[1]->isChecked()
               && ( triangle[ i ].newSelected() ||
                    triangle[ i ].isSelected() ) )
                addSelectedFace( i );
            else for( j = 0; j < 3; j++ ) vertices_col.push_back(
                { vertex[ triangle[ i ].getIndex( j )
                  ].getPosition(), color } );
        }
        structSize = vertexData_ColorSize;
        fragment = "a_color";
        program = programColor;
        verticesLength = vertices_col.size();
        glBufferData( GL_ARRAY_BUFFER, verticesLength * structSize,
                      vertices_col.data(), GL_STATIC_DRAW );
    }
    GLushort *indices = new GLushort[ verticesLength ];
    for( i = 0; i < verticesLength; i++ )
        indices[ i ] = i;
    program->bind();
    int vertexLocation = program->attributeLocation(
                "a_position" );
    program->enableAttributeArray( vertexLocation );
    glVertexAttribPointer( vertexLocation, 3, GL_FLOAT,
        GL_FALSE, structSize, 0 );
    int fragmentLocation = program->attributeLocation(
                fragment );
    program->enableAttributeArray( fragmentLocation );
    glVertexAttribPointer( fragmentLocation, 3, GL_FLOAT,
        GL_FALSE, structSize, vectorSize );
    program->setUniformValue( "mvp_matrix", projectionMatrix );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, verticesLength
                  * GLushortSize, indices, GL_STATIC_DRAW );
    glDrawElements( GL_TRIANGLES, verticesLength,
                    GL_UNSIGNED_SHORT, 0 );

    if( wireframe ) renderingMode = backup;
}

void GLWidget::drawAdittional()
{
    programColor->bind();
    prepareProgramColor( projectionMatrix );
    int i;
    int selectedFacesLength = selectedFaces.size();
    if( selectedFacesLength )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        GLushort *indices = new GLushort[ selectedFacesLength ];
        for( i = 0; i < selectedFacesLength; i++ ) indices[ i ] = i;
        glBufferData( GL_ARRAY_BUFFER, selectedFacesLength *
           vertexData_ColorSize, selectedFaces.data(), GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, selectedFacesLength *
                      GLushortSize, indices, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, selectedFacesLength,
                        GL_UNSIGNED_SHORT, 0 );
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth( 1 );
    glBufferData( GL_ARRAY_BUFFER, 6 * vertexData_ColorSize,
                  axis.vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * GLushortSize,
                  axis.indices.data(), GL_STATIC_DRAW );
    glDrawElements( GL_LINES, 6, GL_UNSIGNED_SHORT, 0 );
    if( projection == PERSPECTIVE )
    {
        prepareProgramColor( projectionMatrix );

        glBufferData( GL_ARRAY_BUFFER, 88 * vertexData_ColorSize,
                      grid.vertices.data(), GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 88 * GLushortSize,
                      grid.indices.data(), GL_STATIC_DRAW );
        glDrawElements( GL_LINES, 88, GL_UNSIGNED_SHORT, 0 );
    }

    QVector3D color;
    if( _isActive )
    {
        if( toolIsOn )
        {
            (*activeTool)->function(DRAW);
            int vertexNumber = toolData.vertices.size();
            int indexNumber = toolData.indices.size();
            for( i = 0; i < vertexNumber; i++ ) toolData.vertices[ i ].
                    color = white;
            prepareProgramColor( toolMatrix );
            glBufferData( GL_ARRAY_BUFFER, vertexNumber *
                vertexData_ColorSize, toolData.vertices.data(),
                          GL_STATIC_DRAW );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexNumber *
                GLushortSize, toolData.indices.data(), GL_STATIC_DRAW );
            toolData.vertices.clear();
            toolData.indices.clear();
            glDrawElements( GL_LINES, indexNumber, GL_UNSIGNED_SHORT,
                            0 );
        }
        color = lightBlue;
        glLineWidth( 4 );
    }
    else
    {
        color = black;
        glLineWidth( 2 );
    }

    for( i = 0; i < 4; i++ ) frame.vertices[ i ].color = color;
    prepareProgramColor( frameMatrix );
    glBufferData( GL_ARRAY_BUFFER, 4 * vertexData_ColorSize,
                  frame.vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 8 * GLushortSize,
                  frame.indices.data(), GL_STATIC_DRAW );
    glDrawElements( GL_LINES, 8, GL_UNSIGNED_SHORT, 0 );
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    update();
}

void GLWidget::initShaders()
{
    if ( !programColor->addShaderFromSourceFile( QGLShader::
                              Vertex, ":/color.vert" ) )
        close();
    if ( !programColor->addShaderFromSourceFile( QGLShader::
                           Fragment, ":/color.frag") )
        close();

    if ( !programTexture->addShaderFromSourceFile( QGLShader::
                              Vertex, ":/texture.vert" ) )
        close();
    if ( !programTexture->addShaderFromSourceFile( QGLShader::
                           Fragment, ":/texture.frag") )
        close();
}

/*
void GLWidget::initTextures()
{
    glEnable( GL_TEXTURE_2D );
    texture = bindTexture( QImage( model->texture.c_str() ) );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     GL_REPEAT );
}*/

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(!_isActive) _mainWindow->setActiveWidget(this);





    if( !toolIsOn ) startPosition = lastPosition = QVector2D( event->x() - halfWidth,
                                          halfHeight - event->y() );
    if( !quickAccess )
    {
        switch( event->buttons() )
        {
        case Qt::LeftButton:
        {
            Tool *aT = *activeTool;
            //  ! stage2     hasStage2 == true && stage2 == false
            //               hasStage2 == false
            //  hasStage2 ? !stage2 : true
            if(aT->hasStage2() ? !aT->stage2() : true) aT->function(
                        START, event);


            break;
        }
        case Qt::RightButton:
        {
            _mainWindow->quickAccessToolOrbit();
            quickAccess = true;
            break;
        }
        case Qt::MiddleButton:
        {
            _mainWindow->quickAccessToolPan();
            quickAccess = true;
        }
        }
    }
}

void GLWidget::mouseMoveEvent( QMouseEvent *event )
{
    if( event->buttons() & Qt::LeftButton || quickAccess || ( *activeTool )->stage2() )
    {
        toolIsOn = true;
        (*activeTool)->function(EXECUTE, event);
        lastPosition = currentPosition = QVector2D( event->x() -
                          halfWidth, halfHeight - event->y() );
    }
}

void GLWidget::mouseReleaseEvent( QMouseEvent *event )
{
    if(quickAccess)
    {
        toolIsOn = false;
        _mainWindow->stopQuickAccess();
        quickAccess = false;
    }
    else if( event->button() == Qt::LeftButton )
    {
        toolIsOn = false;
        Tool *aT = *activeTool;
        if(aT->hasStage2() && !aT->stage2()) aT->function(STAGE2);
        else aT->function(STOP);
    }
}

void GLWidget::wheelEvent( QWheelEvent *event )
{
    bool perspective = projection == PERSPECTIVE;
    double dy =-event->angleDelta().y() / double( perspective ? 150 : 1000 );
    if( perspective )
    {
        QVector3D rotation = camera[ PERSPECTIVE ].rotation();
        double radZ = inRadians( rotation.z() );
        double radX = inRadians( rotation.x() );
        camera[ PERSPECTIVE ].addToPosition( dy * cos( radX ) * cos( radZ ), dy * cos( radX ) * sin( radZ ),
                                   -dy * sin( radX ) );
    }
    else scale *= exp( dy );
}

void GLWidget::countFinalInverseMatrix( bool perspective )
{
    countFinalMatrix( perspective );
    finalMatrixInverse = finalMatrix.inverted();
}

void GLWidget::prepareProgramColor( QMatrix4x4 matrix )
{
    int vertexLocation = programColor->attributeLocation(
                "a_position" );
    programColor->enableAttributeArray( vertexLocation );
    glVertexAttribPointer( vertexLocation, 3, GL_FLOAT,
        GL_FALSE, vertexData_ColorSize, 0 );
    int colorLocation = programColor->attributeLocation(
                "a_color" );
    programColor->enableAttributeArray( colorLocation );
    glVertexAttribPointer( colorLocation, 3, GL_FLOAT,
        GL_FALSE, vertexData_ColorSize, vectorSize );
    programColor->setUniformValue( "mvp_matrix", matrix );
}

void GLWidget::addSelectedFace( int num )
{
    vector <Triangle> &triangle = model->getTriangle();
    QVector3D selectedColor = triangle[ num ].newSelected()
            ? blue : red;
    for( int j = 0; j < 3; j++ ) selectedFaces.push_back(
        VertexData_Color( model->getVertex()[
        triangle[ num ].getIndex( j ) ].getPosition(),
                        selectedColor ) );
}

QVector2D GLWidget::fromWorldToScreen( QVector3D vector, bool point )
{
    bool perspective = projection == PERSPECTIVE;
    countFinalMatrix( perspective );
    QVector4D temp = finalMatrix * QVector4D( vector, point );
    if( point && perspective ) for( int i = 0; i < 2; i++ )
        temp[ i ] /= temp[ 3 ];
    return QVector2D( temp[ 0 ], temp[ 1 ] );
}

void GLWidget::countRotationMatrices()
{
    rotationMatrix.setToIdentity();
    rotationMatrix.scale( aspect, 1, 1 );
    rotationMatrix *= projectionWithoutTranslation;

    rotationMatrixInverse = rotationMatrix.inverted();
}

void GLWidget::setCurrentPosition( double x, double y )
{
    currentPosition.setX( x );
    currentPosition.setY( y );
}
