#include "./astNodes.h"

#include "./../utils/linkedList.h"
#include "./astHelper.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

/////////// Ast Node Block ///////////

parser::Ast_Node_Block::Ast_Node_Block(Ast_Node_Block* _previous, int _environmentType) : previous(_previous) {
    type = AST_NODE_BLOCK;
    environment = new Environment(_environmentType);
    body = new utils::LinkedList <Ast_Node*>();
}

bool parser::Ast_Node_Block::checkIfNameIsDeclared(int _) {
    if (!environment->checkIfNameIsDeclared(_))
        if (previous) return previous->checkIfNameIsDeclared(_);
        else return false;
    return true;
}

bool parser::Ast_Node_Block::checkVariableNameIsDeclared(int _) {
    if (!(environment->checkVariableNameIsDeclared(_)))
        if (previous && environment->type) return previous->checkVariableNameIsDeclared(_);
        else return false;
    return true;
}

bool parser::Ast_Node_Block::checkFunctionNameIsDeclared(int _) {
    if (!(environment->checkFunctionNameIsDeclared(_)))
        if (previous) return previous->checkFunctionNameIsDeclared(_);
        else return false;
    return true; 
}

bool parser::Ast_Node_Block::checkStructNameIsDeclared(int _) {
    if (!(environment->checkStructNameIsDeclared(_)))
        if (previous) return previous->checkStructNameIsDeclared(_);
        else return false;
    return true;
}

void parser::Ast_Node_Block::generateBody(Ast_Control* _astCntrl) {

    while(*_astCntrl->crrntPos < _astCntrl->tokensColl->count) {
        body->join(_astCntrl->getNewNodes(this, 0));
        if (body->last->object->type == AST_NODE_BLOCK_END) { body->remove(body->last); break; }
    }

    // std::cout << "Out body" << std::endl;

}

parser::Ast_Node_Block* parser::Ast_Node_Block::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock, int _envType) {
    std::cout << "--> Node Block <--" << std::endl;
    parser::Ast_Node_Block* _ = (parser::Ast_Node_Block*) malloc(sizeof(parser::Ast_Node_Block));
    new(_) parser::Ast_Node_Block(_crrntBlock, _envType);
    (*_astCntrl->crrntPos)++;
    _->generateBody(_astCntrl);
    return _;
}

/////////// Ast Node Expression ///////////

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* _frst, Ast_Node* _scnd, int _expIdPos) :
    frst(_frst), scnd(_scnd), expressionIdPos(_expIdPos) { type = AST_NODE_EXPRESSION; }

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
    std::cout << "--> Node Expression <--" << std::endl;
    parser::Ast_Node_Expression* _ = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    utils::LinkedList<Ast_Node*>* _frstValue, *_scndValue;
    int _expressionIdPos;

    _frstValue = _astCntrl->getNewNodes(_crrntBlock, 1); // Get first value

    if ((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id == TOKEN_ENDINSTRUCTION || 
        (*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id == TOKEN_CLOSEPARENTHESES || 
        (*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id == TOKEN_COMMA) {
            _expressionIdPos = -1;
            _scndValue = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>)); 
            new (_scndValue) utils::LinkedList <Ast_Node*>();
    }

    else {
        _expressionIdPos = _astCntrl->globalStorage->addNewExpressionId(
            (*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id
        );
        (*_astCntrl->crrntPos)++;
        _scndValue = _astCntrl->getNewNodes(_crrntBlock, 0); // Get Second value
    }

    new (_) parser::Ast_Node_Expression(
        !_frstValue->count ? NULL : _frstValue->frst->object, // Should not be NULL ever
        !_scndValue->count ? NULL : _scndValue->frst->object,
        _expressionIdPos
    );
    
    return _;

}

/////////// Ast Node Value ///////////

parser::Ast_Node_Value::Ast_Node_Value(int _valuePos, int _typeId) : valuePos(_valuePos), typeId(_typeId) 
    { type = AST_NODE_VALUE; }

parser::Ast_Node_Value* parser::Ast_Node_Value::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
    std::cout << "--> Node Value added <--" << std::endl;
    parser::Ast_Node_Value* _ = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new(_) parser::Ast_Node_Value(
        _astCntrl->globalStorage->addNewValue(
            (*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->phr
        ), (*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->id
    );

    return _;

}

/////////// Ast Node Variable Declaraction ///////////

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int _typePos, int _namePos, Ast_Node* _value) :
    typePos(_typePos), namePos(_namePos), value(_value) { type = AST_NODE_VARIABLE_DECLARATION; }

parser::Ast_Node_Variable_Declaration* 
    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithOutAssign(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock, int _typeInfo, int _namePos) {

        std::cout << "Added, no assigment" << std::endl;

        parser::Ast_Node_Variable_Declaration* _ = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));

        new (_) parser::Ast_Node_Variable_Declaration(
            _typeInfo, _namePos, NULL
        );

        return _;

}

