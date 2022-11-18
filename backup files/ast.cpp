#include "./ast.h"

#include "./../utils/linkedList.h"
#include "./astHelper.h"
#include "./astNodes.h"
#include "./token.h"

#include "./../utils/linkedList.h"

#include <iostream>

parser::TypeInformation::TypeInformation(bool _userDefinedType, int _typePos, int _pntrLevel, int _rfrnLevel) :
    userDefinedType(_userDefinedType), typePos(_typePos), pntrLevel(_pntrLevel), rfrnLevel(_rfrnLevel) {}

bool parser::TypeInformation::operator==(TypeInformation& _) {

    return (
        userDefinedType == _.userDefinedType &&
        typePos == _.typePos &&
        pntrLevel == _.pntrLevel &&
        rfrnLevel == _.rfrnLevel 
    );

}

parser::TypeInformation* parser::TypeInformation::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {

    int _keyWordPos, _pntrLevel = 0, _rfrnLevel = 0;
    bool _isUserDefinedType = true;

    if (parser::isPrimativeType((*_astCntrl->tokensColl)[*_astCntrl->crrntPos])) {

        _keyWordPos = _astCntrl->globalStorage->addNewKeyWordId((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id);

        _isUserDefinedType = false;

    } else _keyWordPos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->phr);

    (*_astCntrl->crrntPos)++;
        
    parser_helper::setPntrRfrnLevel(_astCntrl, &_pntrLevel, &_rfrnLevel);

    TypeInformation* _ = (TypeInformation*) malloc(sizeof(TypeInformation));
    new (_) TypeInformation(_isUserDefinedType, _keyWordPos, _pntrLevel, _rfrnLevel);

    return _;

}


parser::Environment::Environment(unsigned char _type) : type(_type), storage(new Storage()) {

    variablesNamesDeclared = new utils::LinkedList <int>();
    functionsNamesDeclared = new utils::LinkedList <int>();
    structsNamesDeclared = new utils::LinkedList <int>();

}
        
bool parser::Environment::checkIfNameIsDeclared(int _) {

    if (checkVariableNameIsDeclared(_)) return true;
    if (checkFunctionNameIsDeclared(_)) return true;

    return false;

}

bool parser::Environment::addVaribleNameDeclared(int _) {

    if (checkIfNameIsDeclared(_)) return false;

    variablesNamesDeclared->add(_);

    return true;

}

bool parser::Environment::checkVariableNameIsDeclared(int _) {
    for (int _c = 0; _c < variablesNamesDeclared->count; _c++) // Check variables names declared
        if ((*variablesNamesDeclared)[_c] == _) return true;

    return false; 
}

bool parser::Environment::addFunctionNameDeclared(int _) {

    if (checkIfNameIsDeclared(_)) return false;

    functionsNamesDeclared->add(_);

    return true;

}

bool parser::Environment::checkFunctionNameIsDeclared(int _) {
    for (int _c = 0; _c < functionsNamesDeclared->count; _c++) // Check variables names declared
        if ((*functionsNamesDeclared)[_c] == _) return true;

    return false; 
}

bool parser::Environment::addStructNameDeclared(int _) {
    if (checkIfNameIsDeclared(_)) return false;
    structsNamesDeclared->add(_);
    return true;   
}

bool parser::Environment::checkStructNameIsDeclared(int _) {
    for (int _c = 0; _c < structsNamesDeclared->count; _c++) // Check variables names declared
        if ((*structsNamesDeclared)[_c] == _) return true;

    return false; 
}


parser::Storage::Storage() {
    values = new utils::LinkedList <char*>();
    names = new utils::LinkedList <char*>();
    expressionsId = new utils::LinkedList <int>();
    keyWordsId = new utils::LinkedList <int>();
    types = new utils::LinkedList <TypeInformation*>();
}

int parser::Storage::addNewName(char* _) {
    int _rtr;
    if ((_rtr = names->getObjectPosition(_, NULL)) == -1) 
        _rtr = names->add(_);
    return _rtr; 
}

int parser::Storage::addNewValue(char* _) {
    int _rtr;
    if ((_rtr = values->getObjectPosition(_, NULL)) == -1) 
        _rtr = values->add(_);
    return _rtr; 
}

int parser::Storage::addNewExpressionId(int _) {
    int _rtr;
    if ((_rtr = expressionsId->getObjectPosition(_, NULL)) == -1) 
        _rtr = expressionsId->add(_);
    return _rtr; 
}

int parser::Storage::addNewKeyWordId(int _) {
    int _rtr;
    if ((_rtr = keyWordsId->getObjectPosition(_, NULL)) == -1) 
        _rtr = keyWordsId->add(_);
    return _rtr; 
}

