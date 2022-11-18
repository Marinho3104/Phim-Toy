#include "./ast_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./token.h" // Tokens id
#include "./ast.h" // Ast Control

#include <iostream>

void parser_helper::setPntrRfrnLevel(parser::Ast_Control* _astCntrl, int* _pntrLvl, int* _rfrnLvl) {

    if ((*_astCntrl->tokensColl)[(_astCntrl->crrntTk)]->id == TOKEN_POINTER) {
        while((*_astCntrl->tokensColl)[(_astCntrl->crrntTk)]->id == TOKEN_POINTER) { (_astCntrl->crrntTk)++; (*_pntrLvl)++; }
    }

}

