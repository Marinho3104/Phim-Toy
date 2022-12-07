#include "./compiler.h"

#include "./../utils/commonFunctions.h"
#include "./../parser/compiler_ast.h"
#include "./../utils/linkedList.h"
#include "./../parser/tokenizer.h"
#include "./../parser/ast.h"

#include <iostream>

compiler::Compiler::~Compiler() { 
    delete tokenizer_control; delete ast_control; delete compiler_control;

    for (int _ = 0; _ < built_in_code_blocks->count; _++)

        delete (*built_in_code_blocks)[_]->byte_code;

    delete built_in_code_blocks;
    delete compiled_output;
}

compiler::Compiler::Compiler(char* __code) : code(__code) { getBuiltInCodeBlock(); }

void compiler::Compiler::getBuiltInCodeBlock() {

    built_in_code = utils::getFileContent("./../built_in/byte.ph");

    runTokenizer(1);

    runAst(1);

    runCompiler(1);

    free(built_in_code);

    delete tokenizer_control; delete ast_control; delete compiler_control;

    std::cout << "------------------------------------------------------------------------------------------------------------------------------" << std::endl;

}

void compiler::Compiler::generateByteCode() {

    runTokenizer(0);

    runAst(0);

    runCompiler(0);

}

void compiler::Compiler::runTokenizer(bool __built_in) {

    tokenizer_control = new parser::Tokenizer_Control(__built_in ? built_in_code : code); 
    
    tokenizer_control->generateTokens();

}

void compiler::Compiler::runAst(bool) {

    ast_control = new parser::Ast_Control(tokenizer_control->tokens, 1); 
    
    ast_control->generate();

}

void compiler::Compiler::runCompiler(bool __built_in) {

    if (__built_in)

        compiler_control = new parser::Compiler_Control(ast_control->nodes_name_spaces, ast_control->storage->implicit_values); 
    
    else 
        compiler_control = new parser::Compiler_Control(ast_control->nodes_name_spaces, ast_control->storage->implicit_values, built_in_code_blocks, 1);

    compiler_control->generate();

    if (__built_in) built_in_code_blocks = compiler_control->compiler_code_blocks;

    else compiled_output = compiler_control->generateOutPut(); 

}



