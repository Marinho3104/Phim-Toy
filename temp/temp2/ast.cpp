#include "./ast.h"

#include "./../utils/linkedList.h"
#include "./ast_helper.h"
#include "./tokenizer.h"
#include "./ast_node.h"

#include <iostream>
#include <string.h>

parser::Declaration_Tracker::~Declaration_Tracker() {
    delete variable_declarations; delete function_declarations; delete struct_declarations;
    delete declaration_ids; delete names;
}

parser::Declaration_Tracker::Declaration_Tracker() {
    variable_declarations = new utils::Linked_List <Ast_Node_Variable_Declaration*>(); variable_declarations->destroy_content = 0;
    function_declarations = new utils::Linked_List <Ast_Node_Function_Declaration*>(); function_declarations->destroy_content = 0;
    struct_declarations = new utils::Linked_List <Ast_Node_Struct_Declaration*>(); struct_declarations->destroy_content = 0;

    declaration_ids = new utils::Linked_List <int>();
    names = new utils::Linked_List <char*>();
}

void parser::Declaration_Tracker::updateOff(int* __off) { off = __off; }

void parser::Declaration_Tracker::addDeclaration(Ast_Node_Variable_Declaration* __variable_declaration) { variable_declarations->add(__variable_declaration); }

parser::Ast_Node_Variable_Declaration* parser::Declaration_Tracker::getVariableDeclaration(int __declaration_id) {

    for (int _ = 0; _ < variable_declarations->count; _++)

        if (variable_declarations->operator[](_)->declaration_id == __declaration_id) return variable_declarations->operator[](_);

    return NULL;

}

void parser::Declaration_Tracker::addDeclaration(Ast_Node_Function_Declaration* __function_declaration) { function_declarations->add(__function_declaration); }

parser::Ast_Node_Function_Declaration* parser::Declaration_Tracker::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    utils::Linked_List <Ast_Node_Variable_Declaration*>* _parameters_variable_declaration = 
        parser_helper::getVariableDeclarationsFromLinkedList(__parameters);

    utils::Linked_List <Ast_Node_Variable_Declaration*>* _choosed_parameters_variable_declaration;
    Ast_Node_Function_Declaration* _choosed_declaration = NULL;

    for (int _ = 0; function_declarations->count; _++) {

        if (function_declarations->operator[](_)->declaration_id != __declaration_id) continue;

        _choosed_declaration = function_declarations->operator[](_);

        // _choosed_parameters_variable_declaration = parser_helper::getVariableDeclarationsFromLinkedList(function_declarations->operator[](_)->parameters);

        // Confirm if are same parameters // length and types

        _choosed_parameters_variable_declaration->destroy_content = 0; delete _choosed_parameters_variable_declaration;

        if (_choosed_declaration) break;

    }

    return _choosed_declaration;

}

void parser::Declaration_Tracker::addDeclaration(Ast_Node_Struct_Declaration* __struct_declaration) { struct_declarations->add(__struct_declaration); }

parser::Ast_Node_Struct_Declaration* parser::Declaration_Tracker::getStructDeclaration(int __declaration_id) {

    for (int _ = 0; _ < struct_declarations->count; _++)

        if (struct_declarations->operator[](_)->declaration_id == __declaration_id) return struct_declarations->operator[](_);

    return NULL;

}

int parser::Declaration_Tracker::addName(char* __name) {

    if (getDeclarationId(__name) != -1) return 0;

    char* _to_add = (char*) malloc(strlen(__name) + 1);

    strcpy(_to_add, __name);

    names->add(_to_add); declaration_ids->add((*off)++);

    return 1;

}

int parser::Declaration_Tracker::getDeclarationId(char* __name) {

    int _pos = __name ? names->getPosition(__name, NULL) : -1;

    return ((_pos != -1) ? (*declaration_ids)[_pos] : -1);

}


parser::Name_Space::~Name_Space() { delete scope; delete declaration_tracker; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __scope) : scope(__scope) { declaration_tracker = new Declaration_Tracker(); }

void parser::Name_Space::printScope() { std::cout << "Scope -> "; scope->printContent(); }

void parser::Name_Space::updateOff(int* __off) { declaration_tracker->updateOff(__off); }


parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces; }

parser::Name_Space_Control::Name_Space_Control() : declaration_off(0) { 
    
    name_spaces = new utils::Linked_List <Name_Space*>();

    utils::Linked_List <char*>* _global_scope = new utils::Linked_List <char*>(); addNameSpace(_global_scope);

}

