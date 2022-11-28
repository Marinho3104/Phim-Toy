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


parser::Type_Information::Type_Information(int __tkId, int __strcDeclId, utils::LinkedList <int>* _pntrOp) 
    : token_id(__tkId), user_defined_declaration_id(__strcDeclId), pointer_level(0), reference_level(0) {

        if (!_pntrOp) return;

        for (int _ = 0; _ < _pntrOp->count; _++)

            if ((*_pntrOp)[_] == TOKEN_POINTER) {

                if (reference_level) new Ast_Exception("Error in type information");

                pointer_level++;

            }        

            else {

                reference_level++;

                if (reference_level > 2) new Ast_Exception("Error in type information");

            }

}

parser::Type_Information* parser::Type_Information::generateDifferentPointersOperations(Ast_Control* __astCntrl) {

    parser::Type_Information* _; 
    utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

    parser_helper::setPointerOperators(__astCntrl, _operators);

    _ = new Type_Information(token_id, user_defined_declaration_id, _operators);

    delete _operators;

    return _;
    
}

parser::Type_Information* parser::Type_Information::generate(parser::Ast_Control* __astCntrl) {

    int _id = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->id, _usrDefDeclId = 0;
    utils::LinkedList <int>* _pntrOprts = new utils::LinkedList <int>();

    // switch (_id) TODO
    // {
    // case TOKEN_IDENTIFIER: 
    //     _usrDefDeclId = __astCntrl->current_code_block->getDeclarationId((*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->phr);
    // default: __astCntrl->current_token_position++;
    // }

    parser_helper::setPointerOperators(__astCntrl, _pntrOprts);

    Type_Information* _ = new Type_Information(_id, _usrDefDeclId, _pntrOprts);

    delete _pntrOprts;

    return _;

}

bool parser::Type_Information::operator==(Type_Information&_) {

    return (
        token_id == _.token_id &&
        pointer_level == _.pointer_level &&
        reference_level == _.reference_level 
    );

}

int parser::Type_Information::getByteSize() {

    if (!pointer_level) {

        if (token_id == TOKEN_IDENTIFIER) { std::cout << "User defined get size not implemented " << std::endl; exit(-1); /* TODO */ }

        else return parser_helper::getSizePrimitiveType(token_id);

    }

    return PRIMITIVES_TYPE_POINTER_SIZE;

}


parser::Names_Tracker::~Names_Tracker() { delete names; }

parser::Names_Tracker::Names_Tracker() { names = new utils::LinkedList <char*>(); }

int parser::Names_Tracker::getDeclarationId(char* __n) { return __n ? names->getObjectPosition(__n, NULL) : -1; }

bool parser::Names_Tracker::addNewName(char* __n, bool __cpy) {
    if (__cpy) __n = utils::copyString(__n, utils::getStringSize(__n));
    if (getDeclarationId(__n) != -1) { if (__cpy) free(__n); return 0; }
    names->add(__n);
    return 1;
}


parser::Name_Space::~Name_Space() { 
    if (declarations) declarations->~Ast_Node_Code_Block(); free(declarations); 
    delete names_tracker; 
    delete name_space_scope; 
    }

parser::Name_Space::Name_Space(utils::LinkedList <char*>* __n, Name_Space_Control* __nmspcCntrl) :  name_space_control(__nmspcCntrl), name_space_scope(__n), declarations(NULL) { 
    if (!name_space_scope) name_space_scope = new utils::LinkedList <char*>();
    names_tracker = new Names_Tracker(); 
}

int parser::Name_Space::getDeclarationId(char* __n) {

    parser::Name_Space* _prevNameSpace;
    int _;

    if ((_ = names_tracker->getDeclarationId(__n)) == -1) {

        _prevNameSpace = name_space_control->getPreviousNameSpace(this);

        return _prevNameSpace ? _prevNameSpace->getDeclarationId(__n) : _;

    }

    return _;

}