int parser::Storage::addNewType(TypeInformation* _) {
    int _rtr;
    if ((_rtr = types->getObjectPosition(_, [](TypeInformation* _f, TypeInformation* _s) -> bool { return *_f == *_s; })) == -1) 
        _rtr = types->add(_);
    return _rtr; 
}


parser::Ast_Control_Exception::Ast_Control_Exception(char* _description) : description(_description) {

    std::cout << "Error: " << description << std::endl;

    exit(-1);

}


parser::Ast_Control::Ast_Control(utils::LinkedList <parser::Token*>* _tokensColl) : 
    tokensColl(_tokensColl), globalBlock(NULL), globalStorage(new Storage()) {

        crrntPos = (int*) malloc(sizeof(int));
        *crrntPos = -1;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Control::getNewNodes(Ast_Node_Block* _crrntBlock, bool _) {

    utils::LinkedList <Ast_Node*>* _rtr = new utils::LinkedList <Ast_Node*>();

    // std::cout << "Get node token -> " << (*tokensColl)[*crrntPos]->id << std::endl;
    // std::cout << "Node -> " << _crrntBlock->environment->variablesNamesDeclared->count << std::endl;

    switch ((*tokensColl)[*crrntPos]->id)
    {
        case TOKEN_ENDINSTRUCTION:
        case TOKEN_END_CODE:

            (*crrntPos)++;

            return _rtr;

        case TOKEN_STRUCT:
        case TOKEN_CONTRACT:

            _rtr->add(
                Ast_Node_Struct_Declaration::generate(this, _crrntBlock)
            );

            return _rtr;

        case TOKEN_OPENCURLYBRACKET:
            
            _rtr->add(
                Ast_Node_Block::generate(this, _crrntBlock, ENVIRONMENT_TYPE_BLOCKOFCODE)
            );

            return _rtr;

        case TOKEN_CLOSECURLYBRACKET:

            _rtr->add(
                Ast_Node_Block_End::generate()
            );
            (*crrntPos)++;

            return _rtr;

        case TOKEN_POINTER:
        case TOKEN_ADDRESS:

            if (!_) break;

            _rtr->add(
                Ast_Node_Pointer_Operators::generate(this, _crrntBlock)
            );

            return _rtr;

        case TOKEN_OPENPARENTHESES:

            if (!_) break;

            _rtr->add(
                Ast_Node_Parenthesis::generate(this, _crrntBlock)
            );

            return _rtr;
        
        default: break;
    }

    int _namePos = 
        ((*tokensColl)[*crrntPos]->id != TOKEN_IDENTIFIER) ? -1 : globalStorage->names->getObjectPosition((*tokensColl)[*crrntPos]->phr, NULL);

    if (parser::isPrimativeType((*tokensColl)[*crrntPos]) || _crrntBlock->checkStructNameIsDeclared(_namePos) ) {

        TypeInformation* _typeInformation = TypeInformation::generate(this, _crrntBlock);

        if ((*tokensColl)[(*crrntPos) + 1]->id == TOKEN_OPENPARENTHESES) {

            _rtr->add(
                Ast_Node_Function_Declaration::generate(this, _crrntBlock, _typeInformation)
            );

        }

        else {

            _rtr->join(
                Ast_Node_Variable_Declaration::generate(this, _crrntBlock, _typeInformation)
            );
            
        }

    }

    else if (_) {

        if ((*tokensColl)[(*crrntPos)]->id == TOKEN_IDENTIFIER && (*tokensColl)[(*crrntPos) + 1]->id == TOKEN_OPENPARENTHESES) {

            _rtr->add(
                Ast_Node_Function_Call::generate(this, _crrntBlock)
            );

        }

        else if (parser::isAssignment((*tokensColl)[*crrntPos]) || parser::isAssignment((*tokensColl)[*crrntPos + 1])) {

            _rtr->add(
                Ast_Node_Variable_Assignment::generate(
                    this, _crrntBlock
                )
            );

        }

        else if ((*tokensColl)[*crrntPos]->id == TOKEN_IDENTIFIER) {

            _rtr->add(
                Ast_Node_Variable::generate(
                    this, _crrntBlock
                )
            );

        }

        else if (parser::isImplicitValue((*tokensColl)[*crrntPos]))

            _rtr->add(Ast_Node_Value::generate(this, _crrntBlock));

    }

    else _rtr->add(Ast_Node_Expression::generate(this, _crrntBlock));

    return _rtr;

}

void parser::Ast_Control::generateAst() {

    globalBlock = Ast_Node_Block::generate(this, NULL, ENVIRONMENT_TYPE_BLOCKOFCODE);

}