parser::Ast_Node_Variable_Declaration* 
    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithAssign(
        Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock, int _typeInfo, int _namePos, Ast_Node* _value) {

            std::cout << "Added with assigment" << std::endl;
            parser::Ast_Node_Variable_Declaration* _ = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));

            new (_) parser::Ast_Node_Variable_Declaration(
                _typeInfo, _namePos, _value
            );

            return _;

}

utils::LinkedList <parser::Ast_Node*>* 
    parser::Ast_Node_Variable_Declaration::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock, TypeInformation* _typeInfo) {
        std::cout << "--> Variable Declaration <--" << std::endl;
        utils::LinkedList <Ast_Node*>* _rtr = new utils::LinkedList <Ast_Node*>();
        int _namePos, _pntrLevel, _rfrnLevel, _typeInformationPos;

        while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id != TOKEN_ENDINSTRUCTION) {

            _typeInformationPos = _astCntrl->globalStorage->addNewType(_typeInfo);
            _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->phr);
            if (!_crrntBlock->environment->addVaribleNameDeclared(_namePos)) 
                new Ast_Control_Exception("Redefetion of name");
            (*_astCntrl->crrntPos)++;

            if ((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id != TOKEN_EQUAL)

                _rtr->add(
                    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithOutAssign(_astCntrl, _crrntBlock, _typeInformationPos, _namePos)
                );

            else {
                (*_astCntrl->crrntPos)++;
                utils::LinkedList<parser::Ast_Node *>* _ = _astCntrl->getNewNodes(_crrntBlock, 0);

                _rtr->add(
                    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithAssign(
                        _astCntrl, 
                        _crrntBlock, 
                        _typeInformationPos,
                        _namePos,
                        !_->count ? NULL : _->frst->object
                    )
                );
            
            }

            if ((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id == TOKEN_COMMA) {
                (*_astCntrl->crrntPos)++;
                _pntrLevel = 0; _rfrnLevel = 0;
                parser_helper::setPntrRfrnLevel(_astCntrl, &_pntrLevel, &_rfrnLevel);
                new(_typeInfo) TypeInformation(_typeInfo->userDefinedType, _typeInfo->typePos, _pntrLevel, _rfrnLevel); // Handle in getNewNode()  to be malloc as well
            }

        }

        if (!_rtr->count) new Ast_Control_Exception("Expected token identifier after Type indentifier");

        return _rtr;

}

utils::LinkedList <parser::Ast_Node*>* 
    parser::Ast_Node_Variable_Declaration::generateFunctionParameters(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {

        std::cout << "--> Ast Node Variable Declaration -- Function Parameters <--" << std::endl;
        utils::LinkedList <Ast_Node*>* _rtr = new utils::LinkedList <Ast_Node*>();
        utils::LinkedList <int>* _declaredNames = new utils::LinkedList <int>();
        int _namePos, _pntrLevel, _rfrnLevel, _typeInformationPos;

        while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id != TOKEN_CLOSEPARENTHESES) {

            TypeInformation* _typeInformation = TypeInformation::generate(_astCntrl, _crrntBlock);
            _typeInformationPos = _astCntrl->globalStorage->addNewType(_typeInformation);

            switch ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id)
            {
            case TOKEN_COMMA:
                (*_astCntrl->crrntPos)++;
                {
                    parser::Ast_Node_Variable_Declaration* _ = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
                    new (_) parser::Ast_Node_Variable_Declaration(
                        _typeInformationPos, -1, NULL
                    );

                    _rtr->add(_);

                }
            case TOKEN_CLOSEPARENTHESES: continue;
            default: break;
            }

            _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->phr);
            if (_declaredNames->getObjectPosition(_namePos, NULL) != -1) new Ast_Control_Exception("Variable name redefenition");

            _declaredNames->add(_namePos);

            if ((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id != TOKEN_EQUAL)

                _rtr->add(
                    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithOutAssign(_astCntrl, _crrntBlock, _typeInformationPos, _namePos)
                );

            else {
                (*_astCntrl->crrntPos)++;
                utils::LinkedList<parser::Ast_Node *>* _ = _astCntrl->getNewNodes(_crrntBlock, 0);

                _rtr->add(
                    parser::Ast_Node_Variable_Declaration::getVariableDeclarationWithAssign(
                        _astCntrl, 
                        _crrntBlock, 
                        _typeInformationPos,
                        _namePos,
                        !_->count ? NULL : _->frst->object
                    )
                );
            }

            if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_COMMA)
                (*_astCntrl->crrntPos)++;

        }

        (*_astCntrl->crrntPos)++; // -> )

        return _rtr;
}

/////////// Ast Node Pointer Operators ///////////

