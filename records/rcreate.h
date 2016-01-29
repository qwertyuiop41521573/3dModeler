#ifndef RCREATE_H
#define RCREATE_H

#include "recordwith1element.h"

class RCreate : public RecordWith1Element
{
public:
    void submit();
    void undo();
    void redo();
};

#endif // RCREATE_H
