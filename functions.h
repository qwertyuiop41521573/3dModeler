#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>

#include "glwidget.h"
#include "gui/myspinbox.h"

struct List;

bool isSelected(QMatrix4x4 finalMatrix, QVector3D vertex, bool perspective,
                QVector2D min, QVector2D max);
QVector3D fromScreenToWorld( QMouseEvent *event, GLWidget *widget, bool forcedHeight = false, double height = 0 );
QVector3D _fromScreenToWorld( QVector4D screenCoordinates, GLWidget *widget, bool forcedHeight = false, double height = 0 );
double inRadians( double value );
QVector4D screenCoordinatesPerspective( double a[ 4 ][ 4 ], double h, QVector4D screenCoordinates );
int sign( double number );

#endif // FUNCTIONS_H

