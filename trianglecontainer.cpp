#include "trianglecontainer.h"
#include "triangle.h"
#include "journal.h"
#include "model.h"

TriangleContainer::TriangleContainer()
{
}

tr_it TriangleContainer::push(const Triangle &triangle)
{
    Model::modify();
   /* int i;

    //find free place (some element marked as "not exists") for new element
    for(i = 0; i < size(); i++)
    {
        if(at(i).exists()) continue;

        at(i) = t;
        index = i;
        break;
    }

    //if no such place found
    if(i == size())
    {
        push_back(t);
        index = size() - 1;
    }*/

    push_back(triangle);

    //record to journal
    Journal::addTriangle(--end());
    return --end();
}

void TriangleContainer::rem(tr_it it)
{
    Model::modify();
    //"element.remove(i)" is the same as "element[i].remove()", but records to journal
    Journal::addBefore(it);
    it->remove();
    Journal::addAfter(false);
}

void TriangleContainer::setSelected(tr_it it, bool value)
{
    //same as remove
    if(it->selected() == value) return;

    Journal::addBefore(it);
    it->setSelected(value);
    Journal::addAfter(false);
}
