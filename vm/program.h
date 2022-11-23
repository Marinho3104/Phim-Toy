#pragma once

#ifndef VM_PROGRAM_H
#define VM_PROGRAM_H

namespace utils { template <typename> struct LinkedList; }
namespace parser { struct Compiled_Byte_Code; }
namespace byte_code { struct Byte_Code; }

namespace vm {

    // Forward
    struct Memory; // Memory emmulation

    /* Holds all important information about a new program execution
    *   
    *   Memory
    *   Bytecode
    * 
    */
    struct Program {

        parser::Compiled_Byte_Code* compiled_byte_code; // Program byte code info 
        Memory* mem; // Stack, Heap, Addresses Table


        /* Constructor */
        Program(parser::Compiled_Byte_Code*);

        /* Start running program */
        void run();

        /* Get new Stack */
        void executeNewStack(utils::LinkedList <byte_code::Byte_Code*>*);

    };

}

#endif