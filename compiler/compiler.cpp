#include "./compiler.h"

#include "./built_in.h"

#include <iostream>


compiler::Compiler::~Compiler() { delete built_in; }

compiler::Compiler::Compiler(char* _code) : code(_code) {
    built_in = new Built_In();
}
