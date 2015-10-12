#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"

#define GRAY       QVector3D(0.5, 0.5, 0.5)
#define BLACK      QVector3D(0, 0, 0)
#define RED        QVector3D(1, 0, 0)
#define GREEN      QVector3D(0, 1, 0)
#define BLUE       QVector3D(0, 0, 1)
#define SHADED     QVector3D(0.73, 0.78, 0.91)
#define DARK_GRAY  QVector3D(0.4, 0.4, 0.4)
#define LIGHT_BLUE QVector3D(0, 0.4, 0.7)
#define WHITE      QVector3D(1, 1, 1)

#include <iostream>
using namespace std;

GLWidget::GLWidget(MainWindow *mainWindow, QWidget *parent) : QOpenGLWidget(parent)
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
    camera[PERSPECTIVE].setPosition(5 * sinR(30), 5 * cosR(30), 5 * tanR(15));
    camera[PERSPECTIVE].setRotation(15, 0,-120);

    for(i = 0; i < 10; i++)
    {
        line(&grid, {i - 10, -10, 0}, {i - 10, 10, 0}, DARK_GRAY);
        line(&grid, {-10, i - 10, 0}, {10, i - 10, 0}, DARK_GRAY);
    }

    line(&grid, {-10, 0, 0}, {0, 0, 0}, DARK_GRAY);
    line(&grid, {1, 0, 0}, {10, 0, 0}, DARK_GRAY);
    line(&grid, {0, -10, 0}, {0, 0, 0}, DARK_GRAY);
    line(&grid, {0, 1, 0}, {0, 10, 0}, DARK_GRAY);

    for(i = 0; i < 10; i++)
    {
        line(&grid, {i + 1, -10, 0}, {i + 1, 10, 0}, DARK_GRAY);
        line(&grid, {-10, i + 1, 0}, {10, i + 1, 0}, DARK_GRAY);
    }

    line(&axis, {0, 0, 0}, {1, 0, 0}, RED);
    line(&axis, {0, 0, 0}, {0, 1, 0}, GREEN);
    line(&axis, {0, 0, 0}, {0, 0, 1}, BLUE);

    frame.vertices.resize(4);
    vector <GLuint> &indicesF = frame.indices;
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
    if(projection == PERSPECTIVE) projectionMatrix.perspective(fov, aspect, zNear, zFar);
    else
    {
        projectionMatrix.ortho(-halfWidth, halfWidth,-halfHeight, halfHeight, -1000.f, 1000.f);
        projectionMatrix.scale(scale, scale, 1);
    }
    const QVector3D &rotation = camera[projection].rotation();
    projectionMatrix.rotate(rotation[0] - 90, 1.f, 0.f, 0.f);
    projectionMatrix.rotate(rotation[1], 0.f, 1.f, 0.f);
    projectionMatrix.rotate(90 - rotation[2], 0.f, 0.f, 1.f);
    projectionWithoutTranslation = projectionMatrix;
    projectionMatrix.translate(-camera[projection].position());
}

