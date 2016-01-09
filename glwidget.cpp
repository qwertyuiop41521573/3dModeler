#include "glwidget.h"
#include "mainwindow.h"
#include "mathfunctions.h"
#include "toolset.h"
#include "target.h"

#include <iostream>

using namespace std;
using namespace Model;
using namespace ToolSet;
using namespace Target;

const QVector3D gray(0.5, 0.5, 0.5);
const QVector3D black(0, 0, 0);
const QVector3D red(1, 0, 0);
const QVector3D green(0, 1, 0);
const QVector3D blue(0, 0, 1);
const QVector3D shaded(0.73, 0.78, 0.91);
const QVector3D darkGray(0.4, 0.4, 0.4);
const QVector3D lightBlue(0, 0.4, 0.7);
const QVector3D white(1, 1, 1);

GLWidget::GLWidget() : QOpenGLWidget(0)
{
    initShaderPrograms();
    initProjections();
    initGrid();
    initAxis();
    initFrame();
}

void GLWidget::initShaderPrograms()
{
    programColor = new QGLShaderProgram;
    programFlat = new QGLShaderProgram;
    programTexture = new QGLShaderProgram;
}

void GLWidget::initProjections()
{
    camera[TOP].setRotation(90, 0, 0);
    camera[BOTTOM].setRotation(-90, 0, 0);
    camera[BACK].setRotation(0, 0, 180);
    camera[LEFT].setRotation(0, 0, 90);
    camera[RIGHT].setRotation(0, 0,-90);
    camera[PERSPECTIVE].setPosition(5 * sinR(30), 5 * cosR(30), 5 * tanR(15));
    camera[PERSPECTIVE].setRotation(15, 0,-120);
}

void GLWidget::initGrid()
{
    for(int i = 0; i < 10; i++)
    {
        line(&grid, {i - 10, -10, 0}, {i - 10, 10, 0}, darkGray);
        line(&grid, {-10, i - 10, 0}, {10, i - 10, 0}, darkGray);
    }

    line(&grid, {-10, 0, 0}, {0, 0, 0}, darkGray);
    line(&grid, {1, 0, 0}, {10, 0, 0}, darkGray);
    line(&grid, {0, -10, 0}, {0, 0, 0}, darkGray);
    line(&grid, {0, 1, 0}, {0, 10, 0}, darkGray);

    for(int i = 0; i < 10; i++)
    {
        line(&grid, {i + 1, -10, 0}, {i + 1, 10, 0}, darkGray);
        line(&grid, {-10, i + 1, 0}, {10, i + 1, 0}, darkGray);
    }
}

void GLWidget::initAxis()
{
    line(&axis, {0, 0, 0}, {1, 0, 0}, red);
    line(&axis, {0, 0, 0}, {0, 1, 0}, green);
    line(&axis, {0, 0, 0}, {0, 0, 1}, blue);
}

void GLWidget::initFrame()
{
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
    glClearColorVector(gray);
    initShaders();
    //if( model->textured ) initTextures();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glGenBuffers(2, modelVboIds);
    glPolygonOffset(-1, -1);
    (new QBasicTimer)->start(0, this);
}

void GLWidget::resizeGL(int newWidth, int newHeight)
{
    _width = newWidth;
    _height = newHeight;
    aspect = _width / double(_height ? _height : 1);
    glViewport(0, 0, _width, _height);
    resizeFrame();
    countMatrices();
}

void GLWidget::resizeFrame()
{
    vector <VertexData_Color> &verticesF = frame.vertices;
    for(int i = 0; i < 4; i++)
    {
        verticesF[i].position.setX((2 * ((i + 1) / 2 % 2) - 1) * _width / 2);
        verticesF[i].position.setY((2 * (i / 2) - 1) * _height / 2);
    }
}

void GLWidget::countMatrices()
{
    frameMatrix.setToIdentity();
    frameMatrix.ortho(-_width / 2, _width / 2,-_height / 2, _height / 2,-1, 1 );
    toolMatrix = frameMatrix;
    toolMatrixInverse = toolMatrix.inverted();
    toolMatrixPerspectiveInverse.setToIdentity();
    toolMatrixPerspectiveInverse.scale(_width / 2, _height / 2, 1);
}

