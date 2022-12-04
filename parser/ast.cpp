#include "./ast.h"

#include "./../utils/commonFunctions.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./parser_helper.h"
#include "./ast_helper.h"
#include "./ast_nodes.h"
#include "./token.h"

#include <iostream>
#include <string.h>

parser::Type_Information::~Type_Information() {}

parser::Type_Information::Type_Information() {}

parser::Type_Information::Type_Information(Ast_Control* __astCntrl, int __tkId, int __usrDeclId, utils::LinkedList <int>* __oprs, Name_Space* _nmSpc) : ast_control(__astCntrl), token_id(__tkId), user_defined_declaration_id(__usrDeclId), pointer_level(0), reference_level(0), name_space(_nmSpc) {

    if (!__oprs) return;

    for (int _ = 0; _ < __oprs->count; _++)

        if ((*__oprs)[_] == TOKEN_POINTER) {

            if (reference_level) new Ast_Exception("Pointer of reference not allowed");

            pointer_level++;

        }

        else {

            reference_level++;

            if (reference_level > 2) new Ast_Exception("Reference of double reference not allowed");

        }

}

parser::Type_Information* parser::Type_Information::generate(Ast_Control* __astCntrl, Name_Space* _nmSpc) {

    utils::LinkedList <int>* _pntrOprts = new utils::LinkedList <int>();
    int _id = __astCntrl->getToken(0)->id, _usrDeclId = -1;

    if (_nmSpc) { __astCntrl->saveState(); __astCntrl->current_name_space = _nmSpc; }

    std::cout << "Name space -> " << _nmSpc << std::endl;
    _nmSpc->printScope();

    if (_id == TOKEN_IDENTIFIER) {

        _usrDeclId = parser_helper::getDeclarationId(__astCntrl);

        std::cout << "Type user defined with id -> " << _usrDeclId << std::endl;

        if (_usrDeclId == -1) new Ast_Exception("Unknow type for declaration");

    }

    __astCntrl->current_token_position++;

    parser_helper::setPointerOperators(__astCntrl, _pntrOprts, _id == TOKEN_IDENTIFIER);

    Type_Information* _ = new Type_Information(__astCntrl, _id, _usrDeclId, _pntrOprts, _nmSpc);

    // std::cout << "Variable type user defined id -> " << _usrDeclId << std::endl;

    delete _pntrOprts;

    if (_nmSpc) __astCntrl->setLastSavedState();

    return _;

}

parser::Type_Information* parser::Type_Information::generate(Ast_Control* __astCntrl, Type_Information* __tpIf) {

    parser::Type_Information* _; 
    utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

    parser_helper::setPointerOperators(__astCntrl, _operators, __tpIf->token_id == TOKEN_IDENTIFIER);

    _ = new Type_Information(__astCntrl, __tpIf->token_id, __tpIf->user_defined_declaration_id, _operators, __tpIf->name_space);

    delete _operators;

    return _;

}

bool parser::Type_Information::operator==(Type_Information& _) {

    return (
        token_id == _.token_id &&
        user_defined_declaration_id == _.user_defined_declaration_id &&
        pointer_level == _.pointer_level &&
        reference_level == _.reference_level 
    );
}

int parser::Type_Information::getByteSize() {
    
    if (!pointer_level) {

        if (token_id == TOKEN_IDENTIFIER) {

            
            new Ast_Exception("Not done - Type Information");

        }

        else return parser_helper::getSizePrimitiveType(token_id);

    }

    return PRIMITIVES_TYPE_POINTER_SIZE;
}

parser::Type_Information* parser::Type_Information::getCopy() {

    parser::Type_Information* _type_information = new parser::Type_Information(
        ast_control, token_id, user_defined_declaration_id, NULL, name_space
    );

    _type_information->pointer_level = pointer_level;
    _type_information->reference_level = reference_level;

    return _type_information;

}


parser::Name_Tracker::~Name_Tracker() { if (delete_off) free(off); delete names_declared; delete names_declarations_id; }

