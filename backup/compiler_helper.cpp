#include "./compiler_helper.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./compiler_ast.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

bool parser_helper::confirmFunctionCallParameters(
    parser::Compiler_Control* __compiler_control, utils::LinkedList <parser::Ast_Node*>* __function_declaration_parameters, utils::LinkedList <parser::Ast_Node_Expression*>* __function_call_parameters) {

        utils::LinkedList <parser::Ast_Node_Variable_Declaration*>* _variable_declarations = new utils::LinkedList <parser::Ast_Node_Variable_Declaration*>();
        bool _status = true;

        for (int _  = 0; _ < __function_declaration_parameters->count; _++)

            if ((*__function_declaration_parameters)[_]->node_id == AST_NODE_VARIABLE_DECLARATION)

                _variable_declarations->add(
                    (parser::Ast_Node_Variable_Declaration*) (*__function_declaration_parameters)[_]
                );

        if (_variable_declarations->count != __function_call_parameters->count) _status = 0;

        for (int _ = 0; _ < _variable_declarations->count; _++)

            std::cout << "Expression size -> " << getSizeOfExpression(__compiler_control, (*__function_call_parameters)[_]) << std::endl;

        _variable_declarations->destroy_content = 0; delete _variable_declarations;

        return _status;

}

int parser_helper::getSizeOfExpression(parser::Compiler_Control* __compiler_control, parser::Ast_Node_Expression* __node_expression) {

    utils::LinkedList <char*>* _scope = getScopeOfPrimitiveType(((parser::Ast_Node_Variable*)__node_expression->first_expression)->variable_declaration->type);

    parser::Compiler_Code_Block* _code_block = __compiler_control->getCompilerCodeBlockFromNameSpaceBuiltIn(_scope);

    std::cout << _code_block << std::endl;

    _scope->destroy_content = 0; delete _scope;

    return 0;

}

utils::LinkedList <char*>* parser_helper::getScopeOfPrimitiveType(parser::Type_Information* __type_information) {

    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();

    _scope->add("built_in");

    switch (__type_information->token_id)
    {
    case TOKEN_TYPE_BYTE:

        _scope->add("Byte");
        break;
    
    default:
        break;
    }

    return _scope;

}

