#include "mathfunctions.h"

#include <QLayout>
#include <QDoubleSpinBox>
#include <iostream>

using namespace std;

double inRadians( double value )
{
    return value * M_PI / double( 180 );
}

int sign( double number )
{
    return ( number > 0 ) ? 1 : -1;
}

double cosR(double x)
{
    return cos(inRadians(x));
}

double sinR(double x)
{
    return sin(inRadians(x));
}
