#include "./stack.h"

#include "./memory.h"
#include "./program.h"
#include "./../utils/linkedList.h"
#include "./../byteCode/byteCode.h"
#include "./execution.h"

#include <iostream>


vm::Stack::Stack(utils::LinkedList <byte_code::Byte_Code*>* _byte_code, Program* _program) : byte_code(_byte_code), program(_program) {
    start_address = _program->mem->topStackMem;
}

void vm::Stack::run() {

    for (int _ = 0; _ < byte_code->count; _++)

        vm::executeByteCode(
            (*byte_code)[_], this
        );


}


