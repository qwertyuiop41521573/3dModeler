#include "vertexcontainer.h"
#include "journal.h"

VertexContainer::VertexContainer(Journal *journal)// : vector <Vertex>
{
    _journal = journal;
}

int VertexContainer::push(Vertex vertex)
{
    int index;
    int i;
    for(i = 0; i < size(); i++)
    {
        if(!at(i).exists())
        {
            at(i) = vertex;
            index = i;
            break;
        }
    }
    if(i == size())
    {
        push_back(vertex);
        index = size() - 1;
    }
    _journal->add(index);
    return index;
}

void VertexContainer::remove(int index)
{
    _journal->add(index);
    at(index).remove();
}
