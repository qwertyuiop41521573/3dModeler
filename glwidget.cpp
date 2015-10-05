#include "glwidget.h"
#include "mainwindow.h"
#include "functions.h"

#include <iostream>
#include <math.h>

#include <QMouseEvent>

#define GRAY       QVector3D(0.5, 0.5, 0.5)
#define BLACK      QVector3D(0, 0, 0)
#define RED        QVector3D(1, 0, 0)
#define GREEN      QVector3D(0, 1, 0)
#define BLUE       QVector3D(0, 0, 1)
#define SHADED     QVector3D(0.73, 0.78, 0.91)
#define DARK_GRAY  QVector3D(0.4, 0.4, 0.4)
#define LIGHT_BLUE QVector3D(0, 0.4, 0.7)
#define WHITE      QVector3D(1, 1, 1)

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
    camera[TOP].setRotation(90, 0, 0);
    camera[BOTTOM].setRotation(-90, 0, 0);
    camera[BACK].setRotation(0, 0, 180);
    camera[LEFT].setRotation(0, 0, 90);
    camera[RIGHT].setRotation(0, 0,-90);
    const double rad15t = 15 * M_PI / double(180);
    const double rad30 = 30 * M_PI / double(180);
    camera[PERSPECTIVE].setPosition(5 * sin(rad30), 5 * cos(rad30), 5 * tan(
                                        rad15t));
    camera[PERSPECTIVE].setRotation(15, 0,-120);

    vector <VertexData_Color> &verticesG = grid.vertices;

    verticesG.resize(88);
    for(i = 0; i < 88; i++) verticesG[i].color = DARK_GRAY;
    for(i = 0; i < 40; i++)
    {
        verticesG[i].position.setX(i % 4 / 2 ? i / 4 - 10 : i % 2 ? 10 : -10);
        verticesG[i].position.setY(i % 4 / 2 ? i % 2 ? 10 : -10 : i / 4 - 10);

        verticesG[48 + i].position.setX(i % 4 / 2 ? i / 4 + 1 : i % 2 ? 10 :
                                                                       -10);
        verticesG[48 + i].position.setY(i % 4 / 2 ? i % 2 ? 10 : -10 : i / 4 +
                                                           1);
    }

    verticesG[40].position.setX(-10);
    verticesG[42].position.setX(1);
    verticesG[43].position.setX(10);
    verticesG[44].position.setY(-10);
    verticesG[46].position.setY(1);
    verticesG[47].position.setY(10);

    grid.indices.resize(88);
    for(i = 0; i < 88; i++) grid.indices[i] = i;

    vector <VertexData_Color> &verticesA = axis.vertices;
    verticesA.resize(6);

    verticesA[0].color = RED;
    verticesA[1].position.setX(1);
    verticesA[1].color = RED;
    verticesA[2].color = GREEN;
    verticesA[3].position.setY(1);
    verticesA[3].color = GREEN;
    verticesA[4].color = BLUE;
    verticesA[5].position.setZ(1);
    verticesA[5].color = BLUE;

    axis.indices.resize(6);
    for( i = 0; i < 6; i++ ) axis.indices[i] = i;

    frame.vertices.resize(4);
    vector <GLushort> &indicesF = frame.indices;
    indicesF.resize(8);

    indicesF[0] = 0;
    indicesF[1] = 1;
    indicesF[2] = 1;
    indicesF[3] = 2;
    indicesF[4] = 2;
    indicesF[5] = 3;
    indicesF[6] = 3;
    indicesF[7] = 0;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColorVector(GRAY);
    initShaders();
    //if( model->textured ) initTextures();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glGenBuffers(2, modelVboIds);
    glPolygonOffset(-1, -1);
    timer->start(0, this);
}

