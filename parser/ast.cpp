#include "./ast.h"

#include "./../utils/linkedList.h" // Linked List
#include "./definitions.h" // Ast Definitions 
#include "./ast_helper.h" // Ast Helper
#include "./ast_nodes.h" // Ast Nodes
#include "./token.h" // Token
#include <iostream>



/*      Type Information        */

parser::Type_Information::Type_Information(int _id, utils::LinkedList <int>* _pntrOprns) 
    : id(_id), pntrOprns(_pntrOprns) {}

bool parser::Type_Information::operator==(parser::Type_Information& _) {

    return ( // Compare Operators TODO
        id == _.id
    );

}

parser::Type_Information* parser::Type_Information::generate(Ast_Control* _astCntrl) {

    int _id = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++]->id;
    utils::LinkedList <int>* _pntrOprts = new utils::LinkedList <int>();

    parser_helper::setPointerOperators(_astCntrl, _pntrOprts);

    Type_Information* _ = (Type_Information*) malloc(sizeof(Type_Information));
    new (_) Type_Information(_id, _pntrOprts);

    return _;

}

/*      Storage        */

parser::Storage::Storage() {
    implicitValues = new utils::LinkedList <char*>(); 
    types = new utils::LinkedList <Type_Information*>();
}

int parser::Storage::addNewValue(char* _) {
    int _rtr;
    if ((_rtr = implicitValues->getObjectPosition(_, NULL)) == -1) 
        _rtr = implicitValues->add(_);
    return _rtr; 
}

int parser::Storage::addNewType(Type_Information* _) {
    int _rtr;
    if ((_rtr = types->getObjectPosition(_, [](Type_Information* _f, Type_Information* _s) -> bool { return *_f == *_s; })) == -1) 
        _rtr = types->add(_);
    return _rtr;
}

/*      Ast Control        */

parser::Ast_Control::Ast_Control(utils::LinkedList <parser::Token*>* _tknsColl) : tokensColl(_tknsColl), crrntTkPos(0), crrntBlock(NULL) {
    code_blocks = new utils::LinkedList <Ast_Node*>();
    storage = new Storage();
}

void parser::Ast_Control::generateAst() {

    // Just add the "global block" than it will start recursing until end of code
    code_blocks->add(
        parser::Ast_Node_Code_Block::generate(
            this, AST_NODE_CODE_BLOCK_ENVIRONMEMT_GLOBAL, NULL
        )
    );

}


