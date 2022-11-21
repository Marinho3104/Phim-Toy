#include "./ast_nodes.h"

#include "./../utils/linkedList.h" // Linked List
#include "./definitions.h" // Parser definitions
#include "./ast_helper.h" // Ast Helper
#include "./token.h" // Token 
#include "./ast.h" // Ast Control
#include <iostream>

/*      Ast Node     */

parser::Ast_Node::Ast_Node(int _id) : id(_id) {}

/*      Ast Node Block Code     */

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(int _envId) : Ast_Node(AST_NODE_CODE_BLOCK), environment(_envId) {
    namesUsedInBlock = new utils::LinkedList <char*>();
}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Code_Block::getNewNodes(Ast_Control* _astCntrl) {

    utils::LinkedList <Ast_Node*>* _rtr = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>));
    new (_rtr) utils::LinkedList <Ast_Node*>();

    std::cout << "Get node ->" << (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id << std::endl;

    switch ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id)
    {

    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: break;

    case TOKEN_ENDINSTRUCTION: _astCntrl->crrntTkPos++; free(_rtr); _rtr = getNewNodes(_astCntrl); break;

    case TOKEN_OPENCURLYBRACKET:

        _astCntrl->crrntTkPos++;
        
        _rtr->add(
            Ast_Node_Code_Block::generate(
                _astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL
            )
        );

        break;
    
    default: goto cont;

    }

    return _rtr; 

cont:

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    if (parser::isPrimativeType(_tk)) { // Miss struct declarations

        Type_Information* _typeInformation = Type_Information::generate(_astCntrl);

        _rtr->join(
            Ast_Node_Variable_Declaration::generate(_astCntrl, _typeInformation)
        );

    }

    else {

        _rtr->add(
            Ast_Node_Expression::generate(
                _astCntrl
            )
        );

    }

    return _rtr;

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* _) { return namesUsedInBlock->getObjectPosition(_, NULL); }

bool parser::Ast_Node_Code_Block::addNewName(char* _) {
    if (namesUsedInBlock->getObjectPosition(_, NULL) != -1) return false;
    namesUsedInBlock->add(_);
    return true;
}

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>* _body, int _envId) 
    : Ast_Node(AST_NODE_CODE_BLOCK), body(_body), environment(_envId) {}

parser::Ast_Node_Code_Block* parser::Ast_Node_Code_Block::generate(Ast_Control* _astCntrl, int _envId) {
    std::cout << "--> Ast Node Block Code <--" << std::endl;

    parser::Ast_Node_Code_Block* _ = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_) parser::Ast_Node_Code_Block(_envId);

    utils::LinkedList <parser::Ast_Node*>* _body = new utils::LinkedList <parser::Ast_Node*>();

    _astCntrl->crrntBlock = _;

    while(_astCntrl->crrntTkPos < _astCntrl->tokensColl->count) {

        if (
            !_body->join(
                getNewNodes(
                    _astCntrl
                )
            )
        ) break;

    }

    std::cout << "--> End of Ast Node Block Code <--" << std::endl;

    new (_) parser::Ast_Node_Code_Block(
        _body, _envId
    );

    return _;

}

/*      Ast Node Expression     */

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* _f, Ast_Node* _s, int _expId) : Ast_Node(AST_NODE_EXPRESSION), frst(_f), scnd(_s), expId(_expId) {}

parser::Ast_Node* parser::Ast_Node_Expression::getFirstNode(Ast_Control* _astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Pointer_Operations
    *   Ast_Node_Variable_Assignment
    */

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];
    parser::Token* _tkNext = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos + 1];

    if (_tk->id == TOKEN_IDENTIFIER) {
        
        if (parser::isAssignment(_tkNext)) return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);
    
        return parser::Ast_Node_Variable::generate(_astCntrl);
    }

    if (parser::isImplicitValue(_tk)) return parser::Ast_Node_Value::generate(_astCntrl);

    if (
        _tk->id == TOKEN_POINTER || _tk->id == TOKEN_ADDRESS
    ) {

        utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

        parser_helper::setPointerOperators(_astCntrl, _operators);

        if (parser::isAssignment((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos + 1])) {
            
            _astCntrl->crrntTkPos -= _operators->count;

            delete _operators;
        
            return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);

        }

        return parser::Ast_Node_Pointer_Operators::generate(
            _astCntrl, _operators
        );

    }

    if (parser::isAssignment(_tk)) return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);

    std::cout << "Ast Node Expression get first -> Something went wrong" << std::endl;

    exit(-1);

}

