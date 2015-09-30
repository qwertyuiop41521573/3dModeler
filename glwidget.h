#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>

#include "tool.h"
#include "model.h"
#include "camera.h"

#include <iostream>

using namespace std;

typedef enum { WIREFRAME, FLAT_SHADED, SMOOTH_SHADED,
               TEXTURED } RenderingMode;

typedef enum { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT,
               PERSPECTIVE } Projection;

struct VertexData_Texture
{
    QVector3D position;
    QVector2D texCoord;
};

class MainWindow;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(MainWindow *mainWindow, QWidget *parent = 0);

    void setActive( bool value )
    { _isActive = value; };

    void setRenderingMode( RenderingMode newRenderingMode )
    { renderingMode = newRenderingMode; };

    RenderingMode getRenderingMode()
    { return renderingMode; };

    bool getWireframeOverlay()
    { return wireframeOverlay; };

    void setWireframeOverlay( bool value )
    { wireframeOverlay = value; };

    Tool *getActiveTool()
    { return *activeTool; };

    WidgetElements *getWorkWithElements()
    { return workWithElements; };

    QVector2D getLastPosition()
    { return lastPosition; };

    QVector2D getStartPosition()
    { return startPosition; };

    QVector2D getCurrentPosition()
    { return currentPosition; };

    void setProjection( Projection newProjection )
    { projection = newProjection; };

    Projection getProjection()
    { return projection; };

    Model *getModel()
    { return model; };

    void multiplyScaleBy( double number )
    { scale *= number; };

    double getScale()
    { return scale; };

    QMatrix4x4 getFinalMatrix()
    { return finalMatrix; };

    QMatrix4x4 getFinalInverseMatrix()
    { return finalMatrixInverse; };

    void setPivot( QVector3D newPivot )
    { pivot = newPivot; };

    QVector3D getPivot()
    { return pivot; };

    QVector2D getPivotTransformed()
    { return pivotTransformed; };

    void setPivotTransformed( QVector2D vector )
    { pivotTransformed = vector; };

    void countFinalMatrix( bool perspective )
    { finalMatrix = ( perspective ? toolMatrixPerspectiveInverse :
        toolMatrixInverse ) * projectionMatrix; };

    QMatrix4x4 getRotationMatrix()
    { return rotationMatrix; };

    QMatrix4x4 getRotationMatrixInverse()
    { return rotationMatrixInverse; };

    void setOldHidden( bool value )
    { _oldHidden = value; };

    bool oldHidden()
    { return _oldHidden; };

    int getHalfWidth()
    { return halfWidth; };

    int getHalfHeight()
    { return halfHeight; };

    Camera *getCamera()
    { return &camera[ projection ]; };

    void setCamera( Camera newCamera )
    { camera[ projection ] = newCamera; };

    void setToolIsOn( bool value)
    { toolIsOn = value; };

    QVector3D startPosition3D()
    { return _startPosition3D; };

    void setStartPosition3D( QVector3D vector )
    { _startPosition3D = vector; };

    VertexAndIndexData *getToolData()
    { return &toolData; };

    QVector2D transform( QVector3D vector, bool point );
    void countFinalInverseMatrix( bool perspective );
    void countRotationMatrices();
    void setCurrentPosition( double x, double y );

protected:
    void initializeGL();
    void resizeGL( int newWidth, int newHeight );
    void paintGL();

    void timerEvent( QTimerEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void wheelEvent( QWheelEvent *event );

    void initShaders();
   // void initTextures();

public slots:

private:
    MainWindow *_mainWindow;





    int width, height;
    int halfWidth, halfHeight;
    qreal aspect;
    QBasicTimer *timer;
    QGLShaderProgram *programColor, *programTexture, *program;
    Model *model;
    GLuint texture;
    QMatrix4x4 projectionMatrix, projectionWithoutTranslation;
    QMatrix4x4 rotationMatrix, rotationMatrixInverse;
    QMatrix4x4 frameMatrix;
    QMatrix4x4 finalMatrix, toolMatrix, toolMatrixPerspectiveInverse;
    QMatrix4x4 toolMatrixInverse, finalMatrixInverse;
    const qreal zNear = 0.1, zFar = 1000, fov = 45.0;
    bool _isActive = false;
    RenderingMode renderingMode = WIREFRAME;
    bool wireframeOverlay = false;
    Tool **activeTool;
    WidgetElements *workWithElements;
    QVector2D lastPosition, currentPosition, startPosition;
    QVector3D _startPosition3D;
    Projection projection;
    Camera camera[ 7 ];
    double scale = 100;
    bool quickAccess = false;
    bool toolIsOn = false;
    GLuint modelVboIds[ 2 ];
    QVector3D gray = { 0.5, 0.5, 0.5 };
    QVector3D black = { 0, 0, 0 };
    QVector3D red = { 1, 0, 0 };
    QVector3D green = { 0, 1, 0 };
    QVector3D blue = { 0, 0, 1 };
    QVector3D shadedColor = { 0.73, 0.78, 0.91 };
    QVector3D darkGray = { 0.4, 0.4, 0.4 };
 //   QVector3D lightBlue = { 0.07, 0.47, 0.75 };
    QVector3D lightBlue = { 0, 0.4, 0.7 };
    QVector3D white = { 1, 1, 1 };
    QVector3D sRed = { 0.7, 0, 0 };
    QVector3D sBlue = { 0, 0, 0.7 };
    const void *vectorSize = ( const void* )sizeof( QVector3D );
    int vertexData_ColorSize = sizeof( VertexData_Color );
    int vertexData_TextureSize = sizeof( VertexData_Texture );
    int GLushortSize = sizeof( GLushort );
    VertexAndIndexData grid, axis, frame;
    QVector3D pivot;
    QVector2D pivotTransformed;

    vector <VertexData_Texture> vertices_tex;
    vector <VertexData_Color> vertices_col;
    vector <VertexData_Color> selectedFaces;

    bool _oldHidden;
    VertexAndIndexData toolData;

    void draw( bool wireframe = false );
    void setupProjection();

    void glClearColorVector( QVector3D vector )
    { glClearColor( vector.x(), vector.y(), vector.z(), 1 ); };

    void drawAdittional();
    void prepareProgramColor( QMatrix4x4 matrix );
    void addSelectedFace( int num );
};

#endif // GLWIDGET_H

