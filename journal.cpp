#include "journal.h"
#include "model.h"

Journal::Journal()
{

}

void Journal::newRecord(Type type)
 {
    currentType = type;
    vertexList.clear();
};

void Journal::add(int index)
{
    vertexList.push_back(index);
}

void Journal::submit()
{
    erase(begin() + _current + 1, end());
    push_back(currentType);
    _current++;
    int i;
    switch(currentType)
    {
    case CREATE:
    {
        for(i = 0; i < vertexList.size(); i++) at(size() - 1).data().createOrRemove->push_back({_model->getVertex()[vertexList[i]], vertexList[i]});
        break;
    }
    case REMOVE:
    {
        vector <VertexAndIndex> &data = *at(size() - 1).data().createOrRemove;
        for(i = 0; i < vertexList.size(); i++)
        {
            data.push_back({_model->getVertex()[vertexList[i]], vertexList[i]});
            data[data.size() - 1].vertex.undoRemove();
        }
        break;
    }
    case SELECT:
    {
        for(i = 0; i < vertexList.size(); i++) at(size() - 1).data().select->push_back({_model->getVertex()[vertexList[i]].selected(), vertexList[i]});
        break;
    }
    }
}

void Journal::cancelRecord()
{
    //cancelling for empty records (add was not used)

}