parser::Name_Tracker::Name_Tracker(int* __off) : off(__off), delete_off(0) {
    names_declarations_id = new utils::LinkedList <int>();
    names_declared = new utils::LinkedList <char*>();
}

int parser::Name_Tracker::getDeclarationId(char* __n) {
    
    int _pos = __n ? names_declared->getObjectPosition(__n, NULL) : -1;

    return ((_pos != -1) ? (*names_declarations_id)[_pos] : -1);

}

bool parser::Name_Tracker::addNewName(char* __n) {

    if (getDeclarationId(__n) != -1) return 0;

    __n = utils::copyString(__n, utils::getStringSize(__n));

    names_declared->add(__n);

    names_declarations_id->add((*off)++);

    return 1;

}


parser::Name_Space::~Name_Space() { delete scope; delete name_tracker; }

parser::Name_Space::Name_Space(Name_Space_Control* __name_space_control, utils::LinkedList <char*>* __scope) 
    : name_space_control(__name_space_control), scope(__scope) { name_tracker = new Name_Tracker(0); }

void parser::Name_Space::updateOff(int* __off) { if (!__off) { __off = (int*) malloc(4); *__off = 0; name_tracker->delete_off = 1; }  name_tracker->off = __off; }

bool parser::Name_Space::addNewName(char* __n) { return name_tracker->addNewName(__n); }

int parser::Name_Space::getDeclarationId(char* __n) {

    Name_Space* _current_name_space = this; int _id;

    _id = 
        name_tracker->getDeclarationId(__n);

    if (_id == -1) {

        _current_name_space = name_space_control->getPreviousNameSpace(scope);

        return _current_name_space ? _current_name_space->getDeclarationId(__n) : _id;

    }

    return _id;

}

void parser::Name_Space::printScope() {

    std::cout << "Name Space scope ->";

    for (int _ = 0; _ < scope->count; _++) std::cout << (*scope)[_] << "::";

    std::cout << std::endl;

}


parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces; }

parser::Name_Space_Control::Name_Space_Control() : declarations_count(0) {
    name_spaces = new utils::LinkedList <Name_Space*>();

    // Global name space
    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();

    parser::Name_Space* _global_name_space = (Name_Space*) malloc(sizeof(Name_Space));
    new (_global_name_space) Name_Space(this, _scope);

    _global_name_space->updateOff(&declarations_count);

    name_spaces->add(_global_name_space);

}