void GLWidget::paintGL()
{
    setupProjection();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, modelVboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelVboIds[1]);


    if(renderingMode != WIREFRAME) drawTriangles();
    else drawSelectedFaces();

    if(renderingMode == WIREFRAME || wireframeOverlay)
    {
        drawVertices();
        drawWireframe();
    }

    drawAxis();
    if(projection == PERSPECTIVE) drawGrid();
    drawFrame();
    if(_isActive && toolIsOn) drawToolLines();
}

void GLWidget::setupProjection()
{
    projectionMatrix.setToIdentity();
    if(projection == PERSPECTIVE) projectionMatrix.perspective(fov, aspect, zNear, zFar);
    else
    {
        projectionMatrix.ortho(-_width / 2, _width / 2,-_height / 2, _height / 2, -1000.f, 1000.f);
        projectionMatrix.scale(scale, scale, 1);
    }
    const QVector3D &rotation = camera[projection].rotation();
    projectionMatrix.rotate(rotation[0] - 90, 1.f, 0.f, 0.f);
    projectionMatrix.rotate(rotation[1], 0.f, 1.f, 0.f);
    projectionMatrix.rotate(90 - rotation[2], 0.f, 0.f, 1.f);
    projectionWithoutTranslation = projectionMatrix;
    projectionMatrix.translate(-camera[projection].position());
}

void GLWidget::drawTriangles()
{
    /*RenderingMode renderingModeCurrent = wireframe ? WIREFRAME : renderingMode;
    if(renderingModeCurrent != WIREFRAME) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_FLAT);
    }
    QVector3D color;
    if(wireframe) color = black;
    if(renderingModeCurrent == FLAT_SHADED || renderingModeCurrent == SMOOTH_SHADED) color = shaded;
    else if(renderingModeCurrent == TEXTURED && !textured()) color = shaded;

    int structSize;
    QString fragment;
    selectedFaces.clear();
    int verticesLength;
    if(renderingModeCurrent == TEXTURED && textured())
    {
        vertices_tex.clear();
        for(int i = 0; i < triangle().size(); i++)
        {
            if(!triangle()[i].exists()) continue;

            if(workWithElements[1]->isChecked() && (triangle()[i].newSelected() || triangle()[i].selected())) addSelectedFace(i);
            else for(int j = 0; j < 3; j++) vertices_tex.push_back({ vertex()[triangle()[i].getIndex(j)].position(), { (rand() % 10) / double(10), (rand() % 10) / double(10) } });
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
        for(int i = 0; i < triangle().size(); i++)
        {
            if(!triangle()[i].exists()) continue;

            if(workWithElements[1]->isChecked() && !wireframe && (triangle()[i].newSelected() || triangle()[i].selected())) addSelectedFace(i);
            else for(int j = 0; j < 3; j++) vertices_col.push_back({ vertex()[triangle()[i].getIndex(j)].position(), color });
        }
        structSize = vertexData_ColorSize;
        fragment = "a_color";
        program = programColor;
        verticesLength = vertices_col.size();
        glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize, vertices_col.data(), GL_STATIC_DRAW);
    }
    indices.resize(verticesLength);
    for(int i = 0; i < verticesLength; i++) indices[i] = i;
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
    glDisable(GL_POLYGON_OFFSET_LINE);*/

    if(renderingMode == FLAT_SHADED) drawFlatShaded();
    if(renderingMode == SMOOTH_SHADED) drawSmoothShaded();
}

