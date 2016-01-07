#ifndef TZOOM_H
#define TZOOM_H

#include "tool.h"

class TZoom : public Tool
{
public:
    TZoom();

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TZOOM_H
