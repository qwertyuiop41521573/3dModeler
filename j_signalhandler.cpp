#include "j_signalhandler.h"

#include <QAction>

Journal::SignalHandler::SignalHandler(QAction *undoAction, QAction *redoAction) : QObject(0)
{
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
}
