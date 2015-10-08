#include "selectableobject.h"

SelectableObject::SelectableObject()
{

}

void SelectableObject::setSelected(bool selected, bool newSelected)
{
    _selected = selected;
    _newSelected = newSelected;
}
