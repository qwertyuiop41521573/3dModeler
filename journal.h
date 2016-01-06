#ifndef JOURNAL_H
#define JOURNAL_H

#include "record.h"
#include "vertex.h"
#include <QRadioButton>

//journal is extended "vector <Record>"
class Journal : public vector <Record>
{
public:
    Journal();

    void setVariables(QRadioButton **workWithElements);

    void newRecord(Type type);

    void addVertex(int index);
    void addTriangle(int index);

    void addBefore(bool isVertex, int index);
    void addAfter(bool isVertex);

    void transform(const QMatrix4x4 &matrix);
    void submit();

    const Record &currentRO()
    { return at(_current); };

    Record &current()
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

    void cleanAll();

signals:

public slots:

private:
    int _current = -1;
    QRadioButton **_workWithElements;

    vector <int> vertexList, triangleList;
    Type currentType;

    void push();
};

#endif // JOURNAL_H
