#include "./frame.h"

#include "./block.h"
#include "./data.h"

#include <iostream>

vm::Vm_Frame::Vm_Frame(utils::LinkedList <byte_code::Byte_Code*>* _bc, parser::Storage* _ps, Vm_Data* _vd) : data(new Vm_Data(_vd)) {
    block = new vm::Vm_Block(_bc, _ps, data);
}

void vm::Vm_Frame::execute() { block->execute(); }