bool parser::Name_Space_Control::addNameSpace(Name_Space* __to_add) {

    if (getNameSpace(__to_add->scope)) return 0;

    __to_add->updateOff(&declaration_off);

    name_spaces->add(__to_add);

    return 1;

}

bool parser::Name_Space_Control::addNameSpace(utils::Linked_List <char*>* __to_add_scope) {

    if (getNameSpace(__to_add_scope)) return 0;

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_name_space) Name_Space(__to_add_scope);

    _name_space->updateOff(&declaration_off);

    name_spaces->add(_name_space);

    return 1;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::Linked_List <char*>* __scope) {

    for (int _ = 0; _ < name_spaces->count; _++) {

        if (*__scope != name_spaces->operator[](_)->scope) continue;

        name_spaces->operator[](_)->updateOff(&declaration_off);

        return name_spaces->operator[](_);

    }

    return NULL;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(utils::Linked_List <char*>* __scope) { // it dont update declarations off

    parser::Name_Space* _name_space = NULL;

    if (__scope->count)

        do { --__scope->count; _name_space = getNameSpace(__scope); } 
        
        while (__scope->count && !_name_space);

    return _name_space ? _name_space : name_spaces->operator[](0);

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceOrAdd(utils::Linked_List <char*>* __scope) {

    parser::Name_Space* _name_space = getNameSpace(__scope);

    if (_name_space) return _name_space;

    _name_space = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

    utils::Linked_List <char*>* _name_space_scope = new utils::Linked_List <char*>();
    char* _scope_name;

    for (int _ = 0; _ < __scope->count; _++) {

        _scope_name = (char*) malloc(strlen(__scope->operator[](_)) + 1);

        strcpy(_scope_name, __scope->operator[](_));

        _name_space_scope->add(_scope_name);

    }


    new (_name_space) parser::Name_Space(
        _name_space_scope
    );    

    addNameSpace(_name_space);

    return _name_space;

}       


parser::Ast_Control::~Ast_Control() {
    delete name_space_nodes; delete name_space_node_chain; delete code_block_node_chain;

    delete implicit_values;
}

parser::Ast_Control::Ast_Control(Tokenizer_Control* __tokenizer_control, bool __debug_info) 
    : tokenizer_control(__tokenizer_control), debug_info(__debug_info), current_token_position(0) {

        name_space_nodes = new utils::Linked_List <Ast_Node_Name_Space*>();
        implicit_values = new utils::Linked_List <char*>();

        code_block_node_chain = new utils::Linked_List <Ast_Node_Code_Block*>();
        name_space_node_chain = new utils::Linked_List <Ast_Node_Name_Space*>();

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_token_position + __off >= tokenizer_control->tokens_collection->count) 
        __off = tokenizer_control->tokens_collection->count - 1 - current_token_position;

    return tokenizer_control->tokens_collection->operator[](current_token_position + __off); 


}

void parser::Ast_Control::printDebugInfo(const char* __info) { if (debug_info) std::cout << "Ast Control - Debug Info: " << __info << std::endl; }

void parser::Ast_Control::addNameSpaceNodeToChain(Ast_Node_Name_Space* __name_space_node) { name_space_node_chain->add(__name_space_node); } 

void parser::Ast_Control::popNameSpaceNodeOfChain() {
    utils::Data_Linked_List <Ast_Node_Name_Space*>* _poped = name_space_node_chain->remove(name_space_node_chain->count); 
    _poped->destroy_content = 0; delete _poped;
}

int parser::Ast_Control::addImplicitValue(char* __value) {

    int _position = implicit_values->getPosition(__value, NULL);

    if (_position != -1) return _position;

    char* __to_add = (char*) malloc(strlen(__value) + 1);

    strcpy(__to_add, __value);

    return implicit_values->add(__to_add);

}

void parser::Ast_Control::generate() {

    if (debug_info) std::cout << "\n\t\t--- Ast Control ---\n" << std::endl;

    Ast_Node_Name_Space::generate(name_space_control->name_spaces->operator[](0));

}


void parser::setUpAst(Tokenizer_Control* __tokenizer_control, bool __debug_info) {

    ast_control = new Ast_Control(__tokenizer_control, __debug_info);

    name_space_control = new Name_Space_Control();

}

void parser::cleanAst() { delete ast_control; delete name_space_control; }

