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


parser_helper::Type_Information::~Type_Information() {}

parser_helper::Type_Information::Type_Information(
    parser::Name_Space* __name_space, parser::Ast_Node_Struct_Declaration* __declaration, int __token_id, utils::Linked_List <int>* __pointer_operations) 
        : name_space(__name_space), declaration(__declaration), token_id(__token_id), pointer_level(0), reference_level(0) {

            if (!__pointer_operations) return;

            for (int _ = 0; _ < __pointer_operations->count; _++)

                if (__pointer_operations->operator[](_) == TOKEN_POINTER) {

                    if (reference_level) 
                        new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(_ - parser::ast_control->current_token_position), "Pointer of reference not allowed");

                    pointer_level++;

                }

                else 

                    if (++reference_level > 2) 

                        new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(_ - parser::ast_control->current_token_position), "Reference of double reference not allowed");

}

bool parser_helper::Type_Information::operator==(Type_Information* __to_compare) {

    return (
        declaration == __to_compare->declaration &&
        token_id == __to_compare->token_id &&
        pointer_level == __to_compare->pointer_level &&
        reference_level == __to_compare->reference_level
    );

}

bool parser_helper::Type_Information::operator!=(Type_Information* __to_compare) { return operator!=(__to_compare); }

parser_helper::Type_Information* parser_helper::Type_Information::generate() {

    parser::Name_Space* _name_space = getNameSpace(0);

    parser::Ast_Node_Struct_Declaration* _declaration = NULL;

    if (_name_space) { parser::ast_control->addNameSpaceToChain(_name_space); parser::ast_control->addCodeBlockToChain(NULL); }

    int _id = parser::ast_control->getToken(0)->id;

    if (parser::isPrimitiveTokenId(_id));
    else if (_id == TOKEN_IDENTIFIER) {

        int _declaration_id = parser_helper::getDeclarationId(parser::ast_control->getToken(0)->phr, 0);

        _declaration = parser_helper::getStructDeclaration(_declaration_id, 0);

        if (_declaration_id == -1 || !_declaration) {

            if (_name_space) { parser::ast_control->popNameSpaceFromChain(); parser::ast_control->popCodeBlockFromChain(); }
            new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Undefined type");

        }

    }
    else new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Expected primitive type or user defined type"); 

    parser::ast_control->current_token_position++;

    utils::Linked_List <int>* _pointer_operations = getPointerOperations(_id == TOKEN_IDENTIFIER); // TODO

    Type_Information* _type_information = new Type_Information(_name_space, _declaration, _id, _pointer_operations);

    delete _pointer_operations;

    if (_name_space) { parser::ast_control->popNameSpaceFromChain(); parser::ast_control->popCodeBlockFromChain(); }

    return _type_information;

}

parser_helper::Type_Information* parser_helper::Type_Information::generate(Type_Information* __type_information) {

    utils::Linked_List <int>* _pointer_operations = getPointerOperations(0);

    Type_Information* _type_information = 
        new Type_Information(__type_information->name_space, __type_information->declaration, __type_information->token_id, _pointer_operations);

    delete _pointer_operations;

    return _type_information;


}

int parser_helper::Type_Information::getByteSize() {

    if (!pointer_level)
        return token_id == TOKEN_IDENTIFIER ? declaration->getByteSize() : getPrimitiveTypeSize(token_id);

    return PRIMITIVES_TYPE_POINTER_SIZE;

}


utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* parser_helper::getVariableDeclarationOfLinkedList(utils::Linked_List <parser::Ast_Node*>* __linked_list) {

    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* _variable_declarations = 
        new utils::Linked_List <parser::Ast_Node_Variable_Declaration*>();

    for (int _ = 0; _ < __linked_list->count; _++)

        if (__linked_list->operator[](_)->node_id == AST_NODE_VARIABLE_DECLARATION) 

            _variable_declarations->add(
                (parser::Ast_Node_Variable_Declaration*) __linked_list->operator[](_)
            );

    return _variable_declarations;
    
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

int parser_helper::getNodeType() {

    switch (parser::ast_control->getToken(0)->id)
    {
    case TOKEN_CLOSECURLYBRACKET: return -1; break;
    case TOKEN_OPENCURLYBRACKET: return -2; break;
    case TOKEN_ENDINSTRUCTION: return -3; break;
    case TOKEN_CLOSEPARENTHESES: return -4; break;
    case TOKEN_COMMA: return -5; break;
    case TOKEN_NAMESPACE: return AST_NODE_NAME_SPACE; break;
    case TOKEN_STRUCT: return AST_NODE_STRUCT_DECLARATION; break;
    case TOKEN_POINTER: case TOKEN_ADDRESS: return AST_NODE_POINTER_OPERATOR; break;
    case TOKEN_OPENPARENTHESES: return AST_NODE_PARENTHESIS; break;
    default: break;
    }

    int _backup = parser::ast_control->current_token_position;

    parser::Name_Space* _name_space = getNameSpace(0);

    if (parser::isPrimitiveTokenId(parser::ast_control->getToken(0)->id) || parser::ast_control->getToken(0)->id == TOKEN_IDENTIFIER) {

        parser::ast_control->current_token_position = _backup;

        Type_Information* _type;
        int _node_type;

        try { _type = Type_Information::generate(); }
        catch(...) { 
            _node_type = (parser::ast_control->getToken(1)->id == TOKEN_OPENPARENTHESES) ? AST_NODE_FUNCTION_CALL : AST_NODE_VARIABLE; 
            goto return_reset; 
        }

        getNameSpace(0);

        switch (parser::ast_control->getToken(1)->id)
        {
        case TOKEN_EQUAL: case TOKEN_COMMA: case TOKEN_ENDINSTRUCTION: _node_type = AST_NODE_VARIABLE_DECLARATION; break;
        case TOKEN_OPENPARENTHESES: _node_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(1), "Unexpected token aqui"); break;
        }

        delete _type;

    return_reset:

        parser::ast_control->current_token_position = _backup;

        return _node_type;

    }
    
    else if (parser::isImplicitValue(parser::ast_control->getToken(0)->id)) return AST_NODE_VALUE;

    else if (parser::isExpressionOperator(parser::ast_control->getToken(0)->id)) return AST_NODE_EXPRESSION;

    else if (parser::isAssignmentOperator(parser::ast_control->getToken(0)->id)) return AST_NODE_ASSIGNMENT;

    new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Unexpected token node type");

    return -1;

}

utils::Linked_List <char*>* parser_helper::getScope() {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();

    int _expected = TOKEN_IDENTIFIER;
    char* _copy;

    while(parser::ast_control->getToken(0)->id == _expected) {

        if (_expected == TOKEN_IDENTIFIER) {

            _copy = (char*) malloc(strlen(parser::ast_control->getToken(0)->phr) + 1);

            strcpy(_copy, parser::ast_control->getToken(0)->phr);

            _scope->add(_copy);

        } 

        _expected = _expected == TOKEN_IDENTIFIER ? TOKEN_NAMESPACE_OPERATOR : TOKEN_IDENTIFIER;

        parser::ast_control->current_token_position++;

    }

    if (_expected == TOKEN_IDENTIFIER) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Expected identifier");

    return _scope;

}

parser::Name_Space* parser_helper::getNameSpace(bool _create_name_space) {

    if (parser::ast_control->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR && parser::ast_control->getToken(1)->id != TOKEN_NAMESPACE_OPERATOR && !_create_name_space) return NULL;
    if (parser::ast_control->getToken(0)->id != TOKEN_IDENTIFIER && parser::ast_control->getToken(1)->id != TOKEN_IDENTIFIER && !_create_name_space) return NULL;

    bool _is_global = parser::ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR;

    if (_is_global) parser::ast_control->current_token_position++;

    utils::Linked_List <char*>* _scope = getScope();

    utils::Linked_List <char*>* _current_scope_copy = new utils::Linked_List <char*>();

    if (!_is_global) {

        utils::Linked_List <char*>* _current_scope = parser::ast_control->name_space_chain->last->object->name_space->scope;
        char* _name_copy;

        for (int _ = 0; _ < _current_scope->count; _++) {

            _name_copy = (char*) malloc(strlen(_current_scope->operator[](_)) + 1);

            strcpy(_name_copy, _current_scope->operator[](_));

            _current_scope_copy->add(_name_copy);

        }

        _scope->join(_current_scope_copy, 0);

    }

    if (!_create_name_space) {
        delete _scope->remove(_scope->count);
        parser::ast_control->current_token_position--;
    }

    parser::Name_Space* _name_space = 
        _create_name_space ? parser::name_space_control->getNameSpaceOrAdd(_scope) : parser::name_space_control->getNameSpace(_scope);

    if (!_name_space) new parser::Exception_Handle(parser::ast_control, parser::ast_control->getToken(0), "Undefined name space");
    
    _current_scope_copy->destroy_content = 0; delete _current_scope_copy;

    delete _scope;

    return _name_space;

}

