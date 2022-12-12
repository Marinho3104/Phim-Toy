#include "./ast.h"

#include "./../utils/linkedList.h"
#include "./tokenizer.h"
#include "./ast_nodes.h"

#include <iostream>
#include <string.h>



parser::Name_Tracker::~Name_Tracker() { delete names; delete declaration_ids; }

parser::Name_Tracker::Name_Tracker() : off(0) { names = new utils::Linked_List <char*>(); declaration_ids = new utils::Linked_List <int>(); }

bool parser::Name_Tracker::addName(char* __name) {

    if (getDeclarationId(__name) != -1) return 0;

    char* _to_add = (char*) malloc(strlen(__name) + 1);

    strcpy(_to_add, __name);

    names->add(_to_add); declaration_ids->add((*off)++);

    return 1;

}

int parser::Name_Tracker::getDeclarationId(char* __name) {

    int _pos = __name ? names->getPosition(__name, NULL) : -1;

    return ((_pos != -1) ? (*declaration_ids)[_pos] : -1);

}


parser::Name_Space::~Name_Space() { delete scope; delete name_tracker; }

parser::Name_Space::Name_Space(Name_Space_Control* __name_space_control, utils::Linked_List <char*>* __scope) 
    : name_space_control(__name_space_control), scope(__scope) { name_tracker = new Name_Tracker(); }

void parser::Name_Space::updateOff(int* __off) { name_tracker->off = __off; }

bool parser::Name_Space::addName(char* __name) { return name_tracker->addName(__name); }

int parser::Name_Space::getDeclarationId(char* __name) {

    int _id = name_tracker->getDeclarationId(__name);

    if (_id == -1 && scope->count) return name_space_control->getPreviousNameSpace(scope)->getDeclarationId(__name);

    return _id;

}

void parser::Name_Space::printScope() { std::cout << "Name Space Scope -> "; scope->printContent(); }


parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces; }

parser::Name_Space_Control::Name_Space_Control() : declarations_off(0) { 
    
    name_spaces = new utils::Linked_List <Name_Space*>();

    utils::Linked_List <char*>* _global_scope = new utils::Linked_List <char*>(); addNameSpace(_global_scope);

}

bool parser::Name_Space_Control::addNameSpace(Name_Space* __to_add) {

    if (getNameSpace(__to_add->scope)) return 0;

    __to_add->updateOff(&declarations_off);

    name_spaces->add(__to_add);

    return 1;

}

bool parser::Name_Space_Control::addNameSpace(utils::Linked_List <char*>* __to_add_scope) {

    if (getNameSpace(__to_add_scope)) return 0;

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_name_space) Name_Space(this, __to_add_scope);

    _name_space->updateOff(&declarations_off);

    name_spaces->add(_name_space);

    return 1;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::Linked_List <char*>* __scope) {

    for (int _ = 0; _ < name_spaces->count; _++) {

        if (*__scope != name_spaces->operator[](_)->scope) continue;

        name_spaces->operator[](_)->updateOff(&declarations_off);

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
        this, _name_space_scope
    );    

    addNameSpace(_name_space);

    return _name_space;

}       




parser::Ast_Control::~Ast_Control() { 
    delete tokenizer_control; delete implicit_values; delete name_space_control; delete name_space_nodes; 

    delete name_space_chain; delete code_block_chain;

}

parser::Ast_Control::Ast_Control(Tokenizer_Control* __tokenizer_control, bool __debug_info) 
    : tokenizer_control(__tokenizer_control), debug_info(__debug_info), current_token_position(0) {
        
        name_space_nodes = new utils::Linked_List <Ast_Node_Name_Space*>();
        implicit_values = new utils::Linked_List <char*>();
        name_space_control = new Name_Space_Control();

        code_block_chain = new utils::Linked_List <Ast_Node_Code_Block*>();
        name_space_chain = new utils::Linked_List <Name_Space*>();

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_token_position + __off >= tokenizer_control->tokens_collection->count) 
        __off = tokenizer_control->tokens_collection->count - 1 - current_token_position;

    return tokenizer_control->tokens_collection->operator[](current_token_position + __off); 

}

parser::Ast_Node_Name_Space* parser::Ast_Control::getNameSpaceNodeFromNameSpace(Name_Space* __name_space) {

    for (int _ = 0; _ < name_space_nodes->count; _++)

        if (name_space_nodes->operator[](_)->name_space == __name_space) return name_space_nodes->operator[](_);

    return NULL;

}

void parser::Ast_Control::printDebugInfo(const char* __debug_info) { if (debug_info) std::cout << "\tAst Control - Debug Info: " << __debug_info << std::endl; }

void parser::Ast_Control::addNameSpace(Name_Space* __name_space) { name_space_chain->add(__name_space); }

void parser::Ast_Control::popNameSpace() { 
    utils::Data_Linked_List <Name_Space*>* _poped = name_space_chain->remove(name_space_chain->count); 
    _poped->destroy_content = 0; delete _poped;
}

void parser::Ast_Control::addCodeBlockNode(Ast_Node_Code_Block* __code_block_node) { code_block_chain->add(__code_block_node); }

void parser::Ast_Control::popCodeBlockNode() {
    utils::Data_Linked_List <Ast_Node_Code_Block*>* _poped = code_block_chain->remove(code_block_chain->count); 
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

    Ast_Node_Name_Space::generate(this, name_space_control->name_spaces->operator[](0));

    if (name_space_chain->count || code_block_chain->count) { std::cout << "Not deleted all environment nodes" << std::endl; exit(1); }

}


