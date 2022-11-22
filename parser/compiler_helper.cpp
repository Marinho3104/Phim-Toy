#include "./compiler_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./compiler.h" // Compiler Control
#include "./ast_nodes.h" // Ast Nodes 
#include "./ast.h" // Storage
#include <iostream>

bool parser_helper::confirmFunctionParameters(utils::LinkedList<parser::Ast_Node *>* _f, utils::LinkedList<parser::Ast_Node *>* _s, parser::Compiler_Control* _comCntrl) {

    if (_f->count != _s->count) return false;

    for (int _ = 0; _ < _f->count; _++) {

        if (
            ((parser::Ast_Node_Variable_Declaration*) (*_f)[_])->getTypeSize(_comCntrl->ast_storage->types) != ((parser::Ast_Node_Expression*) (*_s)[_])->getResultSize(_comCntrl->varDecl, _comCntrl->funcDecl, _comCntrl->ast_storage->types)
        ) return false;

    }

    return true;

}
