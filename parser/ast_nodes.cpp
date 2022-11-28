#include "./ast_nodes.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __id) : node_id(__id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { if (declarations) delete declarations;  }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __nmSpc, utils::LinkedList <Ast_Node*>* __decl) 
    : Ast_Node(AST_NODE_NAME_SPACE), name_space(__nmSpc), declarations(__decl) {}

void parser::Ast_Node_Name_Space::generate(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    __astCntrl->printDebugInfo("--> Ast Node Name Space <--");

    parser::Ast_Node_Name_Space* _ = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));

    Name_Space* _prevNameSpace = __astCntrl->current_name_space;

    __astCntrl->current_name_space = __nmSpc;

    utils::LinkedList <parser::Ast_Node*>* _declrs = getDeclarations(__astCntrl);

    new (_) parser::Ast_Node_Name_Space(
        __nmSpc, _declrs
    );

    __astCntrl->current_name_space = _prevNameSpace;

    __astCntrl->name_spaces->add(_);

    __astCntrl->printDebugInfo("--> Ast Node Name Space End <--");

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarations(Ast_Control* __astCntrl) {

    utils::LinkedList <Ast_Node*>* _body, *body = new utils::LinkedList <Ast_Node*>();
    bool _sts;

    while(__astCntrl->current_token_position < __astCntrl->tokens_collection->count) {
        
        _body = getDeclarationsSingle(__astCntrl);

        _sts = body->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

    return body;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarationsSingle(Ast_Control* __astCntrl) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Token* _tk = __astCntrl->getToken(0);
    parser::Token* _tkNext = __astCntrl->getToken(1);

    // std::cout << "Token 1 -> " << _tk->id << std::endl;
    // std::cout << "Token 2 -> " << _tkNext->id << std::endl;

    switch (_tk->id)
    {
        case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
        case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getDeclarationsSingle(__astCntrl); break;
        case TOKEN_NAMESPACE:
            {
                __astCntrl->current_token_position++;

                Name_Space* _nameSpace = Name_Space::getNameSpace(
                    __astCntrl
                );

                __astCntrl->current_token_position++;

                Ast_Node_Name_Space::generate(
                    __astCntrl, _nameSpace
                );

                delete _;

                _ = getDeclarationsSingle(__astCntrl);

                break;

            }
        default: goto cont;
    }

    return _;

cont:

    if (parser::isPrimativeType(_tk)) {
        Type_Information* _typeInformation = Type_Information::generate(__astCntrl);

        delete _;

        _ = Ast_Node_Variable_Declaration::generate(
            __astCntrl, _typeInformation
        );

    } 
    
    else new Ast_Execption("Declaration not allowed");

    return _;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete variable_type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* __varTp, int __declId) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), variable_type(__varTp), declaration_id(__declId) {

    }

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* __astCntrl, Type_Information* __varTp) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _varDecl;
    int _declId;

    while(__astCntrl->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        __astCntrl->printDebugInfo("--> Ast Node Variable Declaration <--");

        __astCntrl->current_name_space->addNewName(
            __astCntrl->getToken(0)->phr
        );

        _declId = __astCntrl->current_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

        __astCntrl->current_token_position++;

        std::cout << "Declaration id -> " << _declId << std::endl;

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(__varTp, _declId);

        _->add(_varDecl);

        if (__astCntrl->getToken(0)->id == TOKEN_EQUAL) {
            new Ast_Execption("TODO ");
        }

        if (__astCntrl->getToken(0)->id == TOKEN_COMMA) {
            __astCntrl->current_token_position++;
            __varTp = Type_Information::generate(__astCntrl, __varTp);
        }

    }

    return _;

}
