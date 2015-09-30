#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>

#include "glwidget.h"

struct List;
struct WidgetElements;

void setupSpinBox( QDoubleSpinBox *spinBox, double defaultValue );



void toolMoveFunction( GLWidget *widget, Action action, QMouseEvent
                      *event = 0, VertexAndIndexData *data = 0 );
void toolScaleFunction( GLWidget *widget, Action action, QMouseEvent
                       *event, VertexAndIndexData *data );
void toolRotateFunction( GLWidget *widget, Action action, QMouseEvent
                         *event, VertexAndIndexData *data );
void toolVertexFunction( GLWidget *widget, Action action, QMouseEvent
                         *event, VertexAndIndexData *data = 0 );
void toolTriangleFunction( GLWidget *widget, Action action,
             QMouseEvent *event, VertexAndIndexData *data = 0 );
void toolPlaneFunction( GLWidget *widget, Action action, QMouseEvent
                      *event, VertexAndIndexData *data = 0 );
void toolBoxFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data = 0 );
void toolEllipseFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data = 0 );
void toolCylinderFunction( GLWidget *widget, Action action, QMouseEvent *event, VertexAndIndexData *data = 0 );
bool isSelected( QMatrix4x4 finalMatrix, QVector3D vertex,
                 bool perspective, QVector2D min, QVector2D max );
bool getAxis( WidgetElements *toolElements, QMatrix4x4 *rotation,
              double angle );
QVector3D fromScreenToWorld_xy( double x, double y, GLWidget *widget );
QVector3D fromScreenToWorld_vector( QVector2D vector, GLWidget *widget );
QVector3D fromScreenToWorld( QMouseEvent *event, GLWidget *widget, bool forcedHeight = false, double height = 0 );
QVector3D _fromScreenToWorld( QVector4D screenCoordinates, GLWidget *widget, bool forcedHeight = false, double height = 0 );
double inRadians( double value );
QVector4D screenCoordinatesPerspective( double a[ 4 ][ 4 ], double h, QVector4D screenCoordinates );
int sign( double number );
void createEllipseCap( QVector4D rotatingVertex, double angle, QVector3D normal, Model *model, int vertexSize, int segments, QMatrix4x4 scaleAndTranslate );

#endif // FUNCTIONS_H

