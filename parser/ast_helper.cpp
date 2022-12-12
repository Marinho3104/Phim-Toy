#include "./ast_helper.h"

#include "./../built_in/built_in_definitions.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./token_definitions.h"
#include "./exception_handle.h"
#include "./ast_nodes.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>
#include <string.h>

parser_helper::Type_Information::~Type_Information() {

}

parser_helper::Type_Information::Type_Information(parser::Ast_Control* __ast_control, parser::Name_Space* __name_space, int __token_id, int __declaration_id, utils::Linked_List <int>* __pointer_operations) 
    : pointer_level(0), reference_level(0), token_id(__token_id), declaration_id(__declaration_id), ast_control(__ast_control), name_space(__name_space) {

        if (!__pointer_operations) return;

        for (int _ = 0; _ < __pointer_operations->count; _++) 
        
            if (__pointer_operations->operator[](_) == TOKEN_POINTER) {

                if (reference_level) 

                    new parser::Exception_Handle(__ast_control, __ast_control->getToken(_ - __ast_control->current_token_position), "Pointer of reference not allowed");

                ++pointer_level;

            }

            else

                if (++reference_level > 2) 
                    new parser::Exception_Handle(__ast_control, __ast_control->getToken(_ - __ast_control->current_token_position), "Reference of double reference not allowed");

}

parser_helper::Type_Information* parser_helper::Type_Information::generate(parser::Ast_Control* __ast_control, parser::Name_Space* __name_space) {

    utils::Linked_List <int>* _pointer_operations;
    int _id = __ast_control->getToken(0)->id, _declaration_id = 0;

    if (__name_space) __ast_control->addNameSpace(__name_space);

    if (parser::isPrimitiveTokenId(_id));
    else if (_id == TOKEN_IDENTIFIER) { 

        _declaration_id = parser_helper::getDeclarationId(__ast_control, __ast_control->getToken(0)->phr); 

        parser::Ast_Node_Name_Space* _name_space_node = __ast_control->getNameSpaceNodeFromNameSpace(__ast_control->name_space_chain->last->object);

        if (_declaration_id == -1 || !_name_space_node->getStructDeclaration(_declaration_id)) {
            if (__name_space) __ast_control->popNameSpace();
            new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Undefined type");
        }
    }
    else new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected primitive type or user defined type"); // Should never reach this 

    __ast_control->current_token_position++;

    _pointer_operations = getPointerOperations(__ast_control, _id == TOKEN_IDENTIFIER);

    Type_Information* _type_information = new Type_Information(__ast_control, __name_space, _id, _declaration_id, _pointer_operations);

    delete _pointer_operations;

    if (__name_space) __ast_control->popNameSpace();

    return _type_information;

}

parser_helper::Type_Information* parser_helper::Type_Information::generate(parser::Ast_Control* __ast_control, Type_Information* __type_information) {

    utils::Linked_List <int>* _pointer_operations = getPointerOperations(__ast_control, 0);

    Type_Information* _type_information = 
        new Type_Information(__ast_control, __type_information->name_space, __type_information->token_id, __type_information->declaration_id, _pointer_operations);

    delete _pointer_operations;

    return _type_information;

}

