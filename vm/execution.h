#pragma once

#ifndef VM_EXECUTION_H
#define VM_EXECUTION_H

/* Execute the byte code
*   
*/

namespace byte_code { struct Byte_Code; }

namespace vm {

    // Forward
    struct Stack;

    void executeByteCode(
        byte_code::Byte_Code*,
        Stack*
    );

    void MEM_STACK_LOAD_IMPLICIT(int, Stack*);

}

#endif