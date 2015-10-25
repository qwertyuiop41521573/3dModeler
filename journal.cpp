#include "journal.h"
#include "model.h"

Journal::Journal()
{

}

void Journal::push()
{
    for(int i = _current + 1; i < size(); i++) at(i).clean();
    erase(begin() + _current + 1, end());
    push_back(currentType);
    _current++;
}

void Journal::setVariables(Model *model, QRadioButton **workWithElements)
{
    _model = model;
    _workWithElements = workWithElements;
}

void Journal::newRecord(Type type)
{
    currentType = type;
    if(type == CREATE)
    {
        vertexList.clear();
        triangleList.clear();
    }
    if(type == EDIT) push();
};

void Journal::addBefore(bool isVertex, int index)
{
    Edit &data = *at(size() - 1).data().edit;
    if(isVertex) data.vertex().push_back({index, _model->vertex()[index]});
    else data.triangle().push_back({index, _model->triangle()[index]});
}

void Journal::addAfter(bool isVertex)
{
    Edit &data = *at(size() - 1).data().edit;
    if(isVertex)
    {
        vector <TwoElementsWithIndex <Vertex> > &vertex = data.vertex();
        vertex[vertex.size() - 1].setAfter(_model->vertex()[vertex[vertex.size() - 1].index()]);
    }
    else
    {
        vector <TwoElementsWithIndex <Triangle> > &triangle = data.triangle();
        triangle[triangle.size() - 1].setAfter(_model->triangle()[triangle[triangle.size() - 1].index()]);
    }
}

/*
void Journal::addTriangleBefore(int index)
{
    at(size() - 1).data().edit->triangle().push_back({index, _model->triangle()[index]});
}

void Journal::addTriangleAfter()
{
    vector <TwoElementsWithIndex <Triangle> > &triangle = at(size() - 1).data().edit->triangle();
    triangle[triangle.size() - 1].setAfter(_model->triangle()[triangle[triangle.size() - 1].index()]);
}*/

void Journal::submit()
{
    int i;
    switch(currentType)
    {
    case CREATE:
    {
        push();
        Create &data = *at(size() - 1).data().create;
        for(i = 0; i < vertexList.size(); i++) data.ver().push_back({_model->vertex()[vertexList[i]], vertexList[i]});
        for(i = 0; i < triangleList.size(); i++) data.tri().push_back({_model->triangle()[triangleList[i]], triangleList[i]});
        break;
    }
    case EDIT:
    {
        Edit &data = *at(size() - 1).data().edit;
        if(data.verRO().size() || data.triRO().size()) break;

        at(size() - 1).clean();
        erase(end());
        _current--;
        break;
    }
    }
}

void Journal::addVertex(int index)
{
    vertexList.push_back(index);
}

void Journal::addTriangle(int index)
{
    triangleList.push_back(index);
}

