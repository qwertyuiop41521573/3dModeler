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

struct VertexData_Color
{
    VertexData_Color(QVector3D pos = { 0, 0, 0 }, QVector3D col = { 0, 0, 0 })
    { position = pos; color = col; }

    QVector3D position;
    QVector3D color;
};

struct VertexData_Flat
{
    QVector3D position;
    QVector3D normal;
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

class TwoDimArray
{
public:
    TwoDimArray(float *data) { _data = data; }

    float operator()(int ind1, int ind2) const
    { return _data[4 * ind2 + ind1]; }

private:
    float *_data;
};

class AdditiveMap : public map<int, QVector3D>
{
public:
    void push(int smoothingGroup, const QVector3D &normal)
    {
        if(!count(smoothingGroup))
            insert(pair<int, QVector3D>(smoothingGroup, normal));
        else at(smoothingGroup) += normal;
    }
};

class MainWindow;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget();

    void setActive(bool value) { _isActive = value; }

    void setRenderingMode(RenderingMode newRenderingMode)
    { renderingMode = newRenderingMode; }

    RenderingMode getRenderingMode() { return renderingMode; }
    bool getWireframeOverlay() { return wireframeOverlay; }
    void setWireframeOverlay(bool value) { wireframeOverlay = value; }
    const QVector2D &getLastPosition() { return lastPosition; }
    const QVector2D &getStartPosition() { return startPosition; }
    const QVector2D &getCurrentPosition() { return currentPosition; }

    void setProjection(Projection newProjection)
    { projection = newProjection; }

    Projection getProjection() { return projection; }
    void multiplyScaleBy(double number) { scale *= number; }
    double getScale() { return scale; }
    const QMatrix4x4 &getFinalMatrix() { return finalMatrix; }
    const QMatrix4x4 &getFinalInverseMatrix()
    { return finalMatrixInverse; }

    const QMatrix4x4 &getRotationMatrix() { return rotationMatrix; }

    const QMatrix4x4 &getRotationMatrixInverse()
    { return rotationMatrixInverse; }

    void setOldHidden(bool value) { _oldHidden = value; }
    bool oldHidden() { return _oldHidden; }
    int getHalfWidth() { return _width / 2; }
    int getHalfHeight() { return _height / 2; }
    Camera &getCamera() { return camera[projection]; }
    void setToolIsOn(bool value) { toolIsOn = value; }
    VertexAndIndexData &getToolData() { return toolData; }


    void countRotationMatrices();
    void countFinalInverseMatrix();
    void countFinalMatrix()
    { finalMatrix = (projection == PERSPECTIVE ? toolMatrixPerspectiveInverse : toolMatrixInverse) * projectionMatrix; }


    void fromWorldToScreen(QVector2D *answer, const QVector3D &vector, bool point = true);
    void fromScreenToWorld(QVector3D *answer, QMouseEvent *event, bool forcedHeight = false, double height = 0);
    void _fromScreenToWorld(QVector3D *answer, const QVector4D &screenCoordinates, bool forcedHeight = false, double height = 0);
    void screenCoordinatesPerspective(QVector4D *answer, const TwoDimArray &a, double h, const QVector4D &screenCoordinates);
    bool isSelected(const QVector3D &vertex, const QVector2D &min, const QVector2D &max);

protected:


public slots:

private:
    int _width, _height;
    RenderingMode renderingMode = WIREFRAME;
    bool wireframeOverlay = false;
    Projection projection;
    Camera camera[7];
    double scale = 100;

    QGLShaderProgram *programColor, *programShaded, *programTexture;

    QMatrix4x4 projectionMatrix, projectionWithoutTranslation;
    QMatrix4x4 rotationMatrix, rotationMatrixInverse;
    QMatrix4x4 frameMatrix;
    QMatrix4x4 finalMatrix, toolMatrix, toolMatrixPerspectiveInverse;
    QMatrix4x4 toolMatrixInverse, finalMatrixInverse;
    double aspect;

    QVector2D lastPosition, currentPosition, startPosition;

    bool _isActive = false;
    bool quickAccess = false;
    bool toolIsOn = false;

    GLuint texture;
    GLuint modelVboIds[2];

    VertexAndIndexData grid, axis, frame;
    vector <VertexData_Color> selectedFaces;

    bool _oldHidden;
    VertexAndIndexData toolData;


        void initShaderPrograms();
        void initProjections();
        void initGrid();
            void line(VertexAndIndexData *data, QVector3D a, QVector3D b, QVector3D color);
        void initAxis();
        void initFrame();

    void initializeGL();
        void glClearColorVector(const QVector3D &vector)
        { glClearColor(vector.x(), vector.y(), vector.z(), 1); }
        void initShaders();
         // void initTextures();

    void resizeGL(int newWidth, int newHeight);
        void resizeFrame();
        void countMatrices();

    void paintGL();
        void setupProjection();
        void drawTriangles();
        void drawFlatShaded();
        void drawSmoothShaded();
        void drawSelectedFaces();
            void prepareProgramColor(const QMatrix4x4 &matrix);
            void addSelectedFace(int num);
        void drawVertices();
        void drawWireframe();
        void drawAxis();
        void drawGrid();
        void drawFrame();
        void drawToolLines();

    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // GLWIDGET_H