void parser::Name_Space::generateCodeBlock(Ast_Control* __astCntrl) {

    if (!declarations)
        declarations = Ast_Node_Code_Block::generate(
            __astCntrl, this, AST_NODE_CODE_BLOCK_ENVIRONMENT_NAME_SPACE, NULL
        );

    else {

        parser::Ast_Node_Code_Block* _prevBlock = __astCntrl->current_code_block;
        
        __astCntrl->current_code_block = declarations;
        
        declarations->generateBody(__astCntrl);

        __astCntrl->current_code_block = _prevBlock;
    }
}


parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces; delete global_name_space; }

parser::Name_Space_Control::Name_Space_Control() { name_spaces = new utils::LinkedList <Name_Space*>(); global_name_space = new Name_Space(NULL, this); }

parser::Name_Space* parser::Name_Space_Control::isNameSpaceDefined(utils::LinkedList <char*>* __scp) {

    parser::Name_Space* _ = NULL;
    bool _rslt;

    for (int __ = 0; __ < name_spaces->count; __++) {

        _rslt = 1;

        if ((*name_spaces)[__]->name_space_scope->count != __scp->count) continue;

        for (int ___ = 0; ___ < __scp->count; ___++)

            if (strcmp(
                (*(*name_spaces)[__]->name_space_scope)[___], (*__scp)[___]
            )) _rslt = 0;

        if (_rslt) { _ = (*name_spaces)[__]; break; }

    }

    return _;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(Name_Space* __nmSpc) {

    utils::LinkedList <char*>* _toCheck = new utils::LinkedList<char*>();
    Name_Space* _ = NULL;

    if (!__nmSpc->name_space_scope->count) goto rtr;

    for (int _ = 0; _ < __nmSpc->name_space_scope->count - 1; _++)

        _toCheck->add((*__nmSpc->name_space_scope)[_]);

    _ = isNameSpaceDefined(_toCheck);

rtr:
    _toCheck->destroy_content = 0;
    delete _toCheck;

    return _;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::LinkedList <char*>* __scp) {

    if (!__scp) return global_name_space;

    utils::LinkedList <char*>* _crrntScope;
    parser::Name_Space* _crrnt;

    for (int _ = 0; _ < __scp->count; _++) {

        _crrntScope = new utils::LinkedList <char*>();

        for (int __  = 0; __ < _; __++) _crrntScope->add(
            utils::copyString(
                (*__scp)[__], utils::getStringSize((*__scp)[__])
            )
        );

        _crrnt = isNameSpaceDefined(_crrntScope);

        if (!_crrnt) {

            _crrnt = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

            new (_crrnt) parser::Name_Space(
                _crrntScope, this
            );

            name_spaces->add(_crrnt);

        } else delete _crrntScope;
        
    }

    return _crrnt;

}


parser::Storage::~Storage() { delete implicit_values; }

parser::Storage::Storage() { implicit_values = new utils::LinkedList <char*>(); }

int parser::Storage::addNewValue(char* __v, bool __cpy) {
    if (__cpy) __v = utils::copyString(__v, utils::getStringSize(__v));

    int _rtr;
    if ((_rtr = implicit_values->getObjectPosition(__v, NULL)) == -1) _rtr = implicit_values->add(__v);
    else free(__v);
    return _rtr; 
}

parser::Ast_Exception::Ast_Exception(char* __if) : description(__if) { std::cout << "Ast Exception: " << description << std::endl; exit(-1); }


parser::Ast_Control::~Ast_Control() { delete name_space_control; delete storage; }

parser::Ast_Control::Ast_Control(utils::LinkedList <parser::Token*>* __tksColl, bool __dbgIf) 
    : tokens_collection(__tksColl), debug_info(__dbgIf), current_code_block(NULL), current_token_position(0) {
            name_space_control = new Name_Space_Control();
            storage = new Storage();
}

void parser::Ast_Control::printDebugInfo(const char* __if) { if (debug_info) std::cout << __if << std::endl; }

parser::Token* parser::Ast_Control::getToken(int __pls) { return (*tokens_collection)[current_token_position + __pls]; }

void parser::Ast_Control::generateAst() {

    parser::Name_Space* _global_name_space = name_space_control->getNameSpace(NULL);

    _global_name_space->generateCodeBlock(this);

}


