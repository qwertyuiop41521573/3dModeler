#ifndef RECORD_H
#define RECORD_H

class Record
{
public:
    virtual void submit() {}
    virtual void undo() = 0;
    virtual void redo() = 0;
};

#endif // RECORD_H
