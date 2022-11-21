#include "./ast.h"

#include "./ast_helper.h" // Ast Helper setPntrRfrnLevel()
#include "./../utils/linkedList.h" // Linked List
#include "./ast_nodes.h" // Nodes Representation
#include "./token.h" // Tokens Id

#include <iostream>


/*      Type Information     */

parser::TypeInformation::TypeInformation(bool _userDefined, int _typeId, int _pntrLvl, int _rfrnLvl) 
    : userDefined(_userDefined), typeId(_typeId), pntrLvl(_pntrLvl), rfrnLvl(_rfrnLvl) {}

bool parser::TypeInformation::operator==(TypeInformation& _) {

    return (
        userDefined == _.userDefined &&
        typeId == _.typeId &&
        pntrLvl == _.pntrLvl &&
        rfrnLvl == _.rfrnLvl 
    );

}

parser::TypeInformation* parser::TypeInformation::generate(Ast_Control* _astCntrl) { // Reference not supported yet TODO

    int _pntrLvl = 0, _rfrnLvl = 0, _tkId = (*_astCntrl->tokensColl)[_astCntrl->crrntTk]->id;

    bool _userDefined = !parser::isPrimativeType((*_astCntrl->tokensColl)[_astCntrl->crrntTk++]);

    parser_helper::setPntrRfrnLevel(_astCntrl, &_pntrLvl, &_rfrnLvl);

    TypeInformation* _ = (TypeInformation*) malloc(sizeof(TypeInformation));
    new (_) TypeInformation(_userDefined, _tkId, _pntrLvl, _rfrnLvl);

    return _;

}

/*      Storage     */

parser::Storage::Storage() { values = new utils::LinkedList <char*>(); types = new utils::LinkedList <TypeInformation*>(); }

int parser::Storage::addNewValue(char* _) {
    int _rtr;
    if ((_rtr = values->getObjectPosition(_, NULL)) == -1) 
        _rtr = values->add(_);
    return _rtr; 
}

int parser::Storage::addNewType(parser::TypeInformation* _) {
    int _rtr;
    if ((_rtr = types->getObjectPosition(_, [](TypeInformation* _f, TypeInformation* _s) -> bool { return *_f == *_s; })) == -1) 
        _rtr = types->add(_);
    return _rtr;    
}


/*      Ast Control     */

parser::Ast_Control::Ast_Control(utils::LinkedList <parser::Token*>* _tksColl) : tokensColl(_tksColl), crrntTk(0), crrntBlock(NULL) {
    blockCodes = new utils::LinkedList <parser::Ast_Node*>();
    storage = new parser::Storage();
}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Control::getNewNodes(bool _) {

    utils::LinkedList <Ast_Node*>* _rtr = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>));
    new (_rtr) utils::LinkedList <Ast_Node*>();

    std::cout << "Get node ->" << (*tokensColl)[crrntTk]->id << std::endl;

    switch ((*tokensColl)[crrntTk]->id)
    {

    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: break;

    case TOKEN_ENDINSTRUCTION: crrntTk++; free(_rtr); _rtr = getNewNodes(0); break;

    case TOKEN_OPENCURLYBRACKET:

        crrntTk++;
        
        _rtr->add(
            Ast_Node_Block_Code::generate(
                this, AST_NODE_BLOCK_CODE_ENVIRONMENT_BLOCK_CODE
            )
        );

        break;

    case TOKEN_POINTER: case TOKEN_ADDRESS: 
        if(!_) goto cont; 
        _rtr->add(Ast_Node_Pointer_Operators::generate(this));
        break;
    
    default: goto cont;

    }

    return _rtr; 

cont:
    parser::Token* _tk = (*tokensColl)[crrntTk];
    parser::Token* _tkNext = (*tokensColl)[crrntTk + 1];

    if (parser::isPrimativeType(_tk)) { // Miss struct declarations

        TypeInformation* _typeInformation = TypeInformation::generate(this);

        _rtr->join(
            Ast_Node_Variable_Declaration::generate(this, _typeInformation)
        );

    }

    else if (_) {

        if (parser::isAssignment(_tk) || parser::isAssignment(_tkNext))

            _rtr->add(
                Ast_Node_Variable_Assignment::generate(
                    this
                )
            );

        else if (_tk->id == TOKEN_IDENTIFIER) _rtr->add(Ast_Node_Variable::generate(this));

        else if (parser::isImplicitValue(_tk)) _rtr->add(Ast_Node_Value::generate(this));

    }

    else _rtr->add(Ast_Node_Expression::generate(this));

    return _rtr;

}

void parser::Ast_Control::generateAst() {

    blockCodes->add(
        parser::Ast_Node_Block_Code::generate(
            this, AST_NODE_BLOCK_CODE_ENVIRONMENT_BLOCK_CODE
        )
    );

}