int parser_helper::Type_Information::getByteSize() {

    if (!pointer_level) {

        if (token_id == TOKEN_IDENTIFIER) {
            std::cout << "TODO get size of user defined type Type Information" << std::endl;

            exit(1);
        }

        return getSizePrimitiveType(token_id);

    }

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


utils::Linked_List <int>* parser_helper::getPointerOperations(parser::Ast_Control* __ast_control, bool __is_user_defined) {

    utils::Linked_List <int>* _pointer_operations = new utils::Linked_List <int>();

    if (__is_user_defined) {

        if (__ast_control->getToken(0)->id == TOKEN_MULTIPLICATION || __ast_control->getToken(0)->id == TOKEN_BITWISEAND) {

            _pointer_operations->add(
                __ast_control->getToken(0)->id == TOKEN_MULTIPLICATION ? TOKEN_POINTER : TOKEN_ADDRESS
            );

            __ast_control->current_token_position++;

        }

        else return _pointer_operations;

    }

    while(__ast_control->getToken(0)->id == TOKEN_POINTER || __ast_control->getToken(0)->id == TOKEN_ADDRESS) 
        { _pointer_operations->add(__ast_control->getToken(0)->id); __ast_control->current_token_position++; }

    return _pointer_operations;

}

int parser_helper::getNodeType(parser::Ast_Control* __ast_control) {

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_CLOSECURLYBRACKET: return GET_NODE_TYPE_TOKEN_CLOSE_CURLY_BRACKET;
    case TOKEN_ENDINSTRUCTION: return GET_NODE_TYPE_TOKEN_END_INSTRUCTION;
    case TOKEN_CLOSEPARENTHESES: return GET_NODE_TYPE_TOKEN_CLOSE_CURLY_BRACKET;
    case TOKEN_NAMESPACE: return AST_NODE_NAME_SPACE;
    case TOKEN_OPENCURLYBRACKET: return AST_NODE_CODE_BLOCK;
    case TOKEN_STRUCT: case TOKEN_CONTRACT: return AST_NODE_STRUCT_DECLARATION;
    case TOKEN_POINTER: case TOKEN_ADDRESS: return AST_NODE_POINTER_OPERATOR;
    case TOKEN_OPENPARENTHESES: return AST_NODE_PARENTHESIS;
    default: break;
    }

    int _backup_position = __ast_control->current_token_position;

    parser::Name_Space* _name_space = getNameSpace(__ast_control);

    if (parser::isPrimitiveTokenId(__ast_control->getToken(0)->id) || __ast_control->getToken(0)->id == TOKEN_IDENTIFIER) { // Accept struct

        Type_Information* _type;
        int _return_type;

        try { _type = Type_Information::generate(__ast_control, _name_space); }
        catch(...) { _return_type = (__ast_control->getToken(1)->id == TOKEN_OPENPARENTHESES) ? AST_NODE_FUNCTION_CALL : AST_NODE_VARIABLE; goto reset; }

        getNameSpace(__ast_control);

        switch (__ast_control->getToken(1)->id)
        {
        case TOKEN_EQUAL: case TOKEN_COMMA: case TOKEN_ENDINSTRUCTION: _return_type = AST_NODE_VARIABLE_DECLARATION; break;
        case TOKEN_OPENPARENTHESES: _return_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: new parser::Exception_Handle(__ast_control, __ast_control->getToken(1), "Unexpected token aqui"); break;
        }

        delete _type;
        
    reset:
        __ast_control->current_token_position = _backup_position;

        return _return_type;

    }

    else if (parser::isImplicitValue(__ast_control->getToken(0)->id)) return AST_NODE_VALUE;

    else if (parser::isAssignmentOperator(__ast_control->getToken(0)->id)) return AST_NODE_ASSIGNMENT;

    else if (parser::isExpressionOperator(__ast_control->getToken(0)->id)) return AST_NODE_EXPRESSION;

    std::cout << __ast_control->getToken(0)->id << std::endl;

    new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token node type");

    return -1;

}

parser::Ast_Node* parser_helper::checkNext(parser::Ast_Control* __ast_control, parser::Ast_Node* __node) {

    switch (getNodeType(__ast_control))
    {
    case AST_NODE_ASSIGNMENT: return parser::Ast_Node_Assignment::generate(__ast_control, __node); break;
    
    default:
        break;
    }

    return __node;

}

utils::Linked_List <char*>* parser_helper::getNameSpaceScope(parser::Ast_Control* __ast_control) {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();

    int _expected = TOKEN_IDENTIFIER;

    while(__ast_control->getToken(0)->id == _expected) {

        if (_expected == TOKEN_IDENTIFIER) {

            char* _name = (char*) malloc(strlen(__ast_control->getToken(0)->phr) + 1);

            strcpy(_name, __ast_control->getToken(0)->phr);

            _scope->add(_name);

            _expected = TOKEN_NAMESPACE_OPERATOR;

        } else _expected = TOKEN_IDENTIFIER;

        __ast_control->current_token_position++;

    }

    if (_expected == TOKEN_IDENTIFIER) new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected identifier");

    return _scope;

}

