#ifndef RDELETE_H
#define RDELETE_H

#include "recordwith1element.h"

class RDelete : public RecordWith1Element
{
public:
    void submit();
    void undo();
    void redo();
};

#endif // RDELETE_H
