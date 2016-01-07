#include "w_signalhandler.h"
#include "workspace.h"
#include "glwidget.h"

#include <QPushButton>
#include <QCheckBox>

#include "gui/mypushbutton.h"
#include "gui/mycombobox.h"

Workspace::SignalHandler::SignalHandler(QPushButton *maximizeButton, MyComboBox *renderingMode, QCheckBox *wireframeOverlay, MyComboBox *projection) : QObject(0)
{
    connect(maximizeButton, SIGNAL(clicked(bool)), this, SLOT(maximize(bool)));
    connect(renderingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderingMode(int)));
    connect(wireframeOverlay, SIGNAL(clicked(bool)), this, SLOT(changeWireFrameOverlay()));
    connect(projection, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjection(int)));
}
