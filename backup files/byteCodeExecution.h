#pragma once

#ifndef VM_BYTE_CODE_EXECUTION_H
#define VM_BYTE_CODE_EXECUTION_H

namespace byte_code { struct Byte_Code; }

namespace vm {

    // Forward
    struct Vm_Block;

    void executeByteCode(byte_code::Byte_Code*, Vm_Block*);

    void LOAD_NAME(int, Vm_Block*);

    void MEM_STACK_LOAD(int, Vm_Block*);

    void MEM_STACK_LOAD_CONST(int, Vm_Block*);

    void MULTIPLICATION(int, Vm_Block*);

    void ADDITION(int, Vm_Block*);

    void ASSIGN(int, Vm_Block*);

}

#endif
