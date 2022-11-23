#include "./vm.h"

#include "./program.h"

#include <iostream>

void vm::Vm::execute(parser::Compiled_Byte_Code* _com) {

    program = (Program*) malloc(sizeof(Program));

    new (program) Program(
        _com
    );

    program->run();

}
