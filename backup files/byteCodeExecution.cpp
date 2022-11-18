#include "./byteCodeExecution.h"

#include "./../objects/phim_string.h"
#include "./../objects/phim_int.h"

#include "./../parser/byteCodeConverter.h"
#include "./../byteCode/definitions.h"
#include "./../byteCode/byteCode.h"
#include "./../utils/linkedList.h"
#include "./../parser/token.h"
#include "./../parser/ast.h"
#include "./block.h"
#include "./data.h"

#include <iostream>

void vm::executeByteCode(byte_code::Byte_Code* _bc, Vm_Block* _vb) {

    switch (_bc->code)
    {
    case BYTECODE_MEM_STACK_LOAD_CONST:
        vm::MEM_STACK_LOAD_CONST(_bc->argument, _vb);
        break;
    case BYTECODE_ADDITION:
        vm::ADDITION(_bc->argument, _vb);
        break;
    case BYTECODE_MULTIPLICATION:
        vm::MULTIPLICATION(_bc->argument, _vb);
        break;
    case BYTECODE_ASSIGN:
        vm::ASSIGN(_bc->argument, _vb);
        break;
    case BYTECODE_LOAD_NAME:
        vm::LOAD_NAME(_bc->argument, _vb);
        break;
    case BYTECODE_MEM_STACK_LOAD:
        vm::MEM_STACK_LOAD(_bc->argument, _vb);
        break;
    default:
        break;
    }

}

void vm::LOAD_NAME(int _arg, Vm_Block* _block) {
    
    objects::Phim_String* _ = (objects::Phim_String*) malloc(sizeof(objects::Phim_String));

    new (_) objects::Phim_String((*_block->storage->names)[_arg]);

    _block->stack->addFrst(
        _
    );

}

void vm::MEM_STACK_LOAD(int _arg, Vm_Block* _block) {

    parser::TypeInformation* _type = (*_block->storage->types)[_arg];
    objects::Phim_Object* _name = _block->stack->removeFrst()->object; // Remove with free TODO


    if (_type->userDefinedType) {
        exit(1); // TODO
    }

    else {

        switch ((*_block->storage->keyWordsId)[_type->typePos])
        {
        case TOKEN_TYPE_INT:
            
            {

                objects::Phim_Int* _ = (objects::Phim_Int*) malloc(sizeof(objects::Phim_Int));  
                new (_) objects::Phim_Int();
                _->clean();

                _block->data->addNew(_name, _);

                _block->stack->addFrst(_); 

                std::cout << "Added to stack and data" << std::endl;         

                break;
            
            }

        default:
            break;
        }

    }

}

void vm::MEM_STACK_LOAD_CONST(int _arg, Vm_Block* _block) {

    objects::Phim_Object* _;
    char* _value = (*_block->storage->values)[_arg];

    switch (parser::getTokenIdIndetifier(_value))
    {
    case TOKEN_NUMBERINT:
        
        _ = objects::PhimIntFromCString(_value);

        break;
    
    default:
        break;
    }

    _block->stack->addFrst(_);

    std::cout << "Load const added to stack" << std::endl;

}

void vm::MULTIPLICATION(int _arg, Vm_Block* _block) {

    objects::Phim_Object* _frst = _block->stack->removeFrst()->object, *_scnd = _block->stack->removeFrst()->object; // TODO REmove with free

    _block->stack->addFrst(
        *_frst * _scnd
    );

    std::cout << "Multiplication done added to stack result" << std::endl;

}

void vm::ADDITION(int _arg, Vm_Block* _block) {

    objects::Phim_Object* _frst = _block->stack->removeFrst()->object, *_scnd = _block->stack->removeFrst()->object; // TODO REmove with free

    _block->stack->addFrst(
        *_frst + _scnd
    );

    std::cout << "Addition done added to stack result" << std::endl;

}

void vm::ASSIGN(int _arg, Vm_Block* _block) {

    std::cout << "Assign" << std::endl;

    objects::Phim_Object* _frst = _block->stack->removeFrst()->object; // TODO Remove with free

    _frst->bitsRepresentation();

}

