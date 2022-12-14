#include "./ast_helper.h"

#include "./../built_in/built_in_definitions.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./ast_node.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>
#include <string.h>

parser_helper::Type_Information::~Type_Information() {

}

parser_helper::Type_Information::Type_Information(parser::Name_Space* __name_space, int __token_id, parser::Ast_Node_Struct_Declaration* __declaration, utils::Linked_List <int>* __pointer_operations) 
    : pointer_level(0), reference_level(0), token_id(__token_id), declaration(__declaration), name_space(__name_space) {

        if (!__pointer_operations) return;

        for (int _ = 0; _ < __pointer_operations->count; _++) 
        
            if (__pointer_operations->operator[](_) == TOKEN_POINTER) {

                if (reference_level) 

                    new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(_ - parser::ast_control->current_token_position), "Pointer of reference not allowed");

                ++pointer_level;

            }

            else

                if (++reference_level > 2) 
                    new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(_ - parser::ast_control->current_token_position), "Reference of double reference not allowed");

}

bool parser_helper::Type_Information::operator==(Type_Information* __to_compare) {

    return (
        declaration == __to_compare->declaration &&
        pointer_level == __to_compare->pointer_level &&
        reference_level == __to_compare->reference_level &&
        token_id == __to_compare->token_id
    );

}

bool parser_helper::Type_Information::operator!=(Type_Information* __to_compare) { return !operator==(__to_compare); }

parser_helper::Type_Information* parser_helper::Type_Information::generate() {



}

parser_helper::Type_Information* parser_helper::Type_Information::generate(Type_Information* __type_information) {

    utils::Linked_List <int>* _pointer_operations = getPointerOperations(0);

    Type_Information* _type_information = 
        new Type_Information(__type_information->name_space, __type_information->token_id, __type_information->declaration, _pointer_operations);

    delete _pointer_operations;

    return _type_information;

}

int parser_helper::Type_Information::getByteSize() {

    if (!pointer_level)
        ;
        // return token_id == TOKEN_IDENTIFIER ? declaration->getByteSize() : getSizePrimitiveType(token_id);

    return PRIMITIVES_TYPE_POINTER_SIZE;

}

int parser_helper::Type_Information::getByteSize(int __token_id) {

    switch (__token_id)
    {
    case TOKEN_NUMBERINT: return 4;    
    default: break;
    }

    new parser::Exception_Handle("Implicit value token id invalid");

    return -1;

}


utils::Linked_List <int>* parser_helper::getPointerOperations(bool __is_user_defined) {

    utils::Linked_List <int>* _pointer_operations = new utils::Linked_List <int>();

    if (__is_user_defined) {

        if (parser::ast_control->getToken(0)->id == TOKEN_MULTIPLICATION || parser::ast_control->getToken(0)->id == TOKEN_BITWISEAND) {

            _pointer_operations->add(
                parser::ast_control->getToken(0)->id == TOKEN_MULTIPLICATION ? TOKEN_POINTER : TOKEN_ADDRESS
            );

            parser::ast_control->current_token_position++;

        }

        else return _pointer_operations;

    }

    while(parser::ast_control->getToken(0)->id == TOKEN_POINTER || parser::ast_control->getToken(0)->id == TOKEN_ADDRESS) 
        { _pointer_operations->add(parser::ast_control->getToken(0)->id); parser::ast_control->current_token_position++; }

    return _pointer_operations;

}

utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* parser_helper::getVariableDeclarationsFromLinkedList(utils::Linked_List <parser::Ast_Node*>* __linked_list) {

    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* _variable_declarations = new utils::Linked_List <parser::Ast_Node_Variable_Declaration*>();

    for (int _ = 0; _ < __linked_list->count; _++)

        if (__linked_list->operator[](_)->node_id == AST_NODE_VARIABLE_DECLARATION)

            _variable_declarations->add(
                (parser::Ast_Node_Variable_Declaration*) __linked_list->operator[](_)
            );

    return _variable_declarations;

}
    
int parser_helper::getNodeType() {

    switch (parser::ast_control->getToken(0)->id)
    {
    case TOKEN_CLOSECURLYBRACKET: return -1;
    case TOKEN_NAMESPACE: return AST_NODE_NAME_SPACE;
    
    default: break;
    }

    new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Unexpected token node type");

    return -1;

}

utils::Linked_List <char*>* parser_helper::getScope() {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();

    int _expected = TOKEN_IDENTIFIER;

    while(parser::ast_control->getToken(0)->id == _expected) {

        if (_expected == TOKEN_IDENTIFIER) {

            char* _name = (char*) malloc(strlen(parser::ast_control->getToken(0)->phr) + 1);

            strcpy(_name, parser::ast_control->getToken(0)->phr);

            _scope->add(_name);

            _expected = TOKEN_NAMESPACE_OPERATOR;

        } else _expected = TOKEN_IDENTIFIER;

        parser::ast_control->current_token_position++;

    }

    if (_expected == TOKEN_IDENTIFIER) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Expected identifier");

    return _scope;

}

parser::Name_Space* parser_helper::getNameSpace() {

    if (parser::ast_control->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR && parser::ast_control->getToken(1)->id != TOKEN_NAMESPACE_OPERATOR) return NULL;
    if (parser::ast_control->getToken(0)->id != TOKEN_IDENTIFIER && parser::ast_control->getToken(1)->id != TOKEN_IDENTIFIER) return NULL;

    bool _is_global;

    if (_is_global = (parser::ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR)) parser::ast_control->current_token_position++;

    utils::Linked_List <char*>* _name_space_scope = getScope();

    if (!_is_global) {

        utils::Linked_List <char*>* _current_name_space_scope = parser::ast_control->name_space_node_chain->last->object->scope;

        char* _name_scope;

        for (int _ = 0; _ < _current_name_space_scope->count; _++) {

            _name_scope = (char*) malloc(strlen(_current_name_space_scope->operator[](_)) + 1);

            strcpy(_name_scope, _current_name_space_scope->operator[](_));

            _name_space_scope->insert(_name_scope, _);

        }


    }

    delete _name_space_scope->remove(_name_space_scope->count);
    parser::ast_control->current_token_position--;

    parser::Name_Space* _name_space_return = parser::name_space_control->getNameSpace(_name_space_scope);

    if (!_name_space_return) {

        utils::Data_Linked_List <char*>* _data_linked_struct_name = _name_space_scope->remove(_name_space_scope->count);

        if (_name_space_scope->count) {

            parser::Name_Space* _name_space = parser::name_space_control->getNameSpace(_name_space_scope);

            if (!_name_space) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Undefined name space");

            parser::ast_control->addNameSpaceNodeToChain(_name_space);

        }

        int _declaration_id = parser_helper::getDeclarationId(parser::ast_control, _data_linked_struct_name->object);

        delete _data_linked_struct_name;

        if (_declaration_id != -1) {

            parser::Ast_Node_Name_Space* _name_space_node = parser::ast_control->getNameSpaceNodeFromNameSpace(parser::ast_control->name_space_chain->last->object);

            if (!_name_space_node) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Undefined name space");

            parser::Ast_Node_Struct_Declaration* _struct_declaration = _name_space_node->getStructDeclaration(_declaration_id);

            _name_space_return = _struct_declaration->functions->name_space;

        }

        parser::ast_control->popNameSpace();

    }

    if (!_name_space_return) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Undefined name space");

    delete _name_space_scope;

    return _name_space_return;

}

