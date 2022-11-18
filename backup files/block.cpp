#include "./block.h"

#include "./../parser/byteCodeConverter.h"
#include "./../utils/linkedList.h"
#include "./byteCodeExecution.h"

#include <iostream>

vm::Vm_Block::Vm_Block(utils::LinkedList <byte_code::Byte_Code*>* _bc, parser::Storage* _ps, Vm_Data* _vd) : byteCode(_bc), storage(_ps), data(_vd) {
    stack = new utils::LinkedList <objects::Phim_Object*>();
}

void vm::Vm_Block::execute() {

    for (int _ = 0; _ < byteCode->count; _++)

        vm::executeByteCode((*byteCode)[_], this);

}

