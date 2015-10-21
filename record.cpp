#include "record.h"

#include <iostream>

using namespace std;

Record::Record(Type type)
{
    _type = type;

    if(type == CREATE || type == REMOVE) _data.createOrRemove = new CreateOrRemove;
    if(type == SELECT) _data.select = new Select;
    if(type == EDIT) _data.edit = new Edit;

  /*  switch(type)
    {
    case CREATE:
    {
        _data.create = new vector <VertexAndIndex>;
        break;
    }
    case REMOVE:
    {
        _data.remove = new vector <int>;
        break;
    }
    case SELECT:
    {
        _data.select = new vector <SelValueAndIndex>;
        break;
    }
    }*/
}

void Record::clean()
{
    if(_type == CREATE || _type == REMOVE) delete _data.createOrRemove;
    if(_type == SELECT) delete _data.select;
    if(_type == EDIT) delete _data.edit;
}
