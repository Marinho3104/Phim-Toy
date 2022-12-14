#include "./ast.h"

#include "./../utils/linkedList.h"
#include "./exception_handle.h"
#include "./ast_helper.h"
#include "./ast_nodes.h"
#include "./tokenizer.h"

#include <iostream>
#include <string.h>


parser::Declaration_Tracker::~Declaration_Tracker() { 
    delete variable_declarations; delete function_declarations; delete struct_declarations; delete names; delete declaration_ids; 
}

parser::Declaration_Tracker::Declaration_Tracker() {
    variable_declarations = new utils::Linked_List <Ast_Node_Variable_Declaration*>(); variable_declarations->destroy_content = 0;
    function_declarations = new utils::Linked_List <Ast_Node_Function_Declaration*>(); function_declarations->destroy_content = 0;
    struct_declarations = new utils::Linked_List <Ast_Node_Struct_Declaration*>(); struct_declarations->destroy_content = 0;

    names = new utils::Linked_List <char*>(); declaration_ids = new utils::Linked_List <int>();
}

parser::Ast_Node_Variable_Declaration* parser::Declaration_Tracker::getVariableDeclaration(int __declaration_id) {

    for (int _ = 0; _ < variable_declarations->count; _++)

        if (variable_declarations->operator[](_)->declaration_id == __declaration_id) return variable_declarations->operator[](_);

    return NULL;

}

parser::Ast_Node_Function_Declaration* parser::Declaration_Tracker::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* _given_parameters = 
        parser_helper::getVariableDeclarationOfLinkedList(__parameters), *_found_parameters; 
    _given_parameters->destroy_content = 0;
    parser::Ast_Node_Function_Declaration* _function_declaration = NULL;

    for (int _ = 0; _ < function_declarations->count; _++)

        if (function_declarations->operator[](_)->declaration_id == __declaration_id) {

            _function_declaration = function_declarations->operator[](_);

            _found_parameters = parser_helper::getVariableDeclarationOfLinkedList(function_declarations->operator[](_)->parameters);
            _found_parameters->destroy_content = 0;

            if (_found_parameters->count != _given_parameters->count) { delete _found_parameters; _function_declaration = NULL; continue; }

            for (int _ = 0; _ < _given_parameters->count && _function_declaration; _++)

                if (
                    (*_found_parameters->operator[](_)->type) != _given_parameters->operator[](_)->type
                ) _function_declaration = NULL;

            delete _found_parameters;

            if (_function_declaration) break;

        }

    delete _given_parameters;

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Declaration_Tracker::getStructDeclaration(int __declaration_id) {

    for (int _ = 0; _ < struct_declarations->count; _++)

        if (struct_declarations->operator[](_)->declaration_id == __declaration_id) return struct_declarations->operator[](_);

    return NULL;

}

void parser::Declaration_Tracker::addName(char* __name) {

    if (getDeclarationId(__name) != -1) return;

    char* _name_copy = (char*) malloc(strlen(__name) + 1);

    strcpy(_name_copy, __name);

    names->add(_name_copy); declaration_ids->add((*off)++);

}

int parser::Declaration_Tracker::getDeclarationId(char* __name) {

    int _position = names->getPosition(__name, NULL);

    return _position == -1 ? _position : declaration_ids->operator[](_position);

}


parser::Name_Space::~Name_Space() { delete scope; delete declaration_tracker; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __scope) : scope(__scope) {
    declaration_tracker = new Declaration_Tracker();
}


parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces; }

parser::Name_Space_Control::Name_Space_Control() : declarations_off(0) {
    
    name_spaces = new utils::Linked_List <Name_Space*>();

    addNameSpace(new utils::Linked_List <char*>());
    
}

void parser::Name_Space_Control::addNameSpace(Name_Space* __name_space) {

    if (getNameSpace(__name_space->scope)) return;

    __name_space->declaration_tracker->off = &declarations_off;

    name_spaces->add(__name_space);

}