parser::Ast_Node_Pointer_Operators::Ast_Node_Pointer_Operators(utils::LinkedList <int>* _operatorsBefore, Ast_Node* _value) :
    operatorsBefore(_operatorsBefore), value(_value) { type = AST_NODE_POINTER_OPERATORS; }

parser::Ast_Node_Pointer_Operators* parser::Ast_Node_Pointer_Operators::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
    std::cout << "--> Node Pointer Operators <--" << std::endl;
    parser::Ast_Node_Pointer_Operators* _ = (parser::Ast_Node_Pointer_Operators*) malloc(sizeof(parser::Ast_Node_Pointer_Operators));
    utils::LinkedList <int>* _operatorsUsed = new utils::LinkedList <int>();
    while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_ADDRESS || (*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_POINTER) 
        _operatorsUsed->add((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->id);
    utils::LinkedList <Ast_Node*>* _value = _astCntrl->getNewNodes(_crrntBlock, 1);

    new (_) parser::Ast_Node_Pointer_Operators(
        _operatorsUsed,
        !_value->count ? NULL : _value->frst->object
    );

    return _;

}

/////////// Ast Node Variable ///////////

parser::Ast_Node_Variable::Ast_Node_Variable(int _namePos) :
    namePos(_namePos) { type = AST_NODE_VARIABLE; }

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {

    std::cout << "--> Node Variable Use <--" << std::endl;
    parser::Ast_Node_Variable* _ = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));

    int _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->phr);
    if (!_crrntBlock->checkVariableNameIsDeclared(_namePos)) new Ast_Control_Exception("Variable name not declared");

    (*_astCntrl->crrntPos)++;

    new(_) parser::Ast_Node_Variable(_namePos);

    return _;

}

/////////// Ast Node Variable Assignment ///////////

parser::Ast_Node_Variable_Assignment::Ast_Node_Variable_Assignment(
    bool _opIsLeft, int _namePos, int _expressionIdPos, Ast_Node* _value) :
    opIsLeft(_opIsLeft), namePos(_namePos), expressionIdPos(_expressionIdPos), value(_value)
        { type = AST_NODE_VARIABLE_ASSIGNMENT; }

parser::Ast_Node_Variable_Assignment* parser::Ast_Node_Variable_Assignment::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
    std::cout << "--> Variable Assignment <--" << std::endl;
    parser::Ast_Node_Variable_Assignment* _ = (parser::Ast_Node_Variable_Assignment*) malloc(sizeof(parser::Ast_Node_Variable_Assignment));
    bool _opIsLeft = false;
    int _expressionIdPos;
    parser::Token* _expressionTk;

    if (parser::isAssignment((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)])) {

        if (!parser::isSingleAssignment((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)])) new Ast_Control_Exception("Unexpected token, not single assign ");
        _opIsLeft = true;
        _expressionTk = (*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)];
        _expressionIdPos = _astCntrl->globalStorage->addNewExpressionId((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->id);
    }

    int _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->phr);

    if (!_crrntBlock->environment->checkVariableNameIsDeclared(_namePos)) new Ast_Control_Exception("Variable name not declared");

    if (!_opIsLeft) {
        _expressionTk = (*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)];
        _expressionIdPos = _astCntrl->globalStorage->addNewExpressionId((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->id);
    }

    utils::LinkedList <Ast_Node*>* _value;

    if (parser::isSingleAssignment(_expressionTk)) _value = new utils::LinkedList <Ast_Node*>();
    else _value = _astCntrl->getNewNodes(_crrntBlock, 0);

    new(_) Ast_Node_Variable_Assignment(
        _opIsLeft, _namePos, _expressionIdPos, !_value->count ? NULL : _value->frst->object 
    );

    return _;

}

/////////// Ast Node Parenthesis ///////////

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node* _value) : value(_value) { type = AST_NODE_PARENTHESIS; }

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
    std::cout << "--> Generate Parenthesis <--" << std::endl;
    parser::Ast_Node_Parenthesis* _ = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));
    (*_astCntrl->crrntPos)++;
    utils::LinkedList <parser::Ast_Node*>* _value = _astCntrl->getNewNodes(_crrntBlock, 0);
    new (_) parser::Ast_Node_Parenthesis(
        !_value->count ? NULL : _value->frst->object
    );
    std::cout << "--> End Parenthesis <--" << std::endl;
    (*_astCntrl->crrntPos)++;
    return _;
}

