#include "./astHelper.h"

#include "./../utils/linkedList.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

void parser_helper::setPntrRfrnLevel(parser::Ast_Control* _astCntrl, int* _pntrLevel, int* _rfrnLevel) {

    if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_POINTER) {
        while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_POINTER) { (*_astCntrl->crrntPos)++; (*_pntrLevel)++; }
        if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_ADDRESS) new parser::Ast_Control_Exception("Error nesty tokens");
    }
    else if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_ADDRESS) {
        while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_ADDRESS) { (*_astCntrl->crrntPos)++; (*_rfrnLevel)++; }
        if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_POINTER) new parser::Ast_Control_Exception("Error nesty tokens");
    }

}