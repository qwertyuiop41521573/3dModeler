#include "vertexcontainer.h"
#include "journal.h"
#include "model.h"

#include <iostream>

using namespace std;

int VertexContainer::push(const Vertex &vertex)
{
    Model::modify();
    int index;
    int i;

    //find free place (some element marked as "not exists") for new element
    for(i = 0; i < size(); i++)
    {
        if(at(i).exists()) continue;
        
        at(i) = vertex;
        index = i;
        break;
    }

    //if no such place found
    if(i == size())
    {
        push_back(vertex);
        index = size() - 1;
    }

    //record to journal
    //if(t.isVertex())
    Journal::addVertex(index);
    //else Journal::addTriangle(index);
    return index;
}


void VertexContainer::remove(int index)
{
    Model::modify();
    //"element.remove(i)" is the same as "element[i].remove()", but records to journal
    //Journal::addBefore(at(0).isVertex(), index);
    Journal::addVertex(index);
    at(index).remove();
}

void VertexContainer::setSelected(int index, bool value)
{
    //same as remove
    if(at(index).selected() == value) return;

    Journal::addBefore(at(0).isVertex(), index);
    at(index).setSelected(value);
    Journal::addAfter(at(0).isVertex());
}
