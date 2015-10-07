#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QBasicTimer>

#include "tool.h"
#include "model.h"

using namespace std;

typedef enum { WIREFRAME, FLAT_SHADED, SMOOTH_SHADED, TEXTURED } RenderingMode;

typedef enum { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT, PERSPECTIVE } Projection;

class VertexData_Color
{
public:
    VertexData_Color(QVector3D pos = { 0, 0, 0 }, QVector3D col = { 0, 0, 0 })
    { position = pos; color = col; };

    QVector3D position;
    QVector3D color;
};

struct VertexData_Texture
{
    QVector3D position;
    QVector2D texCoord;
};

struct VertexAndIndexData
{
    vector <VertexData_Color> vertices;
    vector <GLuint> indices;
};

class MainWindow;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(MainWindow *mainWindow, QWidget *parent = 0);

    void setActive(bool value)
    { _isActive = value; };

    void setRenderingMode(RenderingMode newRenderingMode)
    { renderingMode = newRenderingMode; };

    RenderingMode getRenderingMode()
    { return renderingMode; };

    bool getWireframeOverlay()
    { return wireframeOverlay; };

    void setWireframeOverlay(bool value)
    { wireframeOverlay = value; };

    const QVector2D &getLastPosition()
    { return lastPosition; };

    const QVector2D &getStartPosition()
    { return startPosition; };

    const QVector2D &getCurrentPosition()
    { return currentPosition; };

    void setProjection(Projection newProjection)
    { projection = newProjection; };

    Projection getProjection()
    { return projection; };

    void multiplyScaleBy(double number)
    { scale *= number; };

    double getScale()
    { return scale; };

    const QMatrix4x4 &getFinalMatrix()
    { return finalMatrix; };

    const QMatrix4x4 &getFinalInverseMatrix()
    { return finalMatrixInverse; };



    const QMatrix4x4 &getRotationMatrix()
    { return rotationMatrix; };

    const QMatrix4x4 &getRotationMatrixInverse()
    { return rotationMatrixInverse; };

    void setOldHidden(bool value)
    { _oldHidden = value; };

    bool oldHidden()
    { return _oldHidden; };

    int getHalfWidth()
    { return halfWidth; };

    int getHalfHeight()
    { return halfHeight; };

    Camera &getCamera()
    { return camera[projection]; };

    void setToolIsOn(bool value)
    { toolIsOn = value; };

    VertexAndIndexData &getToolData()
    { return toolData; };


    void countRotationMatrices();
    void countFinalInverseMatrix();
    void countFinalMatrix()
    { finalMatrix = (projection == PERSPECTIVE ? toolMatrixPerspectiveInverse : toolMatrixInverse) * projectionMatrix; };


    void fromWorldToScreen(QVector2D &answer, const QVector3D &vector, bool point);
    void fromScreenToWorld(QVector3D &answer, QMouseEvent *event, bool forcedHeight = false, double height = 0);
    void _fromScreenToWorld(QVector3D &answer, const QVector4D &screenCoordinates, bool forcedHeight = false, double height = 0);
    void screenCoordinatesPerspective(QVector4D &answer, double a[4][4], double h, const QVector4D &screenCoordinates);
    bool isSelected(const QVector3D &vertex, const QVector2D &min, const QVector2D &max);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void initShaders();
   // void initTextures();

public slots:

private:
    MainWindow *_mainWindow;
    Model *model;
    Tool **activeTool;
    QRadioButton **workWithElements;

    int width, height;
    int halfWidth, halfHeight;
    RenderingMode renderingMode = WIREFRAME;
    bool wireframeOverlay = false;
    Projection projection;
    Camera camera[7];
    double scale = 100;

    QBasicTimer *timer;
    QGLShaderProgram *programColor, *programTexture, *program;



    QMatrix4x4 projectionMatrix, projectionWithoutTranslation;
    QMatrix4x4 rotationMatrix, rotationMatrixInverse;
    QMatrix4x4 frameMatrix;
    QMatrix4x4 finalMatrix, toolMatrix, toolMatrixPerspectiveInverse;
    QMatrix4x4 toolMatrixInverse, finalMatrixInverse;
    qreal aspect;
    const qreal zNear = 0.1, zFar = 1000, fov = 45.0;    

    QVector2D lastPosition, currentPosition, startPosition;

    bool _isActive = false;
    bool quickAccess = false;
    bool toolIsOn = false;

    GLuint texture;
    GLuint modelVboIds[2];

    const void *vectorSize = (const void*)sizeof(QVector3D);
    int vertexData_ColorSize = sizeof(VertexData_Color);
    int vertexData_TextureSize = sizeof(VertexData_Texture);
    int GLuintSize = sizeof(GLuint);

    VertexAndIndexData grid, axis, frame;

    vector <VertexData_Texture> vertices_tex;
    vector <VertexData_Color> vertices_col;
    vector <VertexData_Color> selectedFaces;
    vector <GLuint> indices;


    bool _oldHidden;
    VertexAndIndexData toolData;

    void draw(bool wireframe = false);
    void setupProjection();

    void glClearColorVector(const QVector3D &vector)
    { glClearColor(vector.x(), vector.y(), vector.z(), 1); };

    void drawAdittional();
    void prepareProgramColor(const QMatrix4x4 &matrix);
    void addSelectedFace(int num);
};

#endif // GLWIDGET_H

