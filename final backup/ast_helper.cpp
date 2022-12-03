#include "./ast_helper.h"

#include "./../utils/linkedList.h" // Linked List
#include "./parser_definitions.h" // Definitions
#include "./ast_nodes.h" // Nodes
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

parser::Name_Space* parser_helper::getNameSpaceOfStructByDeclarationId(parser::Ast_Control* __astCntrl, parser::Name_Space* _name_space, int __declId) {

    for (int _ = 0; _ < __astCntrl->name_spaces->count; _++) {

        if ((*__astCntrl->name_spaces)[_]->name_space == _name_space)

            for (int __ = 0; __ < (*__astCntrl->name_spaces)[_]->declarations->count; __++)

                if ((*(*__astCntrl->name_spaces)[_]->declarations)[__]->node_id == AST_NODE_STRUCT_DECLARATION)

                    if (((parser::Ast_Node_Struct_Declaration*)(*(*__astCntrl->name_spaces)[_]->declarations)[__])->declaration_id == __declId)

                        return ((parser::Ast_Node_Struct_Declaration*)(*(*__astCntrl->name_spaces)[_]->declarations)[__])->own_name_space;
    
    }

    return NULL;

}

