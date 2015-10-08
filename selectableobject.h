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
    bool _selected = false;
    bool _newSelected = false;
};

#endif // SELECTABLEOBJECT_H
