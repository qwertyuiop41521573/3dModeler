#include "mypushbutton.h"

MyPushButton::MyPushButton( int newIndex, QWidget *parent ) :
    QPushButton( parent )
{
    setCheckable( true );
    setMaximumWidth( 30 );
    index = newIndex;
    connect( this, SIGNAL( toggled( bool ) ), this,
            SLOT( handleClick() ) );
}
