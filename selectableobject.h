#ifndef SELECTABLEOBJECT_H
#define SELECTABLEOBJECT_H

class SelectableObject
{
public:
    SelectableObject();

    void setSelected(bool value)
    { _selected = value; };

    void setNewSelected(bool value)
    { _newSelected = value; };

    void setSelected(bool selected, bool newSelected);

    bool selected()
    { return _selected; };

    bool newSelected()
    { return _newSelected; };

private:
    //it's more logical to create "enum { YES, NO, NEW } SelectedState" instead of this 2 bools, but then some things in TSelect won't work - vertices that enter selection rectangle become blue (newSelected), but when they leave rectangle it's unknown what color they had (red or black - selected or not) before entering rectangle
    bool _selected = false;
    bool _newSelected = false;
};

#endif // SELECTABLEOBJECT_H
