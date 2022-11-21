#include "./ast_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./ast.h" // Ast Control
#include "./token.h" // Token

#include <iostream>

void parser_helper::setPointerOperators(parser::Ast_Control* _astCntrl, utils::LinkedList<int>* _operators) {

    while(
        (*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_POINTER ||
        (*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_ADDRESS
    ) _operators->add(
        (*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)++]->id
    );

}
