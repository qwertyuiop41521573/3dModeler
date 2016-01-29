#ifndef ELEMENT_H
#define ELEMENT_H

//Vertex and Triangle are inherited from it
class Element
{
public:
    Element();

    void setSelected(bool value) { _selected = value; }
    void setNewSelected(bool value) { _newSelected = value; }
    void setSelected(bool selected, bool newSelected);
    bool selected() const { return _selected; }
    bool newSelected() const { return _newSelected; }

    void operator =(const Element &element);

    bool exists() const { return _exists; }
    virtual void remove() { _exists = false; }
    void undoRemove() { _exists = true; }

    bool isVertex() const { return _isVertex; }

protected:
    bool _isVertex;

private:
    //it's more logical to create "enum { YES, NO, NEW } SelectedState" instead of this 2 bools, but then some things in TSelect won't work - vertices that enter selection rectangle become blue (newSelected), but when they leave rectangle it's unknown what color they had (red or black - selected or not) before entering rectangle
    bool _selected;
    bool _newSelected;
    bool _exists;
};

#endif // ELEMENT_H
