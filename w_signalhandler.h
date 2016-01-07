#ifndef W_SIGNALHANDLER_H
#define W_SIGNALHANDLER_H

#include <QObject>

class QPushButton;
class MyPushButton;
class MyComboBox;
class QCheckBox;

namespace Workspace {
    class SignalHandler : public QObject
    {
        Q_OBJECT
    public:
        SignalHandler(QPushButton *maximizeButton, MyComboBox *renderingMode, QCheckBox *wireframeOverlay, MyComboBox *projection);

    signals:

    public slots:
        void maximize(bool value);
        void changeRenderingMode(int mode);
        void changeWireFrameOverlay();
        void changeProjection(int newProjection);
    };
}

#endif // W_SIGNALHANDLER_H
