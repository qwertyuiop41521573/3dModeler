#include "element.h"

Element::Element()
{

}

void Element::setSelected(bool selected, bool newSelected)
{
    _selected = selected;
    _newSelected = newSelected;
}

void Element::operator =(const Element &element)
{
    _selected = element.selected();
    _newSelected = element.newSelected();
    _exists = element.exists();
    _isVertex = element.isVertex();
}
