#include "./ast_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./ast.h" // Ast Control
#include "./token.h" // Token

#include <iostream>

void parser_helper::setPointerOperators(parser::Ast_Control* _astCntrl, utils::LinkedList<int>* _operators) {

    while(
        (*_astCntrl->tokens_collection)[(_astCntrl->current_token_position)]->id == TOKEN_POINTER ||
        (*_astCntrl->tokens_collection)[(_astCntrl->current_token_position)]->id == TOKEN_ADDRESS
    ) _operators->add(
        (*_astCntrl->tokens_collection)[(_astCntrl->current_token_position)++]->id
    );

}
