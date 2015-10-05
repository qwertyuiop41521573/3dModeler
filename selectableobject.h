#ifndef SELECTABLEOBJECT_H
#define SELECTABLEOBJECT_H


class SelectableObject
{
public:
    SelectableObject();

    void setSelected(bool value)
    { _isSelected = value; };

    bool isSelected()
    { return _isSelected; };

    bool newSelected()
    { return _newSelected; };

    void setNewSelected( bool value )
    { _newSelected = value; };

private:
    bool _isSelected = false;
    bool _newSelected = false;
};

#endif // SELECTABLEOBJECT_H