parser::Ast_Node* parser::Ast_Node_Expression::getSecondNode(Ast_Control* _astCntrl) {
    
    if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_ENDINSTRUCTION) return NULL;

    _astCntrl->crrntTkPos++; //

    return parser::Ast_Node_Expression::generate(_astCntrl); 

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* _astCntrl) {

    std::cout << "--> Ast Node Expression <--" << std::endl;

    parser::Ast_Node_Expression* _ = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    parser::Ast_Node* _frstValue, *_scndValue;
    int _expressionId;

    _frstValue = getFirstNode(_astCntrl);
    _expressionId = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id;
    _scndValue = getSecondNode(_astCntrl);

    new (_) parser::Ast_Node_Expression(
        _frstValue, _scndValue, _scndValue ? _expressionId : -1
    );

    return _;

}

/*      Ast Node Value     */

parser::Ast_Node_Value::Ast_Node_Value(int _valuePos, int _id) : Ast_Node(AST_NODE_VALUE), valuePos(_valuePos), id(_id) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate(Ast_Control* _astCntrl) {

    std::cout << "--> Ast Node Value <--" << std::endl;

    parser::Ast_Node_Value* _ = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new(_) parser::Ast_Node_Value(
        _astCntrl->storage->addNewValue(
            (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->phr
        ), (*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)++]->id
    );

    return _;

}

/*      Ast Node Variable Declaration     */

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int _typePos, int _declId) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), typePos(_typePos), declId(_declId) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* _astCntrl, Type_Information* _type) {
    
    utils::LinkedList <parser::Ast_Node*>* _rtr = new utils::LinkedList <parser::Ast_Node*>();
    utils::LinkedList <int>* _operators;

    int _typeInfoPos, _declId;
    while((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id != TOKEN_ENDINSTRUCTION) {
        std::cout << "--> Ast Node Variable Declaration <--" << std::endl;

        _typeInfoPos = _astCntrl->storage->addNewType(_type);

        if (!_astCntrl->crrntBlock->addNewName((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->phr)) 
            { std::cout << "Error aqui" << std::endl; exit(-1); /* Call exception TODO */ }

        _declId = _astCntrl->crrntBlock->getDeclarationId((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++]->phr);

        parser::Ast_Node* _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(
            _typeInfoPos, _declId
        );

        _rtr->add(_varDecl);

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_EQUAL) { exit(-1); /* TODO Assign */ }

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_COMMA) {

            _astCntrl->crrntTkPos++;

            _operators = new utils::LinkedList <int>();

            parser_helper::setPointerOperators(_astCntrl, _operators);
            
            _type = (Type_Information*) malloc(sizeof(Type_Information));
            new(_type) Type_Information(_type->id, _operators);     

        }

    }

    return _rtr;

}

/*      Ast Node Variable     */

parser::Ast_Node_Variable::Ast_Node_Variable(int _declId) : Ast_Node(AST_NODE_VARIABLE), declId(_declId) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* _astCntrl) {

    std::cout << "--> Ast Node Variable <--" << std::endl;
    parser::Ast_Node_Variable* _ = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));
    
    int _declId = _astCntrl->crrntBlock->getDeclarationId((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)++]->phr);

    if (_declId == -1) { std::cout << "Error Ast Variable" << std::endl; exit(-1);} // Error not defined variable TODO

    new (_) parser::Ast_Node_Variable(_declId);

    return _;

}

/*      Ast Node Pointer Operators     */

