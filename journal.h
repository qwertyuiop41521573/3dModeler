#ifndef JOURNAL_H
#define JOURNAL_H

#include "record.h"
#include "vertex.h"
#include "types.h"

#include <QRadioButton>

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