void GLWidget::resizeGL(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    halfWidth = width / 2;
    halfHeight = height / 2;
    aspect = qreal(width) / qreal(height ? height : 1);
    glViewport(0, 0, width, height);

    vector <VertexData_Color> &verticesF = frame.vertices;
    for(int i = 0; i < 4; i++)
    {
        verticesF[i].position.setX((2 * ((i + 1) / 2 % 2) - 1) * halfWidth);
        verticesF[i].position.setY((2 * (i / 2) - 1) * halfHeight);
    }

    frameMatrix.setToIdentity();
    frameMatrix.ortho(-halfWidth, halfWidth,-halfHeight, halfHeight,-1, 1 );
    toolMatrix = frameMatrix;
    toolMatrixInverse = toolMatrix.inverted();
    toolMatrixPerspectiveInverse.setToIdentity();
    toolMatrixPerspectiveInverse.scale(halfWidth, halfHeight, 1);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupProjection();
    draw();
    drawAdittional();
    if(wireframeOverlay)
    {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT, GL_LINE);
        draw(true);
    }
}

void GLWidget::setupProjection()
{
    projectionMatrix.setToIdentity();
    if(projection == PERSPECTIVE) projectionMatrix.perspective(fov, aspect,
                                                               zNear, zFar);
    else
    {
        projectionMatrix.ortho(-halfWidth, halfWidth,-halfHeight, halfHeight,
                               -1000.f, 1000.f);
        projectionMatrix.scale(scale, scale, 1);
    }
    QVector3D rotation = camera[projection].rotation();
    projectionMatrix.rotate(rotation[0] - 90, 1.f, 0.f, 0.f);
    projectionMatrix.rotate(rotation[1], 0.f, 1.f, 0.f);
    projectionMatrix.rotate(90 - rotation[2], 0.f, 0.f, 1.f);
    projectionWithoutTranslation = projectionMatrix;
    projectionMatrix.translate(-camera[projection].position());
}

void GLWidget::draw(bool wireframe)
{
    RenderingMode renderingModeCurrent = wireframe ? WIREFRAME :
                                                     renderingMode;

    QVector3D color;
    glBindBuffer(GL_ARRAY_BUFFER, modelVboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelVboIds[1]);
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexNumber = vertex.size();
    int triangleNumber = triangle.size();

    switch(renderingModeCurrent)
    {
    //draw vertices
    case WIREFRAME:
    {
        glDisable(GL_CULL_FACE);
        color = BLACK;
        if(workWithElements[0]->isChecked())
        {
            glPointSize(4);
            programColor->bind();
            prepareProgramColor(projectionMatrix);
            vertices_col.resize(vertexNumber);
            indices.resize(vertexNumber);

            for(i = 0; i < vertexNumber; i++)
            {
                vertices_col[i].position = vertex[i].getPosition();
                vertices_col[i].color = vertex[i].newSelected() ? BLUE : vertex[i].isSelected() ? RED : BLACK;
                indices[i] = i;
            }
            glBufferData(GL_ARRAY_BUFFER, vertexNumber *
                     vertexData_ColorSize, vertices_col.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexNumber *
                         GLushortSize, indices.data(), GL_STATIC_DRAW);

            glDrawElements(GL_POINTS, vertexNumber, GL_UNSIGNED_SHORT, 0);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1);
        break;
    }
        //draw triangles
    case FLAT_SHADED:
    {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        color = SHADED;
        glShadeModel(GL_FLAT);
        break;
    }
    case SMOOTH_SHADED:
    {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        color = SHADED;
        glShadeModel(GL_SMOOTH);
        break;
    }
    case TEXTURED:
    {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_SMOOTH);
        if(!model->textured) color = SHADED;
    }
    }
    int j;
    int structSize;
    QString fragment;
    selectedFaces.clear();
    int verticesLength;
    if(renderingModeCurrent == TEXTURED && model->textured)
    {
        vertices_tex.clear();
        for(i = 0; i < triangleNumber; i++)
        {
            if(workWithElements[1]->isChecked() && (triangle[i].newSelected()
                || triangle[i].isSelected())) addSelectedFace(i);
            else for(j = 0; j < 3; j++) vertices_tex.push_back({ vertex[
                triangle[i].getIndex(j)].getPosition(), { (rand() % 10) /
                double(10), (rand() % 10) / double(10) } });
        }
        structSize = vertexData_TextureSize;
        fragment = "a_texcoord";
        programTexture->setUniformValue("texture", 0);
        program = programTexture;
        verticesLength = vertices_tex.size();
        glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize,
                      vertices_tex.data(), GL_STATIC_DRAW);
    }
    else
    {
        vertices_col.clear();
        for(i = 0; i < triangleNumber; i++)
        {
            if(workWithElements[1]->isChecked() && !wireframe && (triangle[i].newSelected() || triangle[i].isSelected())) addSelectedFace(i);
            else for(j = 0; j < 3; j++) vertices_col.push_back({ vertex[
                           triangle[i].getIndex(j)].getPosition(), color });
        }
        structSize = vertexData_ColorSize;
        fragment = "a_color";
        program = programColor;
        verticesLength = vertices_col.size();
        glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize,
                      vertices_col.data(), GL_STATIC_DRAW);
    }
    indices.resize(verticesLength);
    for(i = 0; i < verticesLength; i++) indices[i] = i;
    program->bind();
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, structSize,
                          0);
    int fragmentLocation = program->attributeLocation(fragment);
    program->enableAttributeArray(fragmentLocation);
    glVertexAttribPointer(fragmentLocation, 3, GL_FLOAT, GL_FALSE,
                          structSize, vectorSize);
    program->setUniformValue("mvp_matrix", projectionMatrix);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesLength * GLushortSize,
                 indices.data(), GL_STATIC_DRAW);
    if(wireframe) glEnable(GL_POLYGON_OFFSET_LINE);
    glDrawElements(GL_TRIANGLES, verticesLength, GL_UNSIGNED_SHORT, 0);
    glDisable(GL_POLYGON_OFFSET_LINE);
}