/////////// Ast Node Function declaration ///////////

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    int _typeRtrPos, int _namePos, utils::LinkedList <Ast_Node*>* _parameters, Ast_Node* _body) :
        typeRtrPos(_typeRtrPos), namePos(_namePos), parameters(_parameters), body(_body) { type = AST_NODE_FUNCTION_DECLARATION; }

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock, TypeInformation* _rtrType) {
    std::cout << "--> Node Function Declaration <--" << std::endl;
    parser::Ast_Node_Function_Declaration* _ = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    int _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->phr);
    int _typeRtrPos = _astCntrl->globalStorage->addNewType(_rtrType);

    if (_crrntBlock->checkIfNameIsDeclared(_namePos) || !_crrntBlock->environment->addFunctionNameDeclared(_namePos)) 
        new Ast_Control_Exception("Redefenition of name");

    Ast_Node_Block* _body = NULL; 
    (*_astCntrl->crrntPos)++; // -> (

    utils::LinkedList <parser::Ast_Node*>* _parameters = Ast_Node_Variable_Declaration::generateFunctionParameters(_astCntrl, _crrntBlock);

    if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_OPENCURLYBRACKET) {

        (*_astCntrl->crrntPos)++; // -> 
        _body = (Ast_Node_Block*) malloc(sizeof(Ast_Node_Block));
        new (_body) Ast_Node_Block(_crrntBlock, ENVIRONMENT_TYPE_FUNCTION);

        for (int _c = 0; _c < _parameters->count; _c++) 

            if (((Ast_Node_Variable_Declaration*) (*_parameters)[_c])->namePos != -1) 
                _body->environment->addVaribleNameDeclared(((Ast_Node_Variable_Declaration*) (*_parameters)[_c])->namePos);

        _body->generateBody(_astCntrl);

    }

    new (_) parser::Ast_Node_Function_Declaration(
        _typeRtrPos,
        _namePos,
        _parameters,
        _body
    );


}

/////////// Ast Node Function call ///////////

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(int _namePos, utils::LinkedList <Ast_Node*>* _parameters) :
    namePos(_namePos), parameters(_parameters) { type = AST_NODE_FUNCTION_CALL; }

utils::LinkedList <parser::Ast_Node*>* 
    parser::Ast_Node_Function_Call::getFunctionCallParameters(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {

        utils::LinkedList <Ast_Node*>* _parameters = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>));
        utils::LinkedList <Ast_Node*>* _val;

        while((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id != TOKEN_CLOSEPARENTHESES) {
            _val = _astCntrl->getNewNodes(_crrntBlock, 0);
            if (!_val->count) new Ast_Control_Exception("Error in function call parameters");
            _parameters->add(
                _val->frst->object
            );
            if ((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)]->id == TOKEN_COMMA) (*_astCntrl->crrntPos)++;
        }
        (*_astCntrl->crrntPos)++;

        return _parameters;

}

parser::Ast_Node_Function_Call* 
    parser::Ast_Node_Function_Call::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) {
        std::cout << "--> Node Function Call <--" << std::endl;
        parser::Ast_Node_Function_Call* _ = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));        
        int _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->phr);
        if (!_crrntBlock->checkFunctionNameIsDeclared(_namePos)) new Ast_Control_Exception("Function name not defined");
        (*_astCntrl->crrntPos)++; (*_astCntrl->crrntPos)++;
        utils::LinkedList <Ast_Node*>* _parameters = getFunctionCallParameters(_astCntrl, _crrntBlock);
        new(_) parser::Ast_Node_Function_Call(_namePos, _parameters);
        return _;
}

/////////// Ast Node Struct declaration ///////////

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(int _namePos, bool _isContract, Ast_Node* _body) :
    namePos(_namePos), isContract(_isContract), body(_body) { type = AST_NODE_STRUCT_DECLARATION; }

parser::Ast_Node_Struct_Declaration* 
    parser::Ast_Node_Struct_Declaration::generate(Ast_Control* _astCntrl, Ast_Node_Block* _crrntBlock) { // TODO When is not type defined struct {} test;
        std::cout << "--> Struct Declaration <--" << std::endl;
        parser::Ast_Node_Struct_Declaration* _ = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));
        bool _isContract = (*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->id == TOKEN_CONTRACT;
        int _namePos;

        if ((*_astCntrl->tokensColl)[*_astCntrl->crrntPos]->id == TOKEN_IDENTIFIER) 
            _namePos = _astCntrl->globalStorage->addNewName((*_astCntrl->tokensColl)[(*_astCntrl->crrntPos)++]->phr);

        if (_crrntBlock->checkIfNameIsDeclared(_namePos) || !_crrntBlock->environment->addStructNameDeclared(_namePos)) 
            new Ast_Control_Exception("Redefenition of name");

        (*_astCntrl->crrntPos)++;

        parser::Ast_Node_Block* _newBlock = (parser::Ast_Node_Block*) malloc(sizeof(parser::Ast_Node_Block));
        new (_newBlock) parser::Ast_Node_Block(_crrntBlock, ENVIRONMENT_TYPE_STRUCT);
        _newBlock->generateBody(_astCntrl);

        new(_) parser::Ast_Node_Struct_Declaration(
            _namePos, _isContract, _newBlock
        );

        return _;

}


