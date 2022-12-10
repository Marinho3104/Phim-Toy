#include "./compiler.h"

#include "./../parser/tokenizer.h"
#include "./../utils/common.h"
#include "./../parser/ast.h"

#include <iostream>

compiler::Compiler_Control::~Compiler_Control() {}

compiler::Compiler_Control::Compiler_Control(char* __code) : code(__code) {}

void compiler::Compiler_Control::generate() {

    parser::Tokenizer_Control* _tokenizer_control = new parser::Tokenizer_Control(code, 1);

    _tokenizer_control->generate();

    parser::Ast_Control* _ast_control = new parser::Ast_Control(_tokenizer_control, 1);

    _ast_control->generate();
    
    delete _ast_control;

}


compiler::Compiler::~Compiler() {}

compiler::Compiler::Compiler(char* _code) : code(_code) { generateBuiltIn(); }

void compiler::Compiler::generateBuiltIn() {

    char* _built_in_content = utils::getFileContent("./../built_in/byte.ph");

    compiler::Compiler_Control* built_in = new Compiler_Control(_built_in_content); built_in->generate();

    delete built_in;
    
    free(_built_in_content);

}
