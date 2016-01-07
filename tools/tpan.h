#ifndef TPAN_H
#define TPAN_H

#include "tool.h"

class TPan : public Tool
{
public:
    TPan();

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TPAN_H
