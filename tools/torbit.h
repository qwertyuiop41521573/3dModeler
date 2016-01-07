#ifndef TORBIT_H
#define TORBIT_H

#include "tool.h"

class TOrbit : public Tool
{
public:
    TOrbit();

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TORBIT_H
