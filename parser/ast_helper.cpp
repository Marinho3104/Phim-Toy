#include "./ast_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./ast.h" // Ast Control
#include "./token.h" // Token

#include <iostream>

void parser_helper::setPointerOperators(parser::Ast_Control* __astCntrl, utils::LinkedList<int>* _operators, bool _isUserDefined) {

    if (_isUserDefined) 

        if (__astCntrl->getToken(0)->id == TOKEN_MULTIPLICATION || __astCntrl->getToken(0)->id == TOKEN_BITWISEAND)
            _operators->add(
                (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)++]->id == TOKEN_MULTIPLICATION ? TOKEN_POINTER : TOKEN_ADDRESS
            );

    while(
        (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id == TOKEN_POINTER || (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id == TOKEN_ADDRESS
    ) _operators->add(
        (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)++]->id
    );

}
