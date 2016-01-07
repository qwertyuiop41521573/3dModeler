#ifndef J_SIGNALHANDLER_H
#define J_SIGNALHANDLER_H

#include <QObject>

class QAction;

namespace Journal
{
    class SignalHandler : public QObject
    {
        Q_OBJECT
    public:
        SignalHandler(QAction *undoAction, QAction *redoAction);

    signals:

    public slots:
        void undo();
        void redo();
    };
}

#endif // J_SIGNALHANDLER_H
