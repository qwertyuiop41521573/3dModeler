#include "element.h"

Element::Element() :
    _selected(false),
    _newSelected(false),
    _exists(true)
{}

void Element::setSelected(bool selected, bool newSelected)
{
    _selected = selected;
    _newSelected = newSelected;
}

void Element::operator =(const Element &element)
{
    _selected = element.selected();
    _newSelected = element.newSelected();
    _isVertex = element.isVertex();
    _exists = element.exists();
}
