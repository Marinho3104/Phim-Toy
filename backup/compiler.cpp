#include "./compiler.h"

#include "./../utils/commonFunctions.h"
#include "./../parser/compiler_ast.h"
#include "./../parser/tokenizer.h"
#include "./../parser/ast.h"

#include "./../utils/linkedList.h"

#include <iostream>

compiler::Compiler::~Compiler() {
    delete tokenizer_control;
    delete ast_control;
    delete compiler_control;
    delete compiled_output;

    delete tokenizer_control_built_in;
    delete ast_control_built_in;
    delete compiler_control_built_in;
    delete compiler_output_built_in;

}

compiler::Compiler::Compiler(char* __code) : code(__code), compiled_built_in(NULL) { getBuiltIn(); }

void compiler::Compiler::getBuiltIn() {

    built_in_code = utils::getFileContent("./../built_in/byte.ph");

    runTokenizer(1);

    runAst(1);

    runCompiler(1);

    compiled_built_in = compiler_control_built_in->compiled_code_blocks;

    free(built_in_code);

    std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;

}

void compiler::Compiler::generateByteCode() {

    runTokenizer(0);

    runAst(0);

    runCompiler(0);

}

void compiler::Compiler::runTokenizer(bool __built_in) {

    if (!__built_in) {tokenizer_control = new parser::Tokenizer_Control(code); tokenizer_control->generateTokens();}
    else {tokenizer_control_built_in = new parser::Tokenizer_Control(built_in_code); tokenizer_control_built_in->generateTokens();}
    

}

void compiler::Compiler::runAst(bool __built_in) {

    if (!__built_in) {ast_control = new parser::Ast_Control(tokenizer_control->tokens, 1); ast_control->generate();}
    else {ast_control_built_in = new parser::Ast_Control(tokenizer_control_built_in->tokens, 1); ast_control_built_in->generate();}
    

}

void compiler::Compiler::runCompiler(bool __built_in) {

    if (!__built_in) {compiler_control = new parser::Compiler_Control(ast_control->nodes_name_spaces, ast_control->storage->implicit_values, 1, compiled_built_in); compiler_control->generate();}
    else {compiler_control_built_in = new parser::Compiler_Control(ast_control_built_in->nodes_name_spaces, ast_control_built_in->storage->implicit_values, 1, NULL); compiler_control_built_in->generate();}
    

    if (!__built_in) compiled_output = compiler_control->generateOutPut();
    else compiler_output_built_in = compiler_control_built_in->generateOutPut();

}