parser::Name_Space* parser_helper::getNameSpace(parser::Ast_Control* __ast_control) {

    if (__ast_control->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR && __ast_control->getToken(1)->id != TOKEN_NAMESPACE_OPERATOR) return NULL;
    if (__ast_control->getToken(0)->id != TOKEN_IDENTIFIER && __ast_control->getToken(1)->id != TOKEN_IDENTIFIER) return NULL;

    bool _is_global;

    if (_is_global = (__ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR)) __ast_control->current_token_position++;

    utils::Linked_List <char*>* _name_space_scope = getNameSpaceScope(__ast_control);

    if (!_is_global) {

        utils::Linked_List <char*>* _current_name_space_scope = __ast_control->name_space_chain->last->object->scope;

        char* _name_scope;

        for (int _ = 0; _ < _current_name_space_scope->count; _++) {

            _name_scope = (char*) malloc(strlen(_current_name_space_scope->operator[](_)) + 1);

            strcpy(_name_scope, _current_name_space_scope->operator[](_));

            _name_space_scope->insert(_name_scope, _);

        }


    }

    delete _name_space_scope->remove(_name_space_scope->count);
    __ast_control->current_token_position--;

    parser::Name_Space* _name_space_return = __ast_control->name_space_control->getNameSpace(_name_space_scope);

    if (!_name_space_return) {

        utils::Data_Linked_List <char*>* _data_linked_struct_name = _name_space_scope->remove(_name_space_scope->count);

        if (_name_space_scope->count) {

            parser::Name_Space* _name_space = __ast_control->name_space_control->getNameSpace(_name_space_scope);

            if (!_name_space) new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Undefined name space");

            __ast_control->addNameSpace(_name_space);

        }

        int _declaration_id = parser_helper::getDeclarationId(__ast_control, _data_linked_struct_name->object);

        delete _data_linked_struct_name;

        if (_declaration_id != -1) {

            parser::Ast_Node_Name_Space* _name_space_node = __ast_control->getNameSpaceNodeFromNameSpace(__ast_control->name_space_chain->last->object);

            if (!_name_space_node) new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Undefined name space");

            parser::Ast_Node_Struct_Declaration* _struct_declaration = _name_space_node->getStructDeclaration(_declaration_id);

            _name_space_return = _struct_declaration->functions->name_space;

        }

        __ast_control->popNameSpace();

    }

    if (!_name_space_return) new parser::Exception_Handle(__ast_control, __ast_control->getToken(0), "Undefined name space");

    delete _name_space_scope;

    return _name_space_return;

}

int parser_helper::addName(parser::Ast_Control* __ast_control, char* __to_add) {

    if (__ast_control->code_block_chain->last && __ast_control->code_block_chain->last->object) 
        __ast_control->code_block_chain->last->object->name_tracker->addName(__to_add);

    else __ast_control->name_space_chain->last->object->addName(__to_add);

    return getDeclarationId(__ast_control, __to_add);

}

int parser_helper::getDeclarationId(parser::Ast_Control* __ast_control, char* __to_check) {

    int _declaration_id;

    if (__ast_control->code_block_chain->last && __ast_control->code_block_chain->last->object) {

        _declaration_id = __ast_control->code_block_chain->last->object->getDeclarationId(__to_check);

        if (_declaration_id != -1) return _declaration_id;

    } 

    return __ast_control->name_space_chain->last->object->getDeclarationId(__to_check);

}

bool parser_helper::isGlobalDeclaration(parser::Ast_Control* __ast_control, char* __to_check) {

    if (__ast_control->code_block_chain->last && __ast_control->code_block_chain->last->object) 
        return __ast_control->code_block_chain->last->object->isGlobalDeclaration(__to_check);

    return 1; 

}

int parser_helper::getSizePrimitiveType(int __id) {

    switch (__id)
    {
    case TOKEN_TYPE_INT: return PRIMITIVES_TYPE_INT_SIZE;
    case TOKEN_TYPE_BYTE: return PRIMITVES_TYPE_BYTE_SIZE;
    default: break;
    }

    new parser::Exception_Handle("Unknow Primitive type not supported");

    return -1;

}



