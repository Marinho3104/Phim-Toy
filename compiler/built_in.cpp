#include "./built_in.h"

#include "./../parser/token_definitions.h"
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

parser::Ast_Node_Function_Declaration* compiler::Built_In::getFunctionDeclaration(
    parser_helper::Expression_Variable_Declaration* __expression_variable_declaration) {

        utils::Linked_List <parser::Ast_Node*>* __parameters = new utils::Linked_List <parser::Ast_Node*>();
        __parameters->destroy_content = 0;

        __parameters->add(
            __expression_variable_declaration->declaration
        );

        __parameters->add(
            __expression_variable_declaration->expression->declaration
        );


        int _function_declaration_id;

        switch (__expression_variable_declaration->operator_id)
        {
        case TOKEN_ADDITION: _function_declaration_id = parser_helper::getDeclarationId("addition", 0); break;
        case TOKEN_SUBTRACTION: _function_declaration_id = parser_helper::getDeclarationId("subtraction", 0); break;
        case TOKEN_MULTIPLICATION: _function_declaration_id = parser_helper::getDeclarationId("multiplication", 0); break;
        case TOKEN_DIVISION: _function_declaration_id = parser_helper::getDeclarationId("division", 0); break;
        case TOKEN_MODULUS: _function_declaration_id = parser_helper::getDeclarationId("modulos", 0); break;
        default: std::cout << "error getting function declaration" << std::endl; exit(1);
        }

        parser::Ast_Node_Function_Declaration* _function_declaration = parser_helper::getFunctionDeclaration(_function_declaration_id, __parameters, 0);
        
        delete __parameters;

        return _function_declaration;

}

parser_helper::Type_Information* compiler::Built_In::getReturnTypeOfExpression(parser_helper::Expression_Variable_Declaration* __expression_node) {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>(); _scope->destroy_content = 0;
    parser::Ast_Node_Name_Space* _name_space_node;

    parser::Name_Space_Control* _temp_current_name_space_control = parser::name_space_control;
    parser::Ast_Control* _temp_current_ast_control = parser::ast_control;

    parser::Ast_Node_Function_Declaration* _function_declaration;

    parser::name_space_control = parser::built_in_name_space_control;
    parser::ast_control = parser::built_in_ast_control;

    _scope->add(
        (char*) "built_in"
    );

    switch (__expression_node->declaration->type->token_id)
    {
    case TOKEN_TYPE_BYTE:
        
        _scope->add(
            (char*) "Byte"
        );

        _name_space_node = parser::built_in_ast_control->getNameSpaceNodeFromNameSpaceScope(_scope);

        parser::built_in_ast_control->addNameSpaceToChain(_name_space_node);

        _function_declaration = getFunctionDeclaration(__expression_node);

        parser::built_in_ast_control->popNameSpaceFromChain();

        break;

    case TOKEN_TYPE_INT:
  
        _scope->add(
            (char*) "Int"
        );

        _name_space_node = parser::built_in_ast_control->getNameSpaceNodeFromNameSpaceScope(_scope);

        parser::built_in_ast_control->addNameSpaceToChain(_name_space_node);

        _function_declaration = getFunctionDeclaration(__expression_node);

        parser::built_in_ast_control->popNameSpaceFromChain();

        break;
    
    default:
        break;
    }

    parser::name_space_control = _temp_current_name_space_control;
    parser::ast_control = _temp_current_ast_control;
    
    delete _scope;

    return _function_declaration ? _function_declaration->return_type : NULL;

}   





