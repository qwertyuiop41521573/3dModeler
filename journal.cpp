#include "journal.h"
#include "model.h"
#include "toolset.h"
#include "tool.h"
#include "j_signalhandler.h"
#include "trianglecontainer.h"

#include "records/rcreate.h"
#include "records/redit.h"
#include "records/rdelete.h"

#include <QObject>

using namespace Model;
using namespace ToolSet;

namespace Journal
{
    vector <Record*> _vec;

    int _current = -1;
    QRadioButton **_workWithElements;

    void undo();
    void redo();

    Record *next();
    bool isEmpty();
    bool isFull();


    SignalHandler *signalHandler;

   // Record *currentRecord;
}

void Journal::cleanAll()
{
    for(int i = 0; i < _vec.size(); i++) delete _vec.at(i);
    _vec.clear();
    _current = -1;
}

void Journal::newRecord(Type type)
{
    //if we did some undos and from that point do something, we should clean the journal records starting from current point
    for(int i = _current + 1; i < _vec.size(); i++) delete _vec.at(i);
    _vec.erase(_vec.begin() + _current + 1, _vec.end());
    //and only then push
    Record *record;
    if(type == CREATE) record = new RCreate;
    if(type == EDIT)   record = new REdit;
    if(type == DELETE) record = new RDelete;
    _vec.push_back(record);
    _current++;
}

void Journal::addBefore(bool isVertex, int index)
{
    //before tool was used
    //if(isVertex)
    static_cast<REdit*>(current())->vertex().push_back({index, vertex()[index]});
    //else data.triangle().push_back({index, triangle()[index]});
}

void Journal::addBefore(tr_it it)
{
    //before tool was used
    //Edit &data = *current()->data().edit;
    //if(isVertex) data.vertex().push_back({index, vertex()[index]});
    //else
    static_cast<REdit*>(current())->triangle().push_back(it);
}

void Journal::addAfter(bool isVertex)
{
    //after
    REdit &data = *static_cast<REdit*>(current());
    if(isVertex)
    {
        vector <TwoVerticesWithIndex> &vertex = data.vertex();
        vertex[vertex.size() - 1].after = Model::vertex()[vertex[vertex.size() - 1].index];
    }
    else data.triangle().back().after = *data.triangle().back().iterator;
}

void Journal::submit() { _vec[_vec.size() - 1]->submit(); }

void Journal::addVertex(int index)
{ static_cast<RecordWith1Element*>(_vec.back())->vertex().push_back(index); }
void Journal::addTriangle(tr_it it)
{ static_cast<RecordWith1Element*>(_vec.back())->triangle().push_back(it); }

Record *Journal::current() { return _vec.at(_current); }
Record *Journal::next() { return _vec.at(_current + 1); }

void Journal::undo() { if(_current != -1) _current--; }
void Journal::redo() { if(_current != _vec.size() - 1) _current++; }

bool Journal::isEmpty() { return _current == -1; }
bool Journal::isFull() { return _current == _vec.size() - 1; }

void Journal::SignalHandler::undo()
{
    if(activeTool()->busy() || isEmpty()) return;
    current()->undo();
    Journal::undo();
}

void Journal::SignalHandler::redo()
{
    if(activeTool()->busy() || isFull()) return;
    next()->redo();
    Journal::redo();
}

void Journal::connectActions(QAction *undoAction, QAction *redoAction)
{ signalHandler = new SignalHandler(undoAction, redoAction); }