void GLWidget::draw(bool wireframe)
{
    RenderingMode renderingModeCurrent = wireframe ? WIREFRAME : renderingMode;

    QVector3D color;
    glBindBuffer(GL_ARRAY_BUFFER, modelVboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelVboIds[1]);
    int i;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexNumber = vertex.size();
    int triangleNumber = triangle.size();

  /*  if(_isActive)
    {
        for(i = 0; i < vertexNumber; i++) cerr << vertex[i].exists() << ' ';
        cerr << '\n';
    }*/

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

            vertices_col.clear();
            for(i = 0; i < vertex.size(); i++) if(vertex[i].exists()) vertices_col.push_back({vertex[i].getPosition(), vertex[i].newSelected() ? BLUE : vertex[i].selected() ? RED : BLACK});
            int vertSize = vertices_col.size();
            indices.resize(vertSize);
            for(i = 0; i < vertSize; i++) indices[i] = i;

            glBufferData(GL_ARRAY_BUFFER, vertSize * vertexData_ColorSize, vertices_col.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertSize * GLuintSize, indices.data(), GL_STATIC_DRAW);

            glDrawElements(GL_POINTS, vertSize, GL_UNSIGNED_INT, 0);
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
            if(workWithElements[1]->isChecked() && (triangle[i].newSelected() || triangle[i].selected())) addSelectedFace(i);
            else for(j = 0; j < 3; j++) vertices_tex.push_back({ vertex[triangle[i].getIndex(j)].getPosition(), { (rand() % 10) / double(10), (rand() % 10) / double(10) } });
        }
        structSize = vertexData_TextureSize;
        fragment = "a_texcoord";
        programTexture->setUniformValue("texture", 0);
        program = programTexture;
        verticesLength = vertices_tex.size();
        glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize, vertices_tex.data(), GL_STATIC_DRAW);
    }
    else
    {
        vertices_col.clear();
        for(i = 0; i < triangleNumber; i++)
        {
            if(workWithElements[1]->isChecked() && !wireframe && (triangle[i].newSelected() || triangle[i].selected())) addSelectedFace(i);
            else for(j = 0; j < 3; j++) vertices_col.push_back({ vertex[triangle[i].getIndex(j)].getPosition(), color });
        }
        structSize = vertexData_ColorSize;
        fragment = "a_color";
        program = programColor;
        verticesLength = vertices_col.size();
        glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize, vertices_col.data(), GL_STATIC_DRAW);
    }
    indices.resize(verticesLength);
    for(i = 0; i < verticesLength; i++) indices[i] = i;
    program->bind();
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, structSize, 0);
    int fragmentLocation = program->attributeLocation(fragment);
    program->enableAttributeArray(fragmentLocation);
    glVertexAttribPointer(fragmentLocation, 3, GL_FLOAT, GL_FALSE, structSize, vectorSize);
    program->setUniformValue("mvp_matrix", projectionMatrix);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesLength * GLuintSize, indices.data(), GL_STATIC_DRAW);
    if(wireframe) glEnable(GL_POLYGON_OFFSET_LINE);
    glDrawElements(GL_TRIANGLES, verticesLength, GL_UNSIGNED_INT, 0);
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
        glBufferData(GL_ARRAY_BUFFER, selectedFacesLength * vertexData_ColorSize, selectedFaces.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, selectedFacesLength * GLuintSize, indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, selectedFacesLength, GL_UNSIGNED_INT, 0);
    }

    //draw axis
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);
    glBufferData(GL_ARRAY_BUFFER, 6 * vertexData_ColorSize, axis.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * GLuintSize, axis.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

    //draw grid
    if(projection == PERSPECTIVE)
    {
        prepareProgramColor(projectionMatrix);

        glBufferData(GL_ARRAY_BUFFER, 88 * vertexData_ColorSize, grid.vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 88 * GLuintSize, grid.indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_LINES, 88, GL_UNSIGNED_INT, 0);
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
            glBufferData(GL_ARRAY_BUFFER, vertexNumber * vertexData_ColorSize, toolData.vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber * GLuintSize, toolData.indices.data(), GL_STATIC_DRAW);

            glDrawElements(GL_LINES, indexNumber, GL_UNSIGNED_INT, 0);
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
    glBufferData(GL_ARRAY_BUFFER, 4 * vertexData_ColorSize, frame.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * GLuintSize, frame.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    update();
}

void GLWidget::initShaders()
{
    if(!programColor->addShaderFromSourceFile(QGLShader::Vertex, ":/color.vert")) close();
    if(!programColor->addShaderFromSourceFile(QGLShader::Fragment, ":/color.frag")) close();

    if(!programTexture->addShaderFromSourceFile(QGLShader::Vertex, ":/texture.vert")) close();
    if(!programTexture->addShaderFromSourceFile(QGLShader::Fragment, ":/texture.frag")) close();
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

    if(!toolIsOn) startPosition = lastPosition = QVector2D(event->x() - halfWidth, halfHeight - event->y());
    if(!quickAccess)
    {
        switch(event->buttons())
        {
        case Qt::LeftButton:
        {
            Tool *aT = *activeTool;
            //  ! stage2     hasStage2 == true && stage2 == false
            //               hasStage2 == false
            //  hasStage2 ? !stage2 : true
            if(aT->hasStage2() ? !aT->stage2() : true) aT->function(START, event);

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

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton || quickAccess || (*activeTool)->stage2())
    {
        toolIsOn = true;
        (*activeTool)->function(EXECUTE, event);
        lastPosition = currentPosition = QVector2D(event->x() - halfWidth, halfHeight - event->y());
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
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

void GLWidget::wheelEvent(QWheelEvent *event)
{
    bool perspective = projection == PERSPECTIVE;
    double dy =-event->angleDelta().y() / double(perspective ? 150 : 1000);
    if(perspective)
    {
        QVector3D rotation = camera[PERSPECTIVE].rotation();
        camera[PERSPECTIVE].addToPosition(dy * cosR(rotation.x()) * cosR(rotation.z()), dy * cosR(rotation.x()) * sinR(rotation.z()), -dy * sinR(rotation.x()));
    }
    else scale *= exp(dy);
}


void GLWidget::prepareProgramColor(const QMatrix4x4 &matrix)
{
    int vertexLocation = programColor->attributeLocation("a_position" );
    programColor->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, vertexData_ColorSize, 0);
    int colorLocation = programColor->attributeLocation("a_color");
    programColor->enableAttributeArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, vertexData_ColorSize, vectorSize);
    programColor->setUniformValue("mvp_matrix", matrix);
}

void GLWidget::addSelectedFace(int num)
{
    vector <Triangle> &triangle = model->getTriangle();
    QVector3D selectedColor(triangle[num].newSelected() ? BLUE : RED);
    for(int j = 0; j < 3; j++) selectedFaces.push_back(VertexData_Color(model->getVertex()[triangle[num].getIndex(j)].getPosition(), selectedColor));
}

void GLWidget::countRotationMatrices()
{
    rotationMatrix.setToIdentity();
    rotationMatrix.scale(aspect, 1, 1);
    rotationMatrix *= projectionWithoutTranslation;

    rotationMatrixInverse = rotationMatrix.inverted();
}

void GLWidget::countFinalInverseMatrix()
{
    countFinalMatrix();
    finalMatrixInverse = finalMatrix.inverted();
}


void GLWidget::fromWorldToScreen(QVector2D *answer, const QVector3D &vector, bool point)
{
    QVector4D temp = finalMatrix * QVector4D(vector, point);
    if(point && projection == PERSPECTIVE) for(int i = 0; i < 2; i++) temp[i] /= temp[3];
    *answer = QVector2D(temp[0], temp[1]);
}

void GLWidget::fromScreenToWorld(QVector3D *answer, QMouseEvent *event, bool forcedHeight, double height)
{
    _fromScreenToWorld(answer, QVector4D(event->x() - halfWidth, halfHeight - event->y(), 0, 1), forcedHeight, height);
}

void GLWidget::_fromScreenToWorld(QVector3D *answer, const QVector4D &screenCoordinates, bool forcedHeight, double height)
{
    QVector4D worldCoordinates;
    if(projection == PERSPECTIVE)
    {
        double a[4][4];
        int i, j;
        for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) a[i][j] = finalMatrixInverse.data()[4 * j + i];
        QVector4D screenCoordPersp;
        screenCoordinatesPerspective(&screenCoordPersp, a, height, screenCoordinates);
        if(!forcedHeight && screenCoordPersp.z() < 0)
        {
            height = int(camera[projection].position().z()) + 2 * (camera[projection].position().z() > 0) - 1;
            screenCoordinatesPerspective(&screenCoordPersp, a, height, screenCoordinates);
        }
        worldCoordinates = finalMatrixInverse * screenCoordPersp;
        worldCoordinates.setZ(height);
    }
    else worldCoordinates = finalMatrixInverse * screenCoordinates;
    *answer = QVector3D(worldCoordinates);
}

void GLWidget::screenCoordinatesPerspective(QVector4D *answer, double a[4][4], double h, const QVector4D &screenCoordinates)
{

    double x = screenCoordinates.x(), y = screenCoordinates.y();
    double w = (a[2][2] - a[3][2] * h) / ((a[3][0] * a[2][2] - a[3][2] * a[2][0]) * x + (a[3][1] * a[2][2] - a[3][2] * a[2][1]) * y + a[3][3] * a[2][2] - a[3][2] * a[2][3]);
    *answer = QVector4D(screenCoordinates.x(), screenCoordinates.y(), (h - (a[2][0] * x + a[2][1] * y + a[2][3]) * w) / a[2][2], w);
    for(int i = 0; i < 2; i++) (*answer)[i] *= w;
}

bool GLWidget::isSelected(const QVector3D &vertex, const QVector2D &min, const QVector2D &max)
{
    QVector4D result = finalMatrix * QVector4D(vertex, 1);
    if(projection == PERSPECTIVE) for(int i = 0; i < 2; i++) result[i] /= result[3];
    return result.x() > min.x() && result.x() < max.x() && result.y() > min.y() && result.y() < max.y();
}

void GLWidget::line(VertexAndIndexData *data, QVector3D a, QVector3D b, QVector3D color)
{
    data->vertices.push_back({a, color});
    data->vertices.push_back({b, color});
    for(int i = 0; i < 4; i++) data->indices.push_back(data->indices.size());
}
