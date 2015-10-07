#include "mycheckbox.h"

MyCheckBox::MyCheckBox()
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(handleClick(bool)));
}
