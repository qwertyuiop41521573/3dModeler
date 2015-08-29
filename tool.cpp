#include "tool.h"

Tool::Tool( QString buttonText, toolFunction newFunction,Qt::
    CursorShape cursor, QWidget *widget, WidgetElements
            *newElements, QString finalButtonText, bool hasStage2 )
{
    button = new QPushButton( buttonText );
    setupButton( button );
    connect( button, SIGNAL( clicked( bool ) ), this,
             SLOT( handleClick( bool ) ) );
    if( !finalButtonText.isNull() )
        finalButton = new QPushButton( finalButtonText );
    _hasStage2 = hasStage2;

    function = newFunction;
   // _cursor.setShape( cursor );
    _widget = widget;
    elements = newElements;
}

void Tool::setActive( bool value )
{
    _isActive = value;
    button->setChecked( value );
    if( _widget )
    {
        if( value ) _widget->show();
        else _widget->hide();
    }
}

void Tool::handleClick( bool pressed )
{
    if( pressed ) emit makeMeActive( this );
    else button->setChecked( true );
}

bool Tool::elementsExist()
{
    if( elements == 0 ) return false;
    return true;
}

