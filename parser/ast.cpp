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

parser::Name_Tracker::~Name_Tracker() { delete names_declared; delete names_declarations_id; }

parser::Name_Tracker::Name_Tracker(Ast_Control* __astCntrl) : ast_control(__astCntrl), count(0)
    { names_declared = new utils::LinkedList <char*>(); names_declarations_id = new utils::LinkedList <int>(); }

int parser::Name_Tracker::getDeclarationId(char* __n) { 
    
    int _pos = __n ? names_declared->getObjectPosition(__n, NULL) : -1; 

    return _pos != -1 ? (*names_declarations_id)[_pos] : _pos;
    
}

bool parser::Name_Tracker::addNewName(char* __n, bool __cpy) {
    if (__cpy) __n = utils::copyString(__n, utils::getStringSize(__n));
    if (getDeclarationId(__n) != -1) { if (__cpy) free(__n); return 0; }
    names_declared->add(__n);
    names_declarations_id->add(ast_control ? ast_control->count++ : count++);
    return 1;
}

int parser::Name_Tracker::addToCorrectNameTracker(Ast_Control* __astCntrl) {

    int _declId;

    if (__astCntrl->current_code_block) {

        __astCntrl->current_code_block->name_tracker->addNewName(
            __astCntrl->getToken(0)->phr, 1
        );

        _declId = __astCntrl->current_code_block->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

    }

    else if (__astCntrl->struct_name_space) {

        __astCntrl->struct_name_space->addNewName(
            __astCntrl->getToken(0)->phr
        );

        _declId = __astCntrl->struct_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

    }

    else {

        __astCntrl->current_name_space->addNewName(
            __astCntrl->getToken(0)->phr
        );

        _declId = __astCntrl->current_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

    }

    return _declId;

}

int parser::Name_Tracker::getDeclarationId(Ast_Control* __astCntrl, char* __n) {

    int _declId;

    if (__astCntrl->current_code_block) {
        _declId = __astCntrl->current_code_block->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );
    
        // if (_declId == -1) goto current_name_space_constrain;
    }
    else if (__astCntrl->struct_name_space)  {

        _declId = __astCntrl->struct_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

        if (_declId == -1) goto current_name_space_constrain;

    }

    else {

    current_name_space_constrain:
        _declId = __astCntrl->current_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );
    }

    return _declId;

}

int parser::Name_Tracker::isDeclarationIdGlobal(Ast_Control* __astCntrl, char* __n) {

    bool _isGbl = 1;

    if (__astCntrl->current_code_block) 
        _isGbl = __astCntrl->current_code_block->isDeclarationIdGlobal(
            __astCntrl->getToken(0)->phr
        );

    return _isGbl;

}


parser::Type_Information::~Type_Information() {}

parser::Type_Information::Type_Information() {}

parser::Type_Information::Type_Information(Ast_Control* __astCntrl, int __tkId, int __usrDeclId, utils::LinkedList <int>* __oprs, Name_Space* _nmSpc) : ast_control(__astCntrl), token_id(__tkId), user_defined_declaration_id(__usrDeclId), pointer_level(0), reference_level(0), name_space(_nmSpc) {

    if (!__oprs) return;

    for (int _ = 0; _ < __oprs->count; _++)

        if ((*__oprs)[_] == TOKEN_POINTER) {

            if (reference_level) new Ast_Execption("Pointer of reference not allowed");

            pointer_level++;

        }

        else {

            reference_level++;

            if (reference_level > 2) new Ast_Execption("Reference of double reference not allowed");

        }

}

