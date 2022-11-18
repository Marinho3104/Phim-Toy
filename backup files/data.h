#pragma once

#ifndef VM_DATA_H
#define VM_DATA_H

namespace utils { template <typename> struct LinkedList; }
namespace objects { struct Phim_Object; }

namespace vm {

    struct Vm_Data {

        utils::LinkedList <objects::Phim_Object*>* data, *names;
        Vm_Data* previous;

        Vm_Data(Vm_Data*);

        void addNew(objects::Phim_Object*, objects::Phim_Object*);

        objects::Phim_Object* getValueByName(objects::Phim_Object*);


    };

}

#endif