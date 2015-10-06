#ifndef SELECTABLEOBJECT_H
#define SELECTABLEOBJECT_H

typedef enum { NO, YES, NEW } SelectedState;

class SelectableObject
{
public:
    SelectableObject();

    void select(SelectedState state = YES)
    { _selected = state; };

    void select(bool state)
    { _selected = SelectedState(state); };

    void deselect()
    { _selected = NO; };

    bool selected()
    { return _selected == YES; };

    bool newSelected()
    { return _selected == NEW; };

private:
    SelectedState _selected;
//    bool _isSelected = false;
 //   bool _newSelected = false;
};

#endif // SELECTABLEOBJECT_H
