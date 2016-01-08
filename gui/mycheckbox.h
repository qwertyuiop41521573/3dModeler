#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include "mycheckboxmw.h"

class MyCheckBox : public MyCheckBoxMW
{
    Q_OBJECT
public:
    explicit MyCheckBox()
    { connect(this, SIGNAL(clicked(bool)), this, SLOT(handleClick(bool))); }

    void setNeighbor(QCheckBox *neighbor) { _neighbor = neighbor; }

private:
    QCheckBox *_neighbor;

private slots:
    void handleClick(bool value) { if(value) _neighbor->setChecked(false); }
};

#endif // MYCHECKBOX_H
