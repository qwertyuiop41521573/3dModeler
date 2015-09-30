#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit MyCheckBox()
    { connect( this, SIGNAL( clicked( bool ) ), this, SLOT(
                   handleClick( bool ) ) ); }

signals:
    void handledClick( MyCheckBox *myCheckBox );

public slots:
    void handleClick( bool value )
    { if( value ) emit handledClick( this ); };

};

#endif // MYCHECKBOX_H
