#pragma once

#ifndef VM_FRAME_H
#define VM_FRAME_H

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }
namespace parser { struct Storage; }

namespace vm {

    // Forward
    struct Vm_Block;
    struct Vm_Data;

    struct Vm_Frame {

        Vm_Block* block;
        Vm_Data* data;

        Vm_Frame(utils::LinkedList <byte_code::Byte_Code*>*, parser::Storage*, Vm_Data*);

        void execute();

    };

}

#endif