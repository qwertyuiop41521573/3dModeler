#ifndef WIDGETELEMENTS_H
#define WIDGETELEMENTS_H

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include "gui/mycheckbox.h"

class WidgetElements
{
public:
    WidgetElements( int newPushButtonNumber, int newSpinBoxNumber,
            int newRadioButtonNumber, int newMyCheckBoxNumber, int newCheckBoxNumber,
                    double defaultValue = 0 );

    QPushButton *getPushButton( int index )
    { return pushButton[ index ]; };

    QDoubleSpinBox *getSpinBox( int index )
    { return spinBox[ index ]; };

    QRadioButton *getRadioButton( int index )
    { return radioButton[ index ]; }

    MyCheckBox *getMyCheckBox( int index )
    { return myCheckBox[ index ]; };

    QCheckBox *getCheckBox( int index )
    { return checkBox[ index ]; };

private:
    int pushButtonNumber, spinBoxNumber, radioButtonNumber;
    int myCheckBoxNumber, checkBoxNumber;
    QPushButton **pushButton;
    QDoubleSpinBox **spinBox;
    QRadioButton **radioButton;
    MyCheckBox **myCheckBox;
    QCheckBox **checkBox;
};

#endif // WIDGETELEMENTS_H