parser::Type_Information* parser::Type_Information::generate(Ast_Control* __astCntrl, Name_Space* _nmSpc) {

    utils::LinkedList <int>* _pntrOprts = new utils::LinkedList <int>();
    int _id = __astCntrl->getToken(0)->id, _usrDeclId = -1;

    if (_id == TOKEN_IDENTIFIER) {

        if (_nmSpc) _usrDeclId = _nmSpc->getDeclarationId(__astCntrl->getToken(0)->phr);

        else _usrDeclId = Name_Tracker::getDeclarationId(__astCntrl, __astCntrl->getToken(0)->phr);

        if (_usrDeclId == -1) new Ast_Execption("Unknow type for declaration");

    }

    __astCntrl->current_token_position++;

    parser_helper::setPointerOperators(__astCntrl, _pntrOprts, _id == TOKEN_IDENTIFIER);

    Type_Information* _ = new Type_Information(__astCntrl, _id, _usrDeclId, _pntrOprts, _nmSpc);

    // std::cout << "Variable type user defined id -> " << _usrDeclId << std::endl;

    delete _pntrOprts;

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

parser::Ast_Node_Struct_Declaration* parser::Type_Information::getStructDeclaration() {
    
    if (token_id != TOKEN_IDENTIFIER) return NULL;

    Ast_Node_Name_Space* _astNameSpace = NULL;
    for (int _ = 0;_ < ast_control->name_spaces->count;_++)

        if ((*ast_control->name_spaces)[_]->name_space == name_space) _astNameSpace = (*ast_control->name_spaces)[_];

    if (!_astNameSpace) new Ast_Execption("Name space not found - Type Information");

    for (int _ = 0; _ < _astNameSpace->declarations->count; _++)

        if (
            (*_astNameSpace->declarations)[_]->node_id == AST_NODE_STRUCT_DECLARATION &&
            ((Ast_Node_Struct_Declaration*)(*_astNameSpace->declarations)[_])->declaration_id == user_defined_declaration_id
        ) return ((Ast_Node_Struct_Declaration*)(*_astNameSpace->declarations)[_]);

    return NULL;

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

            Ast_Node_Struct_Declaration* _struct_declaration = getStructDeclaration();

            if (_struct_declaration) return _struct_declaration->getByteSize();

            new Ast_Execption("Error no declaration with given id in given name space - Type Information");

        }

        else return parser_helper::getSizePrimitiveType(token_id);

    }

    return PRIMITIVES_TYPE_POINTER_SIZE;
}


parser::Name_Space::~Name_Space() { delete name_space_scope; delete name_tracker; }

parser::Name_Space::Name_Space(Ast_Control* __astCntrl, Name_Space_Control* _cntrl, utils::LinkedList <char*>* __scp) 
    : name_space_control(_cntrl), name_space_scope(__scp), off_count(0) {
        if (!name_space_scope) name_space_scope = new utils::LinkedList <char*>();
        name_tracker = new Name_Tracker(__astCntrl);
    }

bool parser::Name_Space::addNewName(char* __n) { return name_tracker->addNewName(__n, 1); }

int parser::Name_Space::getDeclarationId(char* __n) {

    parser::Name_Space* _prevNameSpace;
    int _;

    if (
        (_ = name_tracker->getDeclarationId(__n)) == -1
    ) {

        _prevNameSpace = name_space_control->getPreviousNameSpace(this);

        return _prevNameSpace ? _prevNameSpace->getDeclarationId(__n) : _;

    }

    return _ + off_count;

}

parser::Name_Space* parser::Name_Space::checkIfNameSpace(Ast_Control* __astCntrl, int* __strctDecl) {

    Name_Space* _nameSpace = NULL;

    if (__astCntrl->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR || __astCntrl->getToken(1)->id == TOKEN_NAMESPACE_OPERATOR) {

        // if (__astCntrl->getToken(1)->id == TOKEN_NAMESPACE_OPERATOR && __astCntrl->getToken(0)->id != TOKEN_IDENTIFIER) return _nameSpace;

        _nameSpace = Name_Space::getNameSpace(
            __astCntrl, __astCntrl->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR, __strctDecl
        );

    }

    return _nameSpace;

}

