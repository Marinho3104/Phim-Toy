#include "./execution.h"

#include "./../objects/phim_int.h"

#include "./../byteCode/definitions.h" // Byte code definitions
#include "./../byteCode/byteCode.h"
#include "./stack.h"
#include "./../parser/token.h"
#include "./program.h"
#include "./../utils/linkedList.h"
#include "./../parser/compiled_byte_code.h"
#include "./../parser/parser_helper.h"
#include "./vm_stdlib.h"
#include "./vm_definitions.h"

#include <iostream>

void vm::executeByteCode(byte_code::Byte_Code* _byteCode, Stack* _stack) {

    // std::cout << "Execute zone " << std::endl;
    // std::cout << (int) _byteCode->code << " " << _byteCode->argument << std::endl;

    switch (_byteCode->code)
    {
    case BYTECODE_MEM_STACK_LOAD_IMPLICIT: 
        MEM_STACK_LOAD_IMPLICIT(_byteCode->argument, _stack);
        break;
    
    default:
        break;
    }

}

void vm::MEM_STACK_LOAD_IMPLICIT(int _arg, Stack* _stack) {

    char* _value = (*_stack->program->compiled_byte_code->implicit_values)[_arg];

    int _typeId = parser::getTokenIdIdetifier(
        _value
    );

    int _size = parser_helper::getSizeImplicitValue(
        _typeId
    );

    int* _rslt;
    
    objects::Phim_Int::fromCharToInt(_value, _rslt);

    address_t _addr = vm_stdlib::allocMemStack(
        _rslt, _size, _stack->program->mem
    );

    

}