void parser::Name_Space_Control::addNameSpace(utils::Linked_List <char*>* __scope) {

    if (getNameSpace(__scope)) return;

    parser::Name_Space* _name_space = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

    new (_name_space) parser::Name_Space(
        __scope
    );

    addNameSpace(_name_space);

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::Linked_List <char*>* __scope) {

    for (int _ = 0; _ < name_spaces->count; _++)

        if ((*name_spaces->operator[](_)->scope) == __scope) return name_spaces->operator[](_);

    return NULL;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceOrAdd(utils::Linked_List <char*>* __scope) {

    parser::Name_Space* _name_space = getNameSpace(__scope);

    if (_name_space) return _name_space;

    utils::Linked_List <char*>* _scope_copy = new utils::Linked_List <char*>();

    for (int _ = 0; _ < __scope->count; _++) {

        _scope_copy->add(NULL);

        utils::Data_Linked_List <char*>* _data_linked_list = _scope_copy->getDataLinkedList(_);

        _data_linked_list->object = (char*) malloc(strlen(__scope->operator[](_)) + 1);

        strcpy(_data_linked_list->object, __scope->operator[](_));

    }

    addNameSpace(_scope_copy);

    return name_spaces->last->object;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(Name_Space* __name_space) {

    int _backup = __name_space->scope->count;
    parser::Name_Space* _name_space = NULL;

    if (_name_space->scope->count) 

        do __name_space->scope->count--;

        while (!(_name_space = getNameSpace(__name_space->scope)));

    __name_space->scope->count = _backup;

    return _name_space;

}


parser::Ast_Control::~Ast_Control() {
    delete name_space_nodes; 
    delete implicit_values; 
    delete code_block_chain; 
    delete name_space_chain;
}

parser::Ast_Control::Ast_Control(Tokenizer_Control* __tokenizer_control, bool __debug_mode) 
    : tokenizer_control(__tokenizer_control), debug_mode(__debug_mode), current_token_position(0) {

        name_space_nodes = new utils::Linked_List <Ast_Node_Name_Space*>();
        implicit_values = new utils::Linked_List <char*>();

        code_block_chain = new utils::Linked_List <Ast_Node_Code_Block*>();
        name_space_chain = new utils::Linked_List <Ast_Node_Name_Space*>();

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_token_position + __off >= tokenizer_control->tokens_collection->count) 
        __off = tokenizer_control->tokens_collection->count - 1 - current_token_position;

    return tokenizer_control->tokens_collection->operator[](current_token_position + __off); 

}

void parser::Ast_Control::printDebugInfo(const char* __info) { if (debug_mode) std::cout << "Ast Control - Debug Info: " << __info << std::endl; }

void parser::Ast_Control::addNameSpaceNode(Ast_Node_Name_Space* __name_space_node) {

    Ast_Node_Name_Space* _name_space_in_linked_list = getNameSpaceNodeFromNameSpace(__name_space_node->name_space); 

    if (!_name_space_in_linked_list) { name_space_nodes->add(__name_space_node); return; }

    _name_space_in_linked_list->declarations->join(
        __name_space_node->declarations
    );

    __name_space_node->declarations->destroy_content = 0;

    __name_space_node->~Ast_Node_Name_Space(); free(__name_space_node);

}

parser::Ast_Node_Name_Space* parser::Ast_Control::getNameSpaceNodeFromNameSpace(Name_Space* __name_space) {

    for (int _ = 0; _ < name_space_nodes->count; _++)

        if (name_space_nodes->operator[](_)->name_space == __name_space) return name_space_nodes->operator[](_);

    return NULL;

}

void parser::Ast_Control::addNameSpaceToChain(Name_Space* __name_space) {

    Ast_Node_Name_Space* _name_space_node = getNameSpaceNodeFromNameSpace(__name_space);

    if (!__name_space) new Exception_Handle("Error onde nao devia ser fuckkk");

    addNameSpaceToChain(_name_space_node);

}

void parser::Ast_Control::addNameSpaceToChain(Ast_Node_Name_Space* __name_space_node) { name_space_chain->add(__name_space_node); }

void parser::Ast_Control::popNameSpaceFromChain() {
    utils::Data_Linked_List <Ast_Node_Name_Space*>* _poped = name_space_chain->remove(name_space_chain->count); 
    _poped->destroy_content = 0; delete _poped;
}

void parser::Ast_Control::addCodeBlockToChain(Ast_Node_Code_Block* __code_block_node) { code_block_chain->add(__code_block_node); }

void parser::Ast_Control::popCodeBlockFromChain() {
    utils::Data_Linked_List <Ast_Node_Code_Block*>* _poped = code_block_chain->remove(code_block_chain->count); 
    _poped->destroy_content = 0; delete _poped;
}

int parser::Ast_Control::addImplicitValue(char* __value) {

    int _position;

    if ((_position = implicit_values->getPosition(__value, NULL)) != -1) return _position;

    char* _value_copy = (char*) malloc(strlen(__value) + 1);

    strcpy(_value_copy, __value);

    return implicit_values->add(_value_copy);

}

void parser::Ast_Control::generate() { 

    if (debug_mode) std::cout << "\n\t\t--- Ast Control ---\n" << std::endl;

    Ast_Node_Name_Space::generate(name_space_control->name_spaces->operator[](0));
    
}


void parser::setupAst(Tokenizer_Control* __tokenizer_control, bool __debug_mode) {
    ast_control = new Ast_Control(__tokenizer_control, __debug_mode);
    name_space_control = new Name_Space_Control();
}

void parser::cleanAst() {
    delete ast_control; delete name_space_control;
}
