#pragma once

#ifndef VM_H
#define VM_H

namespace parser { struct Compiled_Byte_Code; }

namespace vm {

    // Forward
    struct Program;

    /* Virtual machine 
    *   
    */
    struct Vm {

        Program* program;

        Vm() = default;

        void execute(parser::Compiled_Byte_Code*);

    };

}

#endif