#include "journal.h"

Journal::Journal()
{

}

void Journal::newRecord(Type type)
 {
    push_back(type);
    _current++;
};
