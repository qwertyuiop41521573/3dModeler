#include "record.h"

Record::Record(Type type)
{
    _type = type;

    if(type == CREATE || type == REMOVE) _data.createOrRemove = new vector <VertexAndIndex>;
    if(type == SELECT) _data.select = new vector <SelValueAndIndex>;

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

