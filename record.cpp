#include "record.h"

#include <iostream>

using namespace std;

Record::Record(Type type)
{
    _type = type;

    switch(type)
    {
    case CREATE:
    {
        _data.create = new Create;
        break;
    }
    case EDIT:
    {
        _data.edit = new Edit;
        break;
    }
    }
}

void Record::clean()
{
    switch(_type)
    {
    case CREATE:
    {
        delete _data.create;
        break;
    }
    case EDIT:
    {
        delete _data.edit;
        break;
    }
    }
}
