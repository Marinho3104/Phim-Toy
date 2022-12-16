#include "./compiler.h"

#include "./../byte_code/byte_code.h"
#include "./../parser/tokenizer.h"
#include "./../parser/convertor.h"
#include "./../utils/common.h"
#include "./../parser/ast.h"
#include "./built_in.h"

#include <iostream>


namespace parser {

    Name_Space_Control* name_space_control = 0, *built_in_name_space_control = 0;
    Ast_Control* ast_control = 0, *built_in_ast_control = 0;
    Tokenizer_Control* tokenizer_control = 0;
    Convertor* convertor = 0;

}


compiler::Compiler::~Compiler() { delete built_in; }

compiler::Compiler::Compiler(char* __code) : code(__code) { generateBuiltIn(); }

void compiler::Compiler::generate() {

    std::cout << "\n\n ------------------------- Generating -------------------------\n" << std::endl;

    parser::setUpTokenizer(code, 0);

    parser::tokenizer_control->generate();

    parser::setUpAst(0);

    parser::ast_control->generate();

    parser::setUpConvertor(0);

    parser::convertor->generate();

    byte_code::Compiled_Byte_Code* _compiler_byte_code = parser::convertor->getCompiledByteCode();

    _compiler_byte_code->print();

    delete _compiler_byte_code;

    parser::cleanTokenizer();

    parser::cleanAst();

    parser::cleanConvertor();

}

void compiler::Compiler::generateBuiltIn() { built_in = new Built_In(); }



