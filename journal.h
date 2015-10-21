#ifndef JOURNAL_H
#define JOURNAL_H

#include "record.h"
#include "vertex.h"
#include <QRadioButton>

class Model;

class Journal : public vector <Record>
{
public:
    Journal();

    void setVariables(Model *model, QRadioButton **workWithElements);

    void newRecord(Type type);
    void addVertex(int index);
    void addTriangle(int index);
    void add(int index);
    void addList(const vector <int> &list);
    void transform(const QMatrix4x4 &matrix);
    void submit();

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
    { return list.size() == 0; };

signals:

public slots:

private:
    int _current = -1;
    Model *_model;
    QRadioButton **_workWithElements;

    vector <int> vertexList, triangleList, list;
    Type currentType;

    void push();
};

#endif // JOURNAL_H
