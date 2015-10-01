#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit MyCheckBox()
    { connect(this, SIGNAL(clicked(bool)), this, SLOT(handleClick(
                                                       bool))); };

    void setNeighbor(MyCheckBox *neighbor)
    { _neighbor = neighbor; };

private:
    MyCheckBox *_neighbor;

private slots:
    void handleClick(bool value)
    { if(value) _neighbor->setChecked(false); };

};

#endif // MYCHECKBOX_H