void GLWidget::drawFlatShaded()
{
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);

    vector <VertexData_Flat> vertices;
    for(int i = 0; i < triangle().size(); i++)
    {
        if(!triangle()[i].exists()) continue;

        QVector3D v[3];
        for(int j = 0; j < 3; j++) v[j] = vertex()[triangle()[i].getIndex(j)].position();
        QVector3D normal = QVector3D::crossProduct(v[1] - v[0], v[2] - v[0]).normalized();

        QVector3D color;
        if(workWithElements[1]->isChecked() && (triangle()[i].newSelected() || triangle()[i].selected())) color = triangle()[i].newSelected() ? blue : red;
        else color = shaded;


        for(int j = 0; j < 3; j++)
            vertices.push_back({v[j], normal, color});
    }
    int structSize = sizeof(VertexData_Flat);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * structSize, vertices.data(), GL_STATIC_DRAW);

    indices.resize(vertices.size());
    for(int i = 0; i < vertices.size(); i++) indices[i] = i;
    programFlat->bind();

    int positionLocation = programFlat->attributeLocation("a_position");
    programFlat->enableAttributeArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, structSize, 0);

    int normalLocation = programFlat->attributeLocation("a_normal");
    programFlat->enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)vectorSize);

    int colorLocation = programFlat->attributeLocation("a_color");
    programFlat->enableAttributeArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)(2 * vectorSize));

    programFlat->setUniformValue("mvp_matrix", projectionMatrix);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * GLuintSize, indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
}

void GLWidget::drawSmoothShaded()
{
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);

    vector <VertexData_Flat> vertices;

    int vertexSize = 0;
    for(int i = 0; i < vertex().size(); i++)
        if(vertex()[i].exists()) vertexSize++;
    vector<vector<QVector3D> > vertexGroup(vertexSize);

    for(int i = 0; i < triangle().size(); i++)
    {
        if(!triangle()[i].exists()) continue;

        QVector3D v[3];
        for(int j = 0; j < 3; j++) v[j] = vertex()[triangle()[i].getIndex(j)].position();
        QVector3D normal = QVector3D::crossProduct(v[1] - v[0], v[2] - v[0]).normalized();
        for(int j = 0; j < 3; j++)
            vertexGroup[triangle()[i].getIndex(j)].push_back(normal);
    }

    for(int i = 0; i < vertexGroup.size(); i++) {
        QVector3D normal(0, 0, 0);
        for(int j = 0; j < vertexGroup[i].size(); j++)
            normal += vertexGroup[i][j];
        vertexGroup[i][0] = normal / vertexGroup[i].size();
        vertexGroup[i].resize(1);
    }

    for(int i = 0; i < triangle().size(); i++)
    {
        if(!triangle()[i].exists()) continue;

        QVector3D color;
        if(workWithElements[1]->isChecked() && (triangle()[i].newSelected() || triangle()[i].selected())) color = triangle()[i].newSelected() ? blue : red;
        else color = shaded;


        for(int j = 0; j < 3; j++) {
            int index = triangle()[i].getIndex(j);
            vertices.push_back({vertex()[index].position(), vertexGroup[index][0], color});
        }
    }


    int structSize = sizeof(VertexData_Flat);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * structSize, vertices.data(), GL_STATIC_DRAW);

    indices.resize(vertices.size());
    for(int i = 0; i < vertices.size(); i++) indices[i] = i;
    programFlat->bind();

    int positionLocation = programFlat->attributeLocation("a_position");
    programFlat->enableAttributeArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, structSize, 0);

    int normalLocation = programFlat->attributeLocation("a_normal");
    programFlat->enableAttributeArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)vectorSize);

    int colorLocation = programFlat->attributeLocation("a_color");
    programFlat->enableAttributeArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)(2 * vectorSize));

    programFlat->setUniformValue("mvp_matrix", projectionMatrix);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * GLuintSize, indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
}

void GLWidget::drawSelectedFaces()
{
    if(!workWithElements[1]->isChecked()) return;

    programColor->bind();
    prepareProgramColor(projectionMatrix);

    selectedFaces.clear();
    for(int i = 0; i < triangle().size(); i++) if(triangle()[i].exists() && (triangle()[i].newSelected() || triangle()[i].selected())) addSelectedFace(i);

    int selectedFacesLength = selectedFaces.size();
    if(!selectedFacesLength) return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    indices.resize(selectedFacesLength);
    for(int i = 0; i < selectedFacesLength; i++) indices[i] = i;
    glBufferData(GL_ARRAY_BUFFER, selectedFacesLength * vertexData_ColorSize, selectedFaces.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, selectedFacesLength * GLuintSize, indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, selectedFacesLength, GL_UNSIGNED_INT, 0);
}

