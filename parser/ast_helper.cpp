#include "./ast_helper.h"

#include "./../utils/commonFunctions.h"
#include "./../utils/linkedList.h"
#include "./token.h"
#include "./ast.h"
#include "./ast_nodes.h"

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

parser::Name_Space* parser_helper::checkIfIsNameSpaceChanging(parser::Ast_Control* __ast_control) {

    parser::Name_Space* _name_space = NULL;

    if (__ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR || __ast_control->getToken(1)->id == TOKEN_NAMESPACE_OPERATOR) {

        utils::LinkedList <char*>* _scope = getNameSpaceScope(__ast_control);

        _name_space = __ast_control->name_space_control->getNameSpace(_scope);

        while(!_name_space) { // need to decrease token position

            utils::DataLL <char*>* _last_name = _scope->removeLast();

            if (!_last_name) break;

            _name_space = __ast_control->name_space_control->getNameSpace(_scope);

            delete _last_name;

        }

        delete _scope;

    }

    return _name_space;

}

utils::LinkedList <char*>* parser_helper::getNameSpaceScope(parser::Ast_Control* __ast_control) {

    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();
    
    if (__ast_control->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR) {

        for (int _ = 0; _ < __ast_control->current_name_space->scope->count; _++)

            _scope->add(
                utils::copyString(
                    (*__ast_control->current_name_space->scope)[_], 
                    utils::getStringSize((*__ast_control->current_name_space->scope)[_])
                )
            );

    } else __ast_control->current_token_position++;

    int _expected = TOKEN_IDENTIFIER;

    while(__ast_control->getToken(0)->id == TOKEN_IDENTIFIER || __ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR) {

        if (_expected != __ast_control->getToken(0)->id) {

            if (_expected == TOKEN_NAMESPACE_OPERATOR) break;

            new parser::Ast_Exception("Unexpected token - getNameSpaceScope()");

        }

        else if (_expected != TOKEN_NAMESPACE_OPERATOR) 

            _scope->add(
                utils::copyString(__ast_control->getToken(0)->phr, utils::getStringSize(__ast_control->getToken(0)->phr))
            );
        
        __ast_control->current_token_position ++;

        _expected = _expected == TOKEN_IDENTIFIER ? TOKEN_NAMESPACE_OPERATOR : TOKEN_IDENTIFIER;

    }

    return _scope;

}

int parser_helper::getDeclarationId(parser::Ast_Control* __ast_control) {

    int _declId;

    if (__ast_control->current_code_block) {

        if (
            (_declId = __ast_control->current_code_block->getDeclarationId(
                __ast_control->getToken(0)->phr
            )) == -1
        ) goto current_name_space;

    }

    else {

    current_name_space:
        _declId = __ast_control->current_name_space->getDeclarationId(
            __ast_control->getToken(0)->phr
        );

    }

    return _declId;

}

int parser_helper::addToNameTracker(parser::Ast_Control* __ast_control) {

    int _declaration_id;

    if (__ast_control->current_code_block) 

        __ast_control->current_code_block->name_tracker->addNewName(
            __ast_control->getToken(0)->phr
        );

    else 

        __ast_control->current_name_space->addNewName( 
            __ast_control->getToken(0)->phr
        );

    _declaration_id = getDeclarationId(__ast_control);

    return _declaration_id;

}


int parser_helper::addToNameTracker(parser::Ast_Control* __ast_control, char* __n) {

    int _declaration_id;

    if (__ast_control->current_code_block) 

        __ast_control->current_code_block->name_tracker->addNewName(
            __n
        );

    else 

        __ast_control->current_name_space->addNewName( 
            __n
        );

    _declaration_id = getDeclarationId(__ast_control);

    return _declaration_id;

}

