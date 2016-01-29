#include "trianglecontainer.h"
#include "triangle.h"
#include "journal.h"
#include "model.h"

tr_it TriangleContainer::push(const Triangle &triangle)
{
    Model::modify();

    //find free place (some element marked as "not exists") for new element
    tr_it iterator;
    tr_it it;
    for(it = begin(); it != end(); it++) {
        if(it->exists()) continue;
        *it = triangle;
        iterator = it;
        break;
    }

    //if no such place found
    if(it == end()) {
        push_back(triangle);
        iterator = --end();
    }

    //record to journal
    iterator->record();
    Journal::addTriangle(iterator);
    return iterator;
}

void TriangleContainer::rem(tr_it it)
{
    Model::modify();
    //"element.remove(i)" is the same as "element[i].remove()", but records to journal
    Journal::addTriangle(it);
    it->remove();
    //erase(it);
    //it->remove();
}

void TriangleContainer::setSelected(tr_it it, bool value)
{
    //same as remove
    if(it->selected() == value) return;

    Journal::addBefore(it);
    it->setSelected(value);
    Journal::addAfter(false);
}
