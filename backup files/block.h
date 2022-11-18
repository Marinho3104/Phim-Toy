#pragma once

#ifndef VM_BLOCK_H
#define VM_BLOCK_H

namespace utils { template <typename> struct LinkedList; }
namespace objects { struct Phim_Object; }
namespace byte_code { struct Byte_Code; }
namespace parser { struct Storage; }

namespace vm {

    // Forward
    struct Vm_Data;

    struct Vm_Block {

        utils::LinkedList <byte_code::Byte_Code*>* byteCode;
        utils::LinkedList <objects::Phim_Object*>* stack;
        parser::Storage* storage;
        Vm_Data* data;

        Vm_Block(utils::LinkedList <byte_code::Byte_Code*>*, parser::Storage*, Vm_Data*);

        void execute();

    };

}

#endif