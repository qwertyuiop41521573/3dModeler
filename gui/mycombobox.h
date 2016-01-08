#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>

class MyComboBox : public QComboBox
{
public:
    MyComboBox() { setMaximumWidth(150); }
};

#endif // MYCOMBOBOX_H
