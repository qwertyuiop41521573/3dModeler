#ifndef T_SIGNALHANDLER_H
#define T_SIGNALHANDLER_H

#include <QObject>

#include <vector>

using namespace std;

class QAction;

namespace ToolSet
{
    class SignalHandler : public QObject
    {
        Q_OBJECT
    public:
        SignalHandler(QAction *selectAllAction, QAction *selectNoneAction, QAction *snapTogetherAction, QAction *weldTogetherAction, QAction *deleteAction);

    signals:

    public slots:
        void selectAll();
        void selectNone();
        void snapTogether();
        void weldTogether();
        void deleteSlot();

    private:
        void addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index);
    };
}

#endif // T_SIGNALHANDLER_H
