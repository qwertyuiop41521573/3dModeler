#include "journal.h"
#include "model.h"
#include "toolset.h"
#include "tool.h"
#include "j_signalhandler.h"

#include <QObject>

using namespace Model;
using namespace ToolSet;

namespace Journal
{
    vector <Record> _vec;

    int _current = -1;
    QRadioButton **_workWithElements;

    vector <int> vertexList, triangleList;
    Type currentType;

    void push();
    void undo();
    void redo();
        void updateNormals(const Edit &data);
    const Record &next();
    bool isEmpty();
    bool isFull();


    SignalHandler *signalHandler;
}

void Journal::push()
{
    //if we did some undos and from that point do something, we should clean the journal records starting from current point
    for(int i = _current + 1; i < _vec.size(); i++) _vec.at(i).clean();
    _vec.erase(_vec.begin() + _current + 1, _vec.end());
    //and only then push
    _vec.push_back(currentType);
    _current++;
}

void Journal::cleanAll()
{
    for(int i = 0; i < _vec.size(); i++) _vec.at(i).clean();
    _vec.clear();
    _current = -1;
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
}

void Journal::addBefore(bool isVertex, int index)
{
    //before tool was used
    Edit &data = *current()->data().edit;
    if(isVertex) data.vertex().push_back({index, vertex()[index]});
    else data.triangle().push_back({index, triangle()[index]});
}

void Journal::addAfter(bool isVertex)
{
    //after
    Edit &data = *current()->data().edit;
    if(isVertex)
    {
        vector <TwoElementsWithIndex <Vertex> > &vertex = data.vertex();
        vertex[vertex.size() - 1].setAfter(Model::vertex()[vertex[vertex.size() - 1].index()]);
    }
    else
    {
        vector <TwoElementsWithIndex <Triangle> > &triangle = data.triangle();
        triangle[triangle.size() - 1].setAfter(Model::triangle()[triangle[triangle.size() - 1].index()]);
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
        Create &data = *current()->data().create;
        for(i = 0; i < vertexList.size(); i++) data.vertex().push_back({vertex()[vertexList[i]], vertexList[i]});
        for(i = 0; i < triangleList.size(); i++) data.triangle().push_back({triangle()[triangleList[i]], triangleList[i]});
        break;
    }
    case EDIT:
    {
        Edit &data = *current()->data().edit;
        if(data.vertex().size() || data.triangle().size()) break;

        //if nothing was selected before using tool
        current()->clean();
        _vec.erase(_vec.end());
        _current--;
        break;
    }
    }
}

void Journal::addVertex(int index)
{ vertexList.push_back(index); }

void Journal::addTriangle(int index)
{ triangleList.push_back(index); }

Record *Journal::current() { return &_vec.at(_current); }
const Record &Journal::next() { return _vec.at(_current + 1); }
void Journal::undo() { if(_current != -1) _current--; }
void Journal::redo() { if(_current != _vec.size() - 1) _current++; }
bool Journal::isEmpty() { return _current == -1; }
bool Journal::isFull() { return _current == _vec.size() - 1; }

void Journal::SignalHandler::undo()
{
    if(activeTool()->busy() || isEmpty()) return;

    const Record &rec = *current();

    switch(rec.type())
    {
    case CREATE:
    {
        const Create &data = *rec.data().create;
        const vector <ElementWithIndex <Vertex> > &vertex = data.vertex();
        //remove what was created
        for(int i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()].remove();
        const vector <ElementWithIndex <Triangle> > &triangle = data.triangle();
        for(int i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()].remove();
        break;
    }
    case EDIT:
    {
        const Edit &data = *rec.data().edit;
        //replace edited elements with their "before" value
        const vector <TwoElementsWithIndex <Vertex> > &vertex = data.vertex();
        for(int i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].before();
        const vector <TwoElementsWithIndex <Triangle> > &triangle = data.triangle();
        for(int i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].before();

        updateNormals(data);

        break;
    }
    }

    Journal::undo();
}

void Journal::SignalHandler::redo()
{
    if(activeTool()->busy() || isFull()) return;
    const Record &rec = next();

    int i;

    switch(rec.type())
    {
    case CREATE:
    {
        const Create &data = *rec.data().create;
        //recreate elements
        const vector <ElementWithIndex <Vertex> > &vertex = data.vertex();
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].value();
        const vector <ElementWithIndex <Triangle> > &triangle = data.triangle();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].value();
        break;
    }
    case EDIT:
    {
        const Edit &data = *rec.data().edit;
        //replace edited elements with their "after" value
        const vector <TwoElementsWithIndex <Vertex> > &vertex = data.vertex();
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].after();
        const vector <TwoElementsWithIndex <Triangle> > &triangle = data.triangle();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].after();

        updateNormals(data);

        break;
    }
    }

    Journal::redo();
}

void Journal::updateNormals(const Edit &data)
{
    const vector <TwoElementsWithIndex <Vertex> > &vertex = data.vertex();
    const vector <TwoElementsWithIndex <Triangle> > &triangle = data.triangle();

    vector<int> tri;
    for(int i = 0; i < triangle.size(); i++) tri.push_back(triangle[i].index());
    for(int i = 0; i < vertex.size(); i++) {
        for(int j = 0; j < Model::triangle().size(); j++) {
            for(int k = 0; k < 3; k++) {
                if(Model::triangle()[j].getIndex(k) == vertex[i].index()) {
                    int l;
                    for(l = 0; l < tri.size(); l++) if(tri[l] == j) break;
                    if(l == tri.size()) tri.push_back(j);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < tri.size(); i++) Model::triangle()[tri[i]].countNormal();
}

void Journal::connectActions(QAction *undoAction, QAction *redoAction)
{ signalHandler = new SignalHandler(undoAction, redoAction); }
