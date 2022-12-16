#include "./built_in.h"

#include "./../parser/ast_helper.h"
#include "./../utils/linkedList.h"
#include "./../parser/tokenizer.h"
#include "./../parser/ast_nodes.h"
#include "./../utils/common.h"
#include "./../parser/ast.h"
#include "./compiler.h"


#include <iostream>

compiler::Built_In::~Built_In() { delete parser::built_in_ast_control; delete parser::built_in_name_space_control; }

compiler::Built_In::Built_In() { generate(); }

char* compiler::Built_In::getBuiltInCode() { return utils::getFileContent( (char*) "./../built_in/byte.ph"); }

void compiler::Built_In::generate() {

    char* _code = getBuiltInCode();

    parser::setUpTokenizer(_code, 0);

    parser::tokenizer_control->generate();

    parser::setUpAst(0);

    parser::ast_control->generate();

    parser::built_in_name_space_control = parser::name_space_control;
    parser::built_in_ast_control = parser::ast_control;

    parser::cleanTokenizer();

    free(_code);

}

parser_helper::Type_Information* compiler::Built_In::getReturnTypeOfExpression(parser_helper::Expression_Variable_Declaration* __expression_node) {

    
    

    return NULL;

}   





