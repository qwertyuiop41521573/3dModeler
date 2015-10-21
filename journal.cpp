#include "journal.h"
#include "model.h"

Journal::Journal()
{

}

void Journal::setVariables(Model *model, QRadioButton **workWithElements)
{
    _model = model;
    _workWithElements = workWithElements;
}

void Journal::newRecord(Type type)
 {
    currentType = type;
    if(type == CREATE || type == REMOVE)
    {
        vertexList.clear();
        triangleList.clear();
    }
    if(type == SELECT) list.clear();
};

void Journal::addVertex(int index)
{
    vertexList.push_back(index);
}

void Journal::addTriangle(int index)
{
    triangleList.push_back(index);
}

void Journal::add(int index)
{
    list.push_back(index);
}

void Journal::submit()
{
    int i;
    for(i = _current + 1; i < size(); i++) at(i).clean();
    erase(begin() + _current + 1, end());
    push_back(currentType);
    _current++;

    switch(currentType)
    {
    case CREATE:
    {
        CreateOrRemove &data = *at(size() - 1).data().createOrRemove;
        for(i = 0; i < vertexList.size(); i++) data.ver().push_back({_model->getVertex()[vertexList[i]], vertexList[i]});
        for(i = 0; i < triangleList.size(); i++) data.tri().push_back({_model->getTriangle()[triangleList[i]], triangleList[i]});
        break;
    }
    case REMOVE:
    {
        CreateOrRemove &data = *at(size() - 1).data().createOrRemove;
        vector <ElementWithIndex <Vertex> > &vertex = data.ver();
        for(i = 0; i < vertexList.size(); i++)
        {
            vertex.push_back({_model->getVertex()[vertexList[i]], vertexList[i]});
            vertex[vertex.size() - 1].value().undoRemove();
        }
        vector <ElementWithIndex <Triangle> > &triangle = data.tri();

        for(i = 0; i < triangleList.size(); i++)
        {
            triangle.push_back({_model->getTriangle()[triangleList[i]], triangleList[i]});
            triangle[triangle.size() - 1].value().undoRemove();
        }
        break;
    }
    case SELECT:
    {
        Select &data = *at(size() - 1).data().select;
        data.setVertices(_workWithElements[0]->isChecked());
        for(i = 0; i < list.size(); i++)
        {
            int ind = list[i];
            Element &element = data.vertices() ? (Element&)_model->getVertex()[ind] : (Element&)_model->getTriangle()[ind];
            data.push_back({element.selected(), ind});
        }
        break;
    }
    }
}