void GLWidget::drawVertices()
{
    glDisable(GL_CULL_FACE);
    if(!workWithElements[0]->isChecked()) return;

    glPointSize(4);
    programColor->bind();
    prepareProgramColor(projectionMatrix);

    vertices_col.clear();
    for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertices_col.push_back({vertex()[i].position(), vertex()[i].newSelected() ? blue : vertex()[i].selected() ? red : black});
    int vertSize = vertices_col.size();
    indices.resize(vertSize);
    for(int i = 0; i < vertSize; i++) indices[i] = i;

    glBufferData(GL_ARRAY_BUFFER, vertSize * vertexData_ColorSize, vertices_col.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertSize * GLuintSize, indices.data(), GL_STATIC_DRAW);

    glDrawElements(GL_POINTS, vertSize, GL_UNSIGNED_INT, 0);
}

void GLWidget::drawWireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);

    vertices_col.clear();
    for(int i = 0; i < triangle().size(); i++)
    {
        if(triangle()[i].exists()) for(int j = 0; j < 3; j++) vertices_col.push_back({ vertex()[triangle()[i].getIndex(j)].position(), black });
    }
    int structSize = vertexData_ColorSize;
    int verticesLength = vertices_col.size();
    glBufferData(GL_ARRAY_BUFFER, verticesLength * structSize, vertices_col.data(), GL_STATIC_DRAW);


    indices.resize(verticesLength);
    for(int i = 0; i < verticesLength; i++) indices[i] = i;
    programColor->bind();

    int vertexLocation = programColor->attributeLocation("a_position");
    programColor->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, structSize, 0);

    int fragmentLocation = programColor->attributeLocation("a_color");
    programColor->enableAttributeArray(fragmentLocation);
    glVertexAttribPointer(fragmentLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)vectorSize);

    programColor->setUniformValue("mvp_matrix", projectionMatrix);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesLength * GLuintSize, indices.data(), GL_STATIC_DRAW);
//    if(wireframe)

    glEnable(GL_POLYGON_OFFSET_LINE);
    glDrawElements(GL_TRIANGLES, verticesLength, GL_UNSIGNED_INT, 0);
    glDisable(GL_POLYGON_OFFSET_LINE);
}

void GLWidget::addSelectedFace(int num)
{
    QVector3D selectedColor(triangle()[num].newSelected() ? blue : red);
    for(int j = 0; j < 3; j++) selectedFaces.push_back(VertexData_Color(vertex()[triangle()[num].getIndex(j)].position(), selectedColor));
}

void GLWidget::drawAxis()
{

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);


    int structSize = vertexData_ColorSize;
    glBufferData(GL_ARRAY_BUFFER, 6 * structSize, axis.vertices.data(), GL_STATIC_DRAW);

    programColor->bind();

    int vertexLocation = programColor->attributeLocation("a_position");
    programColor->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, structSize, 0);

    int fragmentLocation = programColor->attributeLocation("a_color");
    programColor->enableAttributeArray(fragmentLocation);
    glVertexAttribPointer(fragmentLocation, 3, GL_FLOAT, GL_FALSE, structSize, (void*)vectorSize);

    programColor->setUniformValue("mvp_matrix", projectionMatrix);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * GLuintSize, axis.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
}

void GLWidget::drawGrid()
{
    prepareProgramColor(projectionMatrix);
    glBufferData(GL_ARRAY_BUFFER, 88 * vertexData_ColorSize, grid.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 88 * GLuintSize, grid.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 88, GL_UNSIGNED_INT, 0);
}

