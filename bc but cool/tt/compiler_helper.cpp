#include "./compiler_helper.h"

#include "./../utils/linkedList.h"
#include "./ast_nodes.h"
#include "./parser_definitions.h"

#include <iostream>


bool parser_helper::confirmFunctionParameters(utils::LinkedList<parser::Ast_Node *>* _funcParams, utils::LinkedList<parser::Ast_Node_Expression *>* _callParams) {

    utils::LinkedList<parser::Ast_Node_Variable_Declaration *>* _vardecl = new utils::LinkedList <parser::Ast_Node_Variable_Declaration*>();
    int _funcParamSize, _callParamSize, _rtr = false;
    _vardecl->destroy_content = 0;

    for (int _ = 0; _ < _funcParams->count; _++)

        if ((*_funcParams)[_]->node_id == AST_NODE_VARIABLE_DECLARATION) _vardecl->add((parser::Ast_Node_Variable_Declaration*) (*_funcParams)[_]);

    if (_vardecl->count != _callParams->count) goto rtr;


    for (int _ = 0; _ < _vardecl->count; _++) {

        _funcParamSize = (*_vardecl)[_]->getByteSize();
        _callParamSize = (*_callParams)[_]->getByteSize();
        
        if (_funcParamSize != _callParamSize) goto rtr;
    }

    _rtr = 1;

rtr:

    delete _vardecl;

    return _rtr;


}


