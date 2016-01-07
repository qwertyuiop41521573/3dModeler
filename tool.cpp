#include "tool.h"
#include "toolset.h"
#include "workspace.h"

Tool::Tool()
{
    button = new QPushButton;
    button->setMaximumWidth(70);
    button->setCheckable(true);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(handleClick(bool)));
}

void Tool::setActive(bool value)
{
    _isActive = value;
    button->setChecked(value);
}

void Tool::handleClick(bool pressed)
{
    if(pressed) ToolSet::setActiveTool(this);
    else button->setChecked(true);
}
