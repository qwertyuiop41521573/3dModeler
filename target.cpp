#include "target.h"

#include <QRadioButton>
#include <QGridLayout>
#include <QWidget>

namespace Target
{
    QRadioButton *workWithElements[2];
}

void Target::createWorkWithWidget(QWidget *workWithWidget)
{
    QGridLayout *workWithLayout = new QGridLayout;
    for(int i = 0; i < 2; i++) workWithElements[i] = new QRadioButton;
    workWithElements[0]->setText("Vertex");
    workWithElements[1]->setText("Triangle");
    for(int i = 0; i < 2; i++) workWithLayout->addWidget(workWithElements[i], i, 0);
    workWithElements[0]->setChecked(true);
    workWithWidget->setLayout(workWithLayout);
}

bool Target::isVertex()   { return workWithElements[0]->isChecked(); }
bool Target::isTriangle() { return workWithElements[1]->isChecked(); }

void Target::set(Tgt target)
{
    if(target == VERTEX)   workWithElements[0]->setChecked(true);
    if(target == TRIANGLE) workWithElements[1]->setChecked(true);
}
