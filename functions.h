#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>

#include "glwidget.h"
#include "gui/myspinbox.h"

struct List;
struct WidgetElements;

void setupSpinBox( QDoubleSpinBox *spinBox, double defaultValue );
bool isSelected( QMatrix4x4 finalMatrix, QVector3D vertex,
                 bool perspective, QVector2D min, QVector2D max );
bool getAxis( MySpinBox **spinBox, QMatrix4x4 *rotation,
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

