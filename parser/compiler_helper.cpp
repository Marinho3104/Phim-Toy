#include "./compiler_helper.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"

#include <iostream>

bool parser_helper::confirmFunctionCallParameters(
    utils::LinkedList <parser::Ast_Node*>* __function_declaration_parameters, utils::LinkedList <parser::Ast_Node_Expression*>* __function_call_parameters) {

        utils::LinkedList <parser::Ast_Node_Variable_Declaration*>* _variable_declarations = new utils::LinkedList <parser::Ast_Node_Variable_Declaration*>();
        bool _status = true;

        for (int _  = 0; _ < __function_declaration_parameters->count; _++)

            if ((*__function_declaration_parameters)[_]->node_id == AST_NODE_VARIABLE_DECLARATION)

                _variable_declarations->add(
                    (parser::Ast_Node_Variable_Declaration*) (*__function_declaration_parameters)[_]
                );

        if (_variable_declarations->count != __function_call_parameters->count) _status = 0;

        _variable_declarations->destroy_content = 0; delete _variable_declarations;

        return _status;

}

