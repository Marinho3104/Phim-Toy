
#include "./../compiler/compiler.h"
#include "./../utils/linkedList.h"

#include <iostream>
#include <string.h>

int main() {

    compiler::Compiler* _compiler = new compiler::Compiler("byte o;");

    delete _compiler;

    return 0;

}