#include "./ast.h"

#include "./../utils/linkedList.h" // Linked List
#include "./ast_nodes.h" // Ast nodes
#include "./token.h" // Token | Token Ids
#include "./parser_definitions.h" // Parser definitions
#include "./parser_helper.h" // getSizePrimitiveType()
#include "./ast_helper.h"
#include "./ast.h"
#include "./../utils/commonFunctions.h"

#include <iostream>

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

    int _id = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->id, _usrDefDeclId = 0;
    utils::LinkedList <int>* _pntrOprts = new utils::LinkedList <int>();
    bool _gbl;

    switch (_id)
    {
    case TOKEN_IDENTIFIER: 
        _usrDefDeclId = __astCntrl->current_block->getDeclarationId((*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->phr, _gbl);
    default: __astCntrl->current_token_position++;
    }

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

parser::Storage::~Storage() {}

parser::Storage::Storage() { implicit_values = new utils::LinkedList <char*>(); }

int parser::Storage::addNewValue(char* __v, bool __cpy) {
    if (__cpy) __v = utils::copyString(__v, utils::getStringSize(__v));

    int _rtr;
    if ((_rtr = implicit_values->getObjectPosition(__v, NULL)) == -1) _rtr = implicit_values->add(__v);
    else free(__v);
    return _rtr; 
}

parser::Ast_Exception::Ast_Exception(const char* __dsc) : description(__dsc) {
    std::cout << "Ast generation error: " << description << std::endl; exit(-1); // TODO
}

parser::Ast_Control::~Ast_Control() {
    delete code_blocks;
}

parser::Ast_Control::Ast_Control(utils::LinkedList <parser::Token*>* __tksColl, bool __dbgInfo) 
    : tokens_collection(__tksColl), debug_info(__dbgInfo), current_block(NULL), current_token_position(0) {
        code_blocks = new utils::LinkedList <Ast_Node_Code_Block*>();
        storage = new Storage();
    }

void parser::Ast_Control::printDebugInfo(const char* __i) {

    if (debug_info) std::cout << __i << std::endl;

}

void parser::Ast_Control::generateAst() {

    parser::Ast_Node_Code_Block::generate(
        this, AST_NODE_CODE_BLOCK_ENVIRONMENT_GLOBAL, NULL
    );

}



