#ifndef MYCHECKBOXMW_H
#define MYCHECKBOXMW_H

#include <QCheckBox>

class MyCheckBoxMW : public QCheckBox
{
public:
    MyCheckBoxMW() { setMaximumWidth(130); }
};

#endif // MYCHECKBOXMW_H
