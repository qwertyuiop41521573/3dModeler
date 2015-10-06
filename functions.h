#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>

#include "glwidget.h"
#include "gui/myspinbox.h"


struct List;

bool isSelected(const QMatrix4x4 &finalMatrix, const QVector3D &vertex, bool perspective, const QVector2D &min, const QVector2D &max);
void fromScreenToWorld(QVector3D &answer, QMouseEvent *event, GLWidget *widget, bool forcedHeight = false, double height = 0);
void _fromScreenToWorld(QVector3D &answer, const QVector4D &screenCoordinates, GLWidget *widget, bool forcedHeight = false, double height = 0);
double inRadians( double value );
void screenCoordinatesPerspective(QVector4D &answer, double a[ 4 ][ 4 ], double h, const QVector4D &screenCoordinates );
int sign( double number );

double cosR(double x);
double sinR(double x);

#endif // FUNCTIONS_H