parser::Name_Space* parser::Name_Space::getNameSpaceFromStruct(Ast_Control* __astCntrl, utils::LinkedList <char*>* __scope, int* __strctDecl) { // TODOD not correct

    // Need to remove last 2 
    utils::DataLL <char*>* _structDecl = __scope->last;
    __scope->remove(_structDecl);

    Name_Space* _name_space = __astCntrl->name_space_control->getNameSpaceDefinition(
        __scope
    );

    if (!_name_space) return NULL;

    _name_space->print();

    int _strctDeclId = _name_space->getDeclarationId(_structDecl->object);

    if (_strctDeclId == -1) return NULL;
    
    if (__strctDecl) *__strctDecl = _strctDeclId;

    delete _structDecl;

    return parser_helper::getNameSpaceOfStructByDeclarationId(__astCntrl, _name_space, _strctDeclId);

}

parser::Name_Space* parser::Name_Space::getNameSpace(Ast_Control* __astCntrl, bool __glb, int* __strctDecl) {

    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();

    if (!__glb)

        for (int _ = 0; _ < __astCntrl->current_name_space->name_space_scope->count; _++)

            _scope->add(
                utils::copyString(
                    (*__astCntrl->current_name_space->name_space_scope)[_],
                    utils::getStringSize((*__astCntrl->current_name_space->name_space_scope)[_])
                )
            );

    else __astCntrl->current_token_position++;

    while (
        __astCntrl->getToken(0)->id == TOKEN_IDENTIFIER && __astCntrl->getToken(1)->id == TOKEN_NAMESPACE_OPERATOR
    ) {

        _scope->add(
            utils::copyString(
                __astCntrl->getToken(0)->phr,
                utils::getStringSize(__astCntrl->getToken(0)->phr)
            )
        );

        __astCntrl->current_token_position += 2;

    }

    Name_Space* _ = __astCntrl->name_space_control->getNameSpaceDefinition(
        _scope
    );

    if (!_ && !(_ = Name_Space::getNameSpaceFromStruct(__astCntrl, _scope, __strctDecl))) new Ast_Execption("Name space not defined");

    delete _scope;

    return _;

}

parser::Name_Space* parser::Name_Space::getNameSpace(Ast_Control* __astCntrl) {

    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();

    for (int _ = 0; _ < __astCntrl->current_name_space->name_space_scope->count; _++) {

        _scope->add(
            utils::copyString(
                (*__astCntrl->current_name_space->name_space_scope)[_],
                utils::getStringSize((*__astCntrl->current_name_space->name_space_scope)[_])
            )
        );

    }
    
    while (
        __astCntrl->getToken(0)->id != TOKEN_OPENCURLYBRACKET
    ) {

        if (__astCntrl->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR) 

            _scope->add(
                utils::copyString(
                    __astCntrl->getToken(0)->phr,
                    utils::getStringSize(__astCntrl->getToken(0)->phr)
                )
            );

        __astCntrl->current_token_position ++;

    }
    
    Name_Space* _ = __astCntrl->name_space_control->getNameSpace(
        _scope
    );

    delete _scope;

    return _;

}

void parser::Name_Space::print() {

    std::cout << "Name space scope -> ";

    for (int _ = 0; _ < name_space_scope->count; _++)

        std::cout << (*name_space_scope)[_] << "::";

    std::cout << std::endl;
 
}



parser::Name_Space_Control::~Name_Space_Control() { delete name_space_declarations; delete global_name_space; }

