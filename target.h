#ifndef TARGET_H
#define TARGET_H

class QWidget;

namespace Target
{
    void createWorkWithWidget(QWidget *workWithWidget);

    bool isVertex();
    bool isTriangle();

    enum Tgt {VERTEX, TRIANGLE};
    void set(Tgt target);
}

#endif // TARGET_H
