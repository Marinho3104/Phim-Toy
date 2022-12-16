#include "./built_in.h"

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

parser_helper::Type_Information* compiler::Built_In::getReturnTypeOfExpression(parser::Ast_Node_Expression* __expression_node) {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();

    _scope->add(
        (char*) "built_in"
    );

    _scope->add(
        (char*) "Byte"
    );

    parser::Ast_Node_Name_Space* _name_space_node = parser::built_in_ast_control->getNameSpaceNodeFromNameSpaceScope(_scope);

    std::cout << "ADRESSSSSSSS ->>>>>>>>>>>>>" << _name_space_node << std::endl;

    _scope->destroy_content = 0; delete _scope;


    return NULL;

}   