parser::Name_Space_Control::Name_Space_Control(Ast_Control* __astCntrl) : ast_control(__astCntrl) {
    name_space_declarations = new utils::LinkedList <Name_Space*>();
    global_name_space = new Name_Space(__astCntrl, this, NULL);
}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceDefinition(utils::LinkedList <char*>* __scp) {

    parser::Name_Space* _name_space = NULL;
    bool _rslt;

    if (!__scp || !__scp->count) return global_name_space;

    for (int _ = 0; _ < name_space_declarations->count; _++) {

        _rslt = 1;

        if (
            (*name_space_declarations)[_]->name_space_scope->count != __scp->count
        ) continue;

        for (int __ = 0; __ < __scp->count && _rslt; __++)

            if (
                strcmp((*(*name_space_declarations)[_]->name_space_scope)[__], (*__scp)[__])
            ) _rslt = 0;

        if (_rslt) {  _name_space = (*name_space_declarations)[_]; break; }

    }

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(Name_Space* __nmSpc) {

    utils::LinkedList <char*>* _prevScope = new utils::LinkedList <char*>();
    Name_Space* _name_space = NULL;

    if (!__nmSpc->name_space_scope->count) { delete _prevScope; return _name_space; }

    for (int _ = 0; _ < __nmSpc->name_space_scope->count - 1; _++) _prevScope->add((*__nmSpc->name_space_scope)[_]);
    
    _name_space = getNameSpaceDefinition(_prevScope);

    _prevScope->destroy_content = 0; delete _prevScope;

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::LinkedList <char*>* __scp) {

    utils::LinkedList <char*>* _crrntScope;
    parser::Name_Space* _crrntNameSpace = getNameSpaceDefinition(__scp);

    if (_crrntNameSpace) return _crrntNameSpace;

    for (int _ = 1; _ < __scp->count + 1; _++) {

        _crrntScope = new utils::LinkedList <char*>();

        for (int __  = 0; __ < _; __++) _crrntScope->add(
            utils::copyString(
                (*__scp)[__], utils::getStringSize((*__scp)[__])
            )
        );

        _crrntNameSpace = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

        new (_crrntNameSpace) parser::Name_Space(
            ast_control, this, _crrntScope
        );

        name_space_declarations->add(_crrntNameSpace);

    }

    return _crrntNameSpace;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceStruct() {

    parser::Name_Space* _name_space_struct = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

    new (_name_space_struct) Name_Space(ast_control, this, NULL);

    return _name_space_struct;

}


parser::Storage::~Storage() {}

parser::Storage::Storage() { implicit_values = new utils::LinkedList <char*>(); }

int parser::Storage::addNewValue(char* __v, bool __cpy) {
    if (__cpy) __v = utils::copyString(__v, utils::getStringSize(__v));
    int _rtr;
    if ((_rtr = implicit_values->getObjectPosition(__v, NULL)) == -1) _rtr = implicit_values->add(__v);
    else free(__v);
    return _rtr; 
}


parser::Ast_Execption::Ast_Execption(const char* __dscr) : description(__dscr) { std::cout << description << std::endl; exit(1); }


parser::Ast_Control::~Ast_Control() { delete name_space_control; delete name_spaces; delete storage; }

parser::Ast_Control::Ast_Control(utils::LinkedList <Token*>* __tksColl, bool __dbg) 
    : tokens_collection(__tksColl), debug_info(__dbg), current_token_position(0), current_name_space(NULL), current_code_block(NULL), struct_name_space(NULL), count(0) {
        name_spaces = new utils::LinkedList <Ast_Node_Name_Space*>();
        name_space_control = new Name_Space_Control(this);
        storage = new Storage();
    }

void parser::Ast_Control::printDebugInfo(const char* __i) {

    if (debug_info) std::cout << "Ast Control - Debug Info: " << __i << std::endl;

}

parser::Token* parser::Ast_Control::getToken(int __off) { 
    return (*tokens_collection)[current_token_position + __off < tokens_collection->count ? current_token_position + __off : current_token_position];
}

void parser::Ast_Control::saveState() { current_code_block_saved = current_code_block; current_name_space_saved = current_name_space; struct_name_space_saved = struct_name_space; }

void parser::Ast_Control::setPreviousSavedState() { current_code_block = current_code_block_saved; current_name_space = current_name_space_saved; struct_name_space = struct_name_space_saved; }

void parser::Ast_Control::generate() { Ast_Node_Name_Space::generate(this, name_space_control->getNameSpace(NULL)); }



