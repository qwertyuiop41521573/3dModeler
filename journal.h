#ifndef JOURNAL_H
#define JOURNAL_H

#include "record.h"
#include "vertex.h"

class Model;

class Journal : public vector <Record>
{
public:
    Journal();

    void setModel(Model *model)
    { _model = model; };

    void newRecord(Type type);
    void add(int index);
    void submit();
    void cancelRecord();

    const Record &current()
    { return at(_current); };

    const Record &next()
    { return at(_current + 1); };

    void undo()
    { if(_current != -1) _current--; };

    void redo()
    { if(_current != size() - 1) _current++; };

    bool isEmpty()
    { return _current == -1; };

    bool isFull()
    { return _current == size() - 1; };

    bool currentRecordIsEmpty()
    { return vertexList.size() == 0; };

signals:

public slots:

private:
    int _current = -1;
    Model *_model;

    vector <int> vertexList;
    Type currentType;
};

#endif // JOURNAL_H