void GLWidget::drawFrame()
{
    QVector3D color;
    if(_isActive)
    {
        color = lightBlue;
        glLineWidth(4);
    }
    else
    {
        color = black;
        glLineWidth(2);
    }

    for(int i = 0; i < 4; i++) frame.vertices[i].color = color;
    prepareProgramColor(frameMatrix);
    glBufferData(GL_ARRAY_BUFFER, 4 * vertexData_ColorSize, frame.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * GLuintSize, frame.indices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
}

void GLWidget::timerEvent(QTimerEvent *event) { update(); }

void GLWidget::initShaders()
{
    if(!programColor->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/color.vert")) close();
    if(!programColor->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/color.frag")) close();

    if(!programFlat->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/flat_shaded.vert")) close();
    if(!programFlat->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/flat_shaded.frag")) close();

    if(!programTexture->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/texture.vert")) close();
    if(!programTexture->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/texture.frag")) close();
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
    if(!_isActive) Workspace::setActiveWidget(this);

    if(!toolIsOn) startPosition = lastPosition = QVector2D(event->x() - _width / 2, _height / 2 - event->y());
    if(quickAccess) return;

    switch(event->buttons())
    {
    case Qt::LeftButton:
    {
        Tool *aT = activeTool();
        //  ! stage2     hasStage2 == true && stage2 == false
        //               hasStage2 == false
        //  hasStage2 ? !stage2 : true
        if(aT->hasStage2() ? !aT->stage2() : true) aT->function(START, event);

        break;
    }
    case Qt::RightButton:
    {
        quickAccessToolOrbit();
        quickAccess = true;
        break;
    }
    case Qt::MiddleButton:
    {
        quickAccessToolPan();
        quickAccess = true;
    }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton || quickAccess || activeTool()->stage2())
    {
        toolIsOn = true;
        activeTool()->function(EXECUTE, event);
        lastPosition = currentPosition = QVector2D(event->x() - _width / 2, _height / 2 - event->y());
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(quickAccess)
    {        toolIsOn = false;
        stopQuickAccess();
        quickAccess = false;
    }
    else if(event->button() == Qt::LeftButton)
    {
        toolIsOn = false;
        Tool *aT = activeTool();
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
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, vertexData_ColorSize, (void*)vectorSize);
    programColor->setUniformValue("mvp_matrix", matrix);
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
    _fromScreenToWorld(answer, QVector4D(event->x() - _width / 2, _height / 2 - event->y(), 0, 1), forcedHeight, height);
}

void GLWidget::_fromScreenToWorld(QVector3D *answer, const QVector4D &screenCoordinates, bool forcedHeight, double height)
{
    QVector4D worldCoordinates;
    if(projection == PERSPECTIVE)
    {
        TwoDimArray a(finalMatrixInverse.data());

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

void GLWidget::screenCoordinatesPerspective(QVector4D *answer, const TwoDimArray &a, double h, const QVector4D &screenCoordinates)
{
    double x = screenCoordinates.x(), y = screenCoordinates.y();
    double w = (a(2, 2) - a(3, 2) * h) / ((a(3, 0) * a(2, 2) - a(3, 2) * a(2, 0)) * x + (a(3, 1) * a(2, 2) - a(3, 2) * a(2, 1)) * y + a(3, 3) * a(2, 2) - a(3, 2) * a(2, 3));
    *answer = QVector4D(screenCoordinates.x(), screenCoordinates.y(), (h - (a(2, 0) * x + a(2, 1) * y + a(2, 3)) * w) / a(2, 2), w);
    for(int i = 0; i < 2; i++) (*answer)[i] *= w;
}

bool GLWidget::isSelected(const QVector3D &vertex, const QVector2D &min, const QVector2D &max)
{
    QVector2D result;
    fromWorldToScreen(&result, vertex);
    //if point on screen is inside rectangle, it's selected
    return result.x() > min.x() && result.x() < max.x() && result.y() > min.y() && result.y() < max.y();
}

void GLWidget::line(VertexAndIndexData *data, QVector3D a, QVector3D b, QVector3D color)
{
    data->vertices.push_back({a, color});
    data->vertices.push_back({b, color});
    for(int i = 0; i < 4; i++) data->indices.push_back(data->indices.size());
}

void GLWidget::drawToolLines()
{
    toolData.vertices.clear();
    toolData.indices.clear();
    activeTool()->function(DRAW);
    vector <VertexData_Color> &vertices = toolData.vertices;
    vector <GLuint> &indices = toolData.indices;
    for(int i = 0; i < vertices.size(); i++) vertices[i].color = white;
    prepareProgramColor(toolMatrix);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * vertexData_ColorSize, vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * GLuintSize, indices.data(), GL_STATIC_DRAW);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(1);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}
