#include "journal.h"
#include "model.h"

Journal::Journal()
{

}

void Journal::push()
{
    //if we did some undos and from that point do something, we should clean the journal records starting from current point
    for(int i = _current + 1; i < size(); i++) at(i).clean();
    erase(begin() + _current + 1, end());
    //and only then push
    push_back(currentType);
    _current++;
}

void Journal::cleanAll()
{
    for(int i = 0; i < size(); i++) at(i).clean();
    clear();
    _current = -1;
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
        //in CREATE we first record indices of new elements
        vertexList.clear();
        triangleList.clear();
    }
    if(type == EDIT) push();
};

void Journal::addBefore(bool isVertex, int index)
{
    //before tool was used
    Edit &data = *current().data().edit;
    if(isVertex) data.vertex().push_back({index, _model->vertex()[index]});
    else data.triangle().push_back({index, _model->triangle()[index]});
}

void Journal::addAfter(bool isVertex)
{
    //after
    Edit &data = *current().data().edit;
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

void Journal::submit()
{
    int i;
    switch(currentType)
    {
    case CREATE:
    {
        //values of new elements are recorded after submit() is called (because vertices are being moved during creation)
        push();        
        Create &data = *current().data().create;
        for(i = 0; i < vertexList.size(); i++) data.ver().push_back({_model->vertex()[vertexList[i]], vertexList[i]});
        for(i = 0; i < triangleList.size(); i++) data.tri().push_back({_model->triangle()[triangleList[i]], triangleList[i]});
        break;
    }
    case EDIT:
    {
        Edit &data = *current().data().edit;
        if(data.verRO().size() || data.triRO().size()) break;

        //if nothing was selected before using tool
        current().clean();
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