void GLWidget::drawAdittional()
{
    programColor->bind();
    prepareProgramColor(projectionMatrix);
    int i;

    //draw selected faces
    int selectedFacesLength = selectedFaces.size();
    if(selectedFacesLength)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        indices.resize(selectedFacesLength);
        for(i = 0; i < selectedFacesLength; i++) indices[i] = i;
        glBufferData(GL_ARRAY_BUFFER, selectedFacesLength *
           vertexData_ColorSize, selectedFaces.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, selectedFacesLength *
                      GLushortSize, indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, selectedFacesLength,
                        GL_UNSIGNED_SHORT, 0);
    }

    //draw axis
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);
    glBufferData(GL_ARRAY_BUFFER, 6 * vertexData_ColorSize,
                  axis.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * GLushortSize,
                  axis.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, 0);
    if(projection == PERSPECTIVE)
    {
        prepareProgramColor(projectionMatrix);

        glBufferData(GL_ARRAY_BUFFER, 88 * vertexData_ColorSize,
                      grid.vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 88 * GLushortSize,
                      grid.indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_LINES, 88, GL_UNSIGNED_SHORT, 0);
    }

    //draw tool marks and select color for frame
    QVector3D color;
    if(_isActive)
    {
        if(toolIsOn)
        {
            toolData.vertices.clear();
            toolData.indices.clear();
            (*activeTool)->function(DRAW);
            int vertexNumber = toolData.vertices.size();
            int indexNumber = toolData.indices.size();
            for(i = 0; i < vertexNumber; i++) toolData.vertices[i].color = WHITE;
            prepareProgramColor(toolMatrix);
            glBufferData(GL_ARRAY_BUFFER, vertexNumber *
                vertexData_ColorSize, toolData.vertices.data(),
                          GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber *
                GLushortSize, toolData.indices.data(), GL_STATIC_DRAW);

            glDrawElements(GL_LINES, indexNumber, GL_UNSIGNED_SHORT,
                            0);
        }
        color = LIGHT_BLUE;
        glLineWidth(4);
    }
    else
    {
        color = BLACK;
        glLineWidth(2);
    }

    //draw frame
    for(i = 0; i < 4; i++) frame.vertices[i].color = color;
    prepareProgramColor(frameMatrix);
    glBufferData(GL_ARRAY_BUFFER, 4 * vertexData_ColorSize,
                  frame.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * GLushortSize,
                  frame.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);
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
    else if(event->button() == Qt::LeftButton)
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
        camera[ PERSPECTIVE ].addToPosition(dy * cosR(rotation.x()) * cosR(rotation.z()), dy * cosR(rotation.x()) * sinR(rotation.z()),
                                   -dy * sinR(rotation.x()));
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
    QVector3D selectedColor(triangle[ num ].newSelected() ? BLUE : RED);
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
