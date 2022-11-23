#pragma once

#ifndef VM_STACK_H
#define VM_STACK_H

#include "./vm_definitions.h" // Vm Definitions

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace vm {

    // Forward
    struct Program; // Program

    /* Holds program stack execution */
    struct Stack {

        utils::LinkedList <byte_code::Byte_Code*>* byte_code;
        address_t start_address;
        Program* program;

        Stack(utils::LinkedList <byte_code::Byte_Code*>*, Program*);

        void run();

    };

}   

#endif