int parser_helper::addName(char* __name) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object) {
     
        parser::ast_control->code_block_chain->last->object->declaration_tracker->addName(__name);

        return parser::ast_control->code_block_chain->last->object->declaration_tracker->getDeclarationId(__name);

    }

    parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->addName(__name);

    return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->getDeclarationId(__name);

}

int parser_helper::getDeclarationId(char* __name, bool __single) {

    if (__single) {
        
        if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
            return parser::ast_control->code_block_chain->last->object->declaration_tracker->getDeclarationId(__name);

        return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->getDeclarationId(__name);
    
    }

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        return parser::ast_control->code_block_chain->last->object->getDeclarationId(__name);

    return parser::ast_control->name_space_chain->last->object->getDeclarationId(__name);


}

bool parser_helper::isGlobalDeclaration(char* __name) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        return parser::ast_control->code_block_chain->last->object->isGlobalDeclaration(__name);

    return 1;

}

parser::Ast_Node_Variable_Declaration* parser_helper::getVariableDeclaration(int __declaration_id, bool __single) {

    if (__single) {
        
        if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
            return parser::ast_control->code_block_chain->last->object->declaration_tracker->getVariableDeclaration(__declaration_id);

        return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->getVariableDeclaration(__declaration_id);
    
    }

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        return parser::ast_control->code_block_chain->last->object->getVariableDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->getVariableDeclaration(__declaration_id);

}

parser::Ast_Node_Function_Declaration* parser_helper::getFunctionDeclaration(int __declaration_id, utils::Linked_List <parser::Ast_Node*>* __parameters, bool __single) {

    if (__single) {
        
        if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
            return parser::ast_control->code_block_chain->last->object->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

        return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);
    
    }

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        return parser::ast_control->code_block_chain->last->object->getFunctionDeclaration(__declaration_id, __parameters);

    return parser::ast_control->name_space_chain->last->object->getFunctionDeclaration(__declaration_id, __parameters);

}

parser::Ast_Node_Struct_Declaration* parser_helper::getStructDeclaration(int __declaration_id, bool __single) {

    if (__single) {
        
        if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
            return parser::ast_control->code_block_chain->last->object->declaration_tracker->getStructDeclaration(__declaration_id);

        return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->getStructDeclaration(__declaration_id);
    
    }

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        return parser::ast_control->code_block_chain->last->object->getStructDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->getStructDeclaration(__declaration_id);

}

void parser_helper::addVariableDeclaration(parser::Ast_Node_Variable_Declaration* __variable_declaration) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        parser::ast_control->code_block_chain->last->object->declaration_tracker->variable_declarations->add(__variable_declaration);

    else parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->variable_declarations->add(__variable_declaration);

}

void parser_helper::addFunctionDeclaration(parser::Ast_Node_Function_Declaration* __function_declaration) {


    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        parser::ast_control->code_block_chain->last->object->declaration_tracker->function_declarations->add(__function_declaration);
        
    else parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->function_declarations->add(__function_declaration);

}

void parser_helper::addStructDeclaration(parser::Ast_Node_Struct_Declaration* __struct_declaration) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)
        parser::ast_control->code_block_chain->last->object->declaration_tracker->struct_declarations->add(__struct_declaration);

    else parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->struct_declarations->add(__struct_declaration);

}

int parser_helper::getPrimitiveTypeSize(int __token_id) {

    switch (__token_id)
    {
    case TOKEN_TYPE_BYTE: return PRIMITVES_TYPE_BYTE_SIZE; break;
    case TOKEN_TYPE_INT: return PRIMITIVES_TYPE_INT_SIZE; break;
    default: break;
    }

    new parser::Exception_Handle("Primitive Type not defined");

}


