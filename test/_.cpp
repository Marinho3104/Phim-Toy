
#include "./../compiler/compiler.h"
#include "./../utils/linkedList.h"
#include "./../utils/common.h"

#include <iostream>
#include <string.h>

// int {};

namespace ola {

}

int main() {

    int _;

    char* _code = utils::getFileContent((char*) "./test_code.ph");

    compiler::Compiler* _compiler = new compiler::Compiler(_code);

    _compiler->generate();

    delete _compiler;

    free(_code);

    return 0;

}