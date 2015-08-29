#include "widgetelements.h"
#include "functions.h"

WidgetElements::WidgetElements( int newPushButtonNumber,
                  int newSpinBoxNumber, int newRadioButtonNumber,
                  int newMyCheckBoxNumber, int newCheckBoxNumber, double defaultValue )
{
    pushButtonNumber = newPushButtonNumber;
    spinBoxNumber = newSpinBoxNumber;
    radioButtonNumber = newRadioButtonNumber;
    myCheckBoxNumber = newMyCheckBoxNumber;
    checkBoxNumber = newCheckBoxNumber;

    pushButton = new QPushButton*[ pushButtonNumber ];
    spinBox = new QDoubleSpinBox*[ spinBoxNumber ];
    radioButton = new QRadioButton*[ radioButtonNumber ];
    myCheckBox = new MyCheckBox*[ myCheckBoxNumber ];
    checkBox = new QCheckBox*[ checkBoxNumber ];

    int i;
    for( i = 0; i < pushButtonNumber; i++ )
    {
        pushButton[ i ] = new QPushButton;
        setupButton( pushButton[ i ] );
    }
    for( i = 0; i < spinBoxNumber; i++ )
    {
        spinBox[ i ] = new QDoubleSpinBox;
        setupSpinBox( spinBox[ i ], defaultValue );
    }
    for( i = 0; i < radioButtonNumber; i++ )
        radioButton[ i ] = new QRadioButton;
    for( i = 0; i < myCheckBoxNumber; i++ )
        myCheckBox[ i ] = new MyCheckBox;
    for( i = 0; i < checkBoxNumber; i++ ) checkBox[ i ] = new QCheckBox;
}

