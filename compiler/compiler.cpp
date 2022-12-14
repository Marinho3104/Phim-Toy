#include "./compiler.h"

#include "./../parser/convertor.h"
#include "./../parser/tokenizer.h"
#include "./../utils/common.h"
#include "./../parser/ast.h"

#include "./../byte_code/byte_code.h"

#include <iostream>

compiler::Compiler_Control::~Compiler_Control() {}

compiler::Compiler_Control::Compiler_Control(char* __code) : code(__code) {}

void compiler::Compiler_Control::generate() {

    parser::Tokenizer_Control* _tokenizer_control = new parser::Tokenizer_Control(code, 1);

    _tokenizer_control->generate();

    parser::Ast_Control* _ast_control = new parser::Ast_Control(_tokenizer_control, 1);

    _ast_control->generate();

    parser::Convertor* _convertor = new parser::Convertor(_ast_control, NULL, 1); _convertor->generate();

    byte_code::Compiled_Byte_Code* _compiled = _convertor->getCompiledByteCode();

    std::cout << std::endl;

    _compiled->print();
    
    delete _convertor;

    delete _compiled;

}


compiler::Compiler::~Compiler() {}

compiler::Compiler::Compiler(char* _code) : code(_code) { generateBuiltIn(); }

void compiler::Compiler::generateBuiltIn() {

    char* _built_in_content = utils::getFileContent("./../built_in/byte.ph");

    compiler::Compiler_Control* built_in = new Compiler_Control(_built_in_content); built_in->generate();

    delete built_in;
    
    free(_built_in_content);

}