bool parser::Name_Space_Control::addNameSpace(Name_Space* __name_space) {

    if (getNameSpace(__name_space->scope)) return 0;

    __name_space->updateOff(&declarations_count);

    name_spaces->add(__name_space);

    return 1;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceSpecific(utils::LinkedList <char*>* __scope) {

    parser::Name_Space* _name_space = NULL;

    for (int _ = 0; _ < name_spaces->count && !_name_space; _++) {

        if ((*name_spaces)[_]->scope->count != __scope->count) continue;

        _name_space = (*name_spaces)[_];

        for (int __ = 0; __ < __scope->count && _name_space; __++)

            if (strcmp((*_name_space->scope)[__], (*__scope)[__])) _name_space = NULL;


    }

    if (_name_space) _name_space->updateOff(&declarations_count);

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::LinkedList <char*>* __scope) {

    if (!__scope || !__scope->count) return (*name_spaces)[0];

    parser::Name_Space* _name_space = NULL;
    utils::LinkedList <char*>* _current_name_space_scope;

    for (int _ = 0; _ < __scope->count; _++) {

        _current_name_space_scope = new utils::LinkedList <char*>();

        for (int __ = 0; __ < _ + 1; __++)

            _current_name_space_scope->add(
                utils::copyString((*__scope)[__], utils::getStringSize((*__scope)[__]))
            );

        _name_space = getNameSpaceSpecific(_current_name_space_scope);

        delete _current_name_space_scope;

        if (!_name_space) break;

    }

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceOrAdd(utils::LinkedList <char*>* __scope) {

    if (!__scope || !__scope->count) return name_spaces->frst->object;

    parser::Name_Space* _name_space = NULL;
    utils::LinkedList <char*>* _current_name_space_scope;

    for (int _ = 0; _ < __scope->count; _++) {

        _current_name_space_scope = new utils::LinkedList <char*>();

        for (int __ = 0; __ < _ + 1; __++)

            _current_name_space_scope->add(
                utils::copyString((*__scope)[__], utils::getStringSize((*__scope)[__]))
            );

        if ((_name_space = getNameSpaceSpecific(_current_name_space_scope))) continue;

        _name_space = (Name_Space*) malloc(sizeof(Name_Space));
        new (_name_space) Name_Space(this, _current_name_space_scope);

        addNameSpace(_name_space);

    }

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(utils::LinkedList <char*>* __scope) {

    utils::LinkedList <char*>* _previous_scope = new utils::LinkedList <char*>();

    if (!__scope->count) { delete _previous_scope; return NULL; }

    for (int _ = 0; _ < __scope->count - 1; _++) 

        _previous_scope->add(
            utils::copyString((*__scope)[_], utils::getStringSize((*__scope)[_]))
        );

    Name_Space* _previous_name_space = getNameSpace(_previous_scope);

    // delete _previous_scope;

    if (!_previous_name_space) new Ast_Exception("Unexpected error - getPreviousNameSpace()");

    return _previous_name_space;

}


parser::Ast_Exception::Ast_Exception(const char* __descr) : description(__descr) { std::cout << "Ast Exception: " << description << std::endl; exit(1); }


parser::Ast_Control::~Ast_Control() { delete name_space_control; delete name_spaces_saved; delete nodes_name_spaces; delete code_blocks_saved; }

parser::Ast_Control::Ast_Control(utils::LinkedList <Token*>* __tokens_collection, bool __debug_info) 
    : tokens_collection(__tokens_collection), debug_info(__debug_info), current_name_space(NULL), current_code_block(NULL), current_token_position(0) { 
        nodes_name_spaces = new utils::LinkedList <Ast_Node_Name_Space*>();
        name_spaces_saved = new utils::LinkedList <Name_Space*>(); 
        code_blocks_saved = new utils::LinkedList <Ast_Node_Code_Block*>(); 
        name_space_control = new Name_Space_Control(); 
    }

void parser::Ast_Control::saveState() {

    name_spaces_saved->addFrst(current_name_space);
    code_blocks_saved->addFrst(current_code_block);

}

void parser::Ast_Control::setLastSavedState() {

    utils::DataLL <Name_Space*>* _saved_name_space = name_spaces_saved->removeFrst();
    utils::DataLL <Ast_Node_Code_Block*>* _saved_code_block = code_blocks_saved->removeFrst();

    current_name_space = _saved_name_space->object;
    current_code_block = _saved_code_block->object;

    _saved_name_space->destroy_content = 0; delete _saved_name_space;
    _saved_code_block->destroy_content = 0; delete _saved_code_block;

}

void parser::Ast_Control::printDebugInfo(const char* __debug_info) {

    if (debug_info) std::cout << "Ast Control - Debug Info: " << __debug_info << "\n" << std::endl;

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_token_position + __off >= tokens_collection->count) __off = 0;

    return (*tokens_collection)[current_token_position + __off];  

}

void parser::Ast_Control::generate() { Ast_Node_Name_Space::generate(this, name_space_control->getNameSpaceOrAdd(NULL)); }

parser::Ast_Node_Name_Space* parser::Ast_Control::getNameSpaceNodeFromNameSpace(Name_Space* __name_space) {

    for (int _ = 0; _ < nodes_name_spaces->count; _++)

        if ((*nodes_name_spaces)[_]->name_space == __name_space) return (*nodes_name_spaces)[_];

    return NULL;

}