parser::Ast_Node_Pointer_Operators::Ast_Node_Pointer_Operators(utils::LinkedList <int>* _oprs, Ast_Node* _value) 
    : Ast_Node(AST_NODE_POINTER_OPERATORS), operations(_oprs), value(_value) {}

parser::Ast_Node* parser::Ast_Node_Pointer_Operators::getNewNode(Ast_Control* _astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Parenthesis //
    */

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    if (_tk->id == TOKEN_IDENTIFIER) return parser::Ast_Node_Variable::generate(_astCntrl);

    if (parser::isImplicitValue(_tk)) return parser::Ast_Node_Value::generate(_astCntrl);

    std::cout << "Ast Node Pointer Operators -> Something went wrong" << std::endl;

    exit(-1);

}

parser::Ast_Node_Pointer_Operators* parser::Ast_Node_Pointer_Operators::generate(Ast_Control* _astCntrl, utils::LinkedList <int>* _operators) {

    std::cout << "--> Ast Node Pointer Operators <--" << std::endl;

    parser::Ast_Node* _value = getNewNode(_astCntrl);

    if (!_value) { std::cout << "Error" << std::endl; exit(-1); }

    parser::Ast_Node_Pointer_Operators* _ = (parser::Ast_Node_Pointer_Operators*) malloc(sizeof(parser::Ast_Node_Pointer_Operators));
    new (_) parser::Ast_Node_Pointer_Operators(
        _operators, _value
    );

    return _;

}

/*      Ast Node Variable Assignment     */

parser::Ast_Node_Variable_Assignment::Ast_Node_Variable_Assignment(bool _isLft, Ast_Node* _valBefAssign, int _expId, Ast_Node* _value) 
    : Ast_Node(AST_NODE_VARIABLE_ASSIGNMENT), opIsLeft(_isLft), valueBeforeAssign(_valBefAssign), id(_expId), value(_value) {}

parser::Ast_Node* parser::Ast_Node_Variable_Assignment::getValueBeforeAssign(Ast_Control* _astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Variable 
    *   Ast_Parenthesis 
    *   Ast_Pointer_Operators //
    */

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    if (_tk->id == TOKEN_IDENTIFIER) return parser::Ast_Node_Variable::generate(_astCntrl);

    if (
        _tk->id == TOKEN_POINTER || _tk->id == TOKEN_ADDRESS
    ) {

        utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

        parser_helper::setPointerOperators(_astCntrl, _operators);

        return parser::Ast_Node_Pointer_Operators::generate(
            _astCntrl, _operators
        );

    }

    std::cout << "Ast Node Variable Assignment get value before assignment -> Something went wrong" << std::endl;

    exit(-1);

}

parser::Ast_Node* parser::Ast_Node_Variable_Assignment::getValue(Ast_Control* _astCntrl) {

    return parser::Ast_Node_Expression::generate(_astCntrl);

}

parser::Ast_Node_Variable_Assignment* parser::Ast_Node_Variable_Assignment::generate(Ast_Control* _astCntrl) {

    std::cout << "--> Variable Assignment <--" << std::endl;
    parser::Ast_Node_Variable_Assignment* _ = (parser::Ast_Node_Variable_Assignment*) malloc(sizeof(parser::Ast_Node_Variable_Assignment));
    parser::Ast_Node* _valBefAssign, *_value;
    bool _islft = false;
    parser::Token* _expTk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    if (parser::isAssignment((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)])) {
        if (!parser::isSingleAssignment((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)])) { std::cout << "error" << std::endl; exit(-1); } // TODO
        _astCntrl->crrntTkPos++;
        _islft = true;
    }

    _valBefAssign = getValueBeforeAssign(_astCntrl);

    if (!_islft) _expTk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++];

    if (parser::isSingleAssignment(_expTk)) _value = NULL;
    else _value = getValue(_astCntrl);

    new (_) Ast_Node_Variable_Assignment(
        _islft, _valBefAssign, _expTk->id, _value
    );

    return _;

}













