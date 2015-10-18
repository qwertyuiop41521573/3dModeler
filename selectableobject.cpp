#include "selectableobject.h"

SelectableObject::SelectableObject()
{

}

void SelectableObject::setSelected(bool selected, bool newSelected)
{
    _selected = selected;
    _newSelected = newSelected;
}

void SelectableObject::operator =(const SelectableObject &selObj)
{
    _selected = selObj.selected();
    _newSelected = selObj.newSelected();
}
