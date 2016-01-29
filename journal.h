#ifndef JOURNAL_H
#define JOURNAL_H

#include "vertex.h"
#include "types.h"

#include <QRadioButton>

//record types
//CREATE - new elements
//EDIT   - changing elements (transform, remove, select / deselect)
typedef enum { CREATE, EDIT, DELETE } Type;

class Record;

namespace Journal
{
    void newRecord(Type type);

    void addVertex(int index);
    void addTriangle(tr_it it);

    void addBefore(bool isVertex, int index);
    void addBefore(tr_it it);
    void addAfter(bool isVertex);

    void transform(const QMatrix4x4 &matrix);
    void submit();

    Record *current();
    void cleanAll();    

    void connectActions(QAction *undoAction, QAction *redoAction);
}

#endif // JOURNAL_H
