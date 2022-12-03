#include "./compiler_helper.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./ast_nodes.h"

#include <iostream>


bool parser_helper::confirmFunctionCallParameters(utils::LinkedList<parser::Ast_Node *>* __funcDeclParams, utils::LinkedList<parser::Ast_Node_Expression *>* __funcCallParams) {

    utils::LinkedList<parser::Ast_Node_Variable_Declaration *>* _varDecl = new utils::LinkedList <parser::Ast_Node_Variable_Declaration*>();
    bool _ = 0;

    for (int _ = 0; _ < __funcDeclParams->count; _++)

        if ((*__funcDeclParams)[_]->node_id == AST_NODE_VARIABLE_DECLARATION) _varDecl->add((parser::Ast_Node_Variable_Declaration*) (*__funcDeclParams)[_]);

    if (_varDecl->count != __funcCallParams->count) goto rtr;

    for (int _ = 0; _ < _varDecl->count; _++) 

        if (
            (*_varDecl)[_]->getByteSize() != (*__funcCallParams)[_]->getByteSize()
        ) goto rtr;

    _ = 1;

rtr:

    _varDecl->destroy_content = 0;

    delete _varDecl;

    return _;

}


