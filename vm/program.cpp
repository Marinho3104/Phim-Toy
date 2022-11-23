#include "./program.h"

#include "./../parser/compiled_byte_code.h" 
#include "./../byteCode/byteCode.h" // Byte Code
#include "./../utils/linkedList.h" // Linked List
#include "./memory.h" // Memory 
#include "./stack.h" // Stack

#include <iostream>


vm::Program::Program(parser::Compiled_Byte_Code* __c) : compiled_byte_code(__c) {

    mem = (Memory*) malloc(sizeof(Memory));
    new (mem) Memory();

}

void vm::Program::run() {

    // Run the global scope first
    executeNewStack(
        (*compiled_byte_code->byte_code_blocks)[compiled_byte_code->byte_code_blocks->count - 1]->byte_code_block
    );

}

void vm::Program::executeNewStack(utils::LinkedList <byte_code::Byte_Code*>* _stack_code) {
    
    Stack* _stck = (Stack*) malloc(sizeof(Stack));

    new (_stck) Stack(
        _stack_code, this
    );

    _stck->run();

}
