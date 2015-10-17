#ifndef JOURNAL_H
#define JOURNAL_H

#include "record.h"
#include "vertex.h"

class Journal : public vector <Record>
{
public:
    Journal();

    void newRecord(Type type);

    void add(Vertex vertex, int index)
    { at(size() - 1).getVertices().push_back({vertex, index}); };

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

signals:

public slots:

private:
    int _current = -1;
};

#endif // JOURNAL_H
