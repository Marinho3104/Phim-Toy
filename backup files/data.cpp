#include "./data.h"

#include "./../objects/phim_object.h"
#include "./../utils/linkedList.h"

#include <iostream>


vm::Vm_Data::Vm_Data(Vm_Data* _previous) : previous(_previous) {
    data = new utils::LinkedList <objects::Phim_Object*>();
    names = new utils::LinkedList <objects::Phim_Object*>();
}

void vm::Vm_Data::addNew(objects::Phim_Object* _name, objects::Phim_Object* _value) {

    names->add(_name);

    data->add(_value);

}

objects::Phim_Object* vm::Vm_Data::getValueByName(objects::Phim_Object* _) {

    int _rtr = names->getObjectPosition(_, [](objects::Phim_Object* _f, objects::Phim_Object* _s) { return _f->operator==(_s); });

    if (_rtr != -1) return (*data)[_rtr];

    if (previous) return getValueByName(_);
    
    return NULL;

}


