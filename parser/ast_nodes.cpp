#include "./ast_nodes.h"

#include "./../utils/linkedList.h" // Linked List
#include "./definitions.h" // Parser definitions
#include "./ast_helper.h" // Ast Helper
#include "./token.h" // Token 
#include "./ast.h" // Ast Control
#include "./parser_helper.h" // Parser helper

#include <iostream>

/*      Ast Node     */

parser::Ast_Node::Ast_Node(int _id) : id(_id) {}

/*      Ast Node Block Code     */

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(int _envId, utils::LinkedList <char*>* _namesDeclrs, Ast_Node_Code_Block* _previousBlock) 
    : Ast_Node(AST_NODE_CODE_BLOCK), environment(_envId), namesUsedInBlock(_namesDeclrs), previousBlock(_previousBlock) {
        if (!namesUsedInBlock) namesUsedInBlock = new utils::LinkedList <char*>();
}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Code_Block::getNewNodes(Ast_Control* _astCntrl) {

    utils::LinkedList <Ast_Node*>* _rtr = (utils::LinkedList <Ast_Node*>*) malloc(sizeof(utils::LinkedList <Ast_Node*>));
    new (_rtr) utils::LinkedList <Ast_Node*>();

    std::cout << "Get node ->" << (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id << std::endl;

    switch ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id)
    {

    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: _astCntrl->crrntTkPos++; break;

    case TOKEN_ENDINSTRUCTION: _astCntrl->crrntTkPos++; free(_rtr); _rtr = getNewNodes(_astCntrl); break;

    case TOKEN_OPENCURLYBRACKET:

        _astCntrl->crrntTkPos++;
        
        _rtr->add(
            Ast_Node_Code_Block::generate(
                _astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL, NULL
            )
        );

        break;

    case TOKEN_STRUCT: case TOKEN_CONTRACT:

        _rtr->add(
            Ast_Node_Struct_Declaration::generate(
                _astCntrl
            )
        );

        break;

    default: goto cont;

    }

    return _rtr; 

cont:

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    if (parser::isPrimativeType(_tk) || _astCntrl->crrntBlock->getDeclarationId(_tk->phr) != -1 ) { // Miss struct declarations TODO

        switch ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos + 1]->id)
        {
        case TOKEN_EQUAL: case TOKEN_OPENPARENTHESES: goto expressionGen;
        
        default:
            break;
        }

        Type_Information* _typeInformation = Type_Information::generate(_astCntrl);

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos + 1]->id == TOKEN_OPENPARENTHESES)

            _rtr->add(
                Ast_Node_Function_Declaration::generate(_astCntrl, _typeInformation)
            );

        else
            _rtr->join(
                Ast_Node_Variable_Declaration::generate(_astCntrl, _typeInformation)
            );

    }

    else {

    expressionGen:

        _rtr->add(
            Ast_Node_Expression::generate(
                _astCntrl
            )
        );

    }

    return _rtr;

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* _) {
    if (!_) return -1;
    int _rtr;
    if ((_rtr = namesUsedInBlock->getObjectPosition(_, NULL)) == -1)
        if (previousBlock) return previousBlock->getDeclarationId(_);
    return _rtr;
}

int parser::Ast_Node_Code_Block::getDeclarationIdCurrntBlock(char* _) { return namesUsedInBlock->getObjectPosition(_, NULL); }

bool parser::Ast_Node_Code_Block::addNewName(char* _) {
    if (namesUsedInBlock->getObjectPosition(_, NULL) != -1) return false;
    namesUsedInBlock->add(_);
    return true;
}

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>* _body, int _envId) 
    : Ast_Node(AST_NODE_CODE_BLOCK), body(_body), environment(_envId) {}

parser::Ast_Node_Code_Block* parser::Ast_Node_Code_Block::generate(Ast_Control* _astCntrl, int _envId, utils::LinkedList <char*>* _namesDeclrs) {
    std::cout << "--> Ast Node Block Code <--" << std::endl;

    parser::Ast_Node_Code_Block* _ = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_) parser::Ast_Node_Code_Block(_envId, _namesDeclrs, _astCntrl->crrntBlock);

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

    _astCntrl->crrntBlock = _astCntrl->crrntBlock->previousBlock;

    _astCntrl->code_blocks->add(_);

    return _;

}

/*      Ast Node Expression     */

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* _f, Ast_Node* _s, int _expId) : Ast_Node(AST_NODE_EXPRESSION), frst(_f), scnd(_s), expId(_expId) {}

parser::Ast_Node* parser::Ast_Node_Expression::getFirstNode(Ast_Control* _astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Node_Parenthesis
    *   Ast_Node_Function_Call
    *   Ast_Node_Pointer_Operations
    */

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];
    parser::Token* _tkNext = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos + 1];

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER:
        
        if (parser::isAssignment(_tkNext)) return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);

        else if (_tkNext->id == TOKEN_OPENPARENTHESES) return parser::Ast_Node_Function_Call::generate(_astCntrl);
    
        return parser::Ast_Node_Variable::generate(_astCntrl);

    case TOKEN_POINTER: case TOKEN_ADDRESS:

        {
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
    
    case TOKEN_OPENPARENTHESES:

        {
            int _ = _astCntrl->crrntTkPos;

            while(
                (*_astCntrl->tokensColl)[_++]->id != TOKEN_CLOSEPARENTHESES
            );

            if (parser::isAssignment((*_astCntrl->tokensColl)[_])) return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);

            return parser::Ast_Node_Parenthesis::generate(_astCntrl);
        }

    default:
        break;
    }

    if (parser::isImplicitValue(_tk)) return parser::Ast_Node_Value::generate(_astCntrl);

    if (parser::isAssignment(_tk)) return parser::Ast_Node_Variable_Assignment::generate(_astCntrl);

    std::cout << "Ast Node Expression get first -> Something went wrong" << std::endl;

    exit(-1);

}

parser::Ast_Node* parser::Ast_Node_Expression::getSecondNode(Ast_Control* _astCntrl) {

    switch ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id)
    {
    case TOKEN_ENDINSTRUCTION: case TOKEN_CLOSEPARENTHESES:

        return NULL;
    
    default:
        break;
    }
    
    _astCntrl->crrntTkPos++;

    return parser::Ast_Node_Expression::generate(_astCntrl); 

}

int parser::Ast_Node_Expression::getResultSize(utils::LinkedList <Ast_Node_Variable_Declaration*>* _varDeclTable, utils::LinkedList <Ast_Node_Function_Declaration*>* _funcDeclTable, utils::LinkedList <Type_Information*>* _typeTable) {

    int _ = 0;

    if (!frst) return _;

    switch (frst->id)
    {
    case AST_NODE_VALUE: _ =  ((Ast_Node_Value*) frst)->getTypeSize(); break;
    case AST_NODE_VARIABLE: _ =  ((Ast_Node_Variable*) frst)->getTypeSize(_varDeclTable, _typeTable); break;
    case AST_NODE_PARENTHESIS: _ = ((Ast_Node_Parenthesis*) frst)->getTypeSize(_varDeclTable, _funcDeclTable, _typeTable); break;
    case AST_NODE_FUNCTION_CALL: _ = ((Ast_Node_Function_Call*) frst)->getTypeSize(_funcDeclTable, _typeTable); break;
    default: break;
    }

    if (!scnd) return _;

    if (
        int _n = ((Ast_Node_Expression*) scnd)->getResultSize(_varDeclTable, _funcDeclTable, _typeTable) > _
    ) _ = _n;

    return _;

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

parser::Ast_Node_Value::Ast_Node_Value(int _valuePos, int _id) : Ast_Node(AST_NODE_VALUE), valuePos(_valuePos), tkId(_id) {}

int parser::Ast_Node_Value::getTypeSize() {

    return parser_helper::getSizeImplicitValue(tkId);

}

// Return here needs to be deallocated
parser::Type_Information* parser::Ast_Node_Value::getType() {

    parser::Type_Information* _ = (parser::Type_Information*) malloc(sizeof(parser::Type_Information));
    utils::LinkedList <int>* __ = new utils::LinkedList <int>();

    new (_) parser::Type_Information(
        parser_helper::getTokenIdTypeFromTokenIdImplicitValue(tkId),
        0, __
    );

    return _;

}

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

int parser::Ast_Node_Variable_Declaration::getTypeSize(utils::LinkedList <Type_Information*>* _typeTable) {

    return (*_typeTable)[typePos]->getByteSize();

}

parser::Type_Information* parser::Ast_Node_Variable_Declaration::getType(utils::LinkedList <Type_Information*>* _typeTable) { return (*_typeTable)[typePos]; }

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

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_EQUAL) { _astCntrl->crrntTkPos--; _rtr->add(parser::Ast_Node_Variable_Assignment::generate(_astCntrl)); }

        if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_COMMA) {

            _astCntrl->crrntTkPos++;

            _operators = new utils::LinkedList <int>();

            parser_helper::setPointerOperators(_astCntrl, _operators);
            
            _type = (Type_Information*) malloc(sizeof(Type_Information));
            new(_type) Type_Information(_type->id, _type->usrDefDeclId, _operators);     

        }

    }

    return _rtr;

}

/*      Ast Node Variable     */

parser::Ast_Node_Variable::Ast_Node_Variable(int _declId) : Ast_Node(AST_NODE_VARIABLE), declId(_declId) {}

int parser::Ast_Node_Variable::getTypeSize(utils::LinkedList <Ast_Node_Variable_Declaration*>* _varDeclTable, utils::LinkedList <Type_Information*>* _typeTable) {

    for (int _ = 0; _ < _varDeclTable->count; _++)

        if ((*_varDeclTable)[_]->declId == declId) return (*_varDeclTable)[_]->getTypeSize(_typeTable);

    std::cout << "Error get size variable <" << std::endl; exit(-1);

    return -1;

}

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
    : Ast_Node(AST_NODE_POINTER_OPERATORS), operations(_oprs), value(_value), pntrLvl(0) {

        for (int _ = 0; _ < operations->count; _++)  // TODO & dont mean address is reference fuck ....
            
            pntrLvl += ((*operations)[_] == TOKEN_POINTER) ? 1 : -1;

}

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

int parser::Ast_Node_Pointer_Operators::getTypeSize() {

    int _fnlS = 0;



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
    : Ast_Node(AST_NODE_VARIABLE_ASSIGNMENT), opIsLeft(_isLft), valueBeforeAssign(_valBefAssign), expId(_expId), value(_value) {}

parser::Ast_Node* parser::Ast_Node_Variable_Assignment::getValueBeforeAssign(Ast_Control* _astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Variable 
    *   Ast_Parenthesis //
    *   Ast_Pointer_Operators 
    */

    parser::Token* _tk = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos];

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER: return parser::Ast_Node_Variable::generate(_astCntrl);
    case TOKEN_POINTER: case TOKEN_ADDRESS: 

        {

            utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

            parser_helper::setPointerOperators(_astCntrl, _operators);

            return parser::Ast_Node_Pointer_Operators::generate(
                _astCntrl, _operators
            );

        }
    
    case TOKEN_OPENPARENTHESES: return parser::Ast_Node_Parenthesis::generate(_astCntrl);

    default:
        break;
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

/*      Ast Node Parenthesis     */

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node* _value) : Ast_Node(AST_NODE_PARENTHESIS), value(_value) {}

int parser::Ast_Node_Parenthesis::getTypeSize(utils::LinkedList <Ast_Node_Variable_Declaration*>* _varDeclTable, utils::LinkedList <Ast_Node_Function_Declaration*>* _funcDeclTable, utils::LinkedList <Type_Information*>* _typeTable) {

    return ((Ast_Node_Expression*) value)->getResultSize(_varDeclTable, _funcDeclTable, _typeTable);

}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate(Ast_Control* _astCntrl) {
    std::cout << "--> Generate Parenthesis <--" << std::endl;
    parser::Ast_Node_Parenthesis* _ = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));
    (_astCntrl->crrntTkPos)++;
    new (_) parser::Ast_Node_Parenthesis(
        parser::Ast_Node_Expression::generate(_astCntrl)
    );
    std::cout << "--> End Parenthesis <--" << std::endl;
    (_astCntrl->crrntTkPos)++;
    return _;

}

/*      Ast Node Function Declaration     */

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(int _typeRtrPos, int _declId, utils::LinkedList <Ast_Node*>* _params, Ast_Node* _body) 
    : Ast_Node(AST_NODE_FUNCTION_DECLARATION), typeRtrPos(_typeRtrPos), declId(_declId), parameters(_params), body(_body) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(Ast_Control* _astCntrl, utils::LinkedList <char*>* _namesDeclrs) {

    std::cout << "--> Ast Node Function Parameters <--" << std::endl;
    utils::LinkedList <Ast_Node*>* _rtr = new utils::LinkedList <Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _varDecl;
    Type_Information* _typeInfo;
    int _typeInfoPos, _declId;

    (_astCntrl->crrntTkPos)++;

    while((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id != TOKEN_CLOSEPARENTHESES) {

        _typeInfo = Type_Information::generate(_astCntrl);
        _typeInfoPos = _astCntrl->storage->addNewType(_typeInfo);

        switch ((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id)
        {
        case TOKEN_COMMA:
            _astCntrl->crrntTkPos++;
        case TOKEN_CLOSEPARENTHESES: 
        
            {

                _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
                new (_varDecl) parser::Ast_Node_Variable_Declaration(
                    _typeInfoPos, -1
                );

                _rtr->add(_varDecl);

            }
            continue;
 
        default:
            break;
        }

        if (_namesDeclrs->getObjectPosition(
            (*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->phr, NULL
        ) != -1) { std::cout << "Error " << std::endl; exit(-1); /* TODO*/ }

        _declId = _namesDeclrs->add((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)++]->phr);

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(
            _typeInfoPos, _declId
        );

        _rtr->add(
            _varDecl
        );

        if ((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_EQUAL) { exit(-1); /*TODO*/}

        if ((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_COMMA) (_astCntrl->crrntTkPos)++;

    }

    _astCntrl->crrntTkPos++;

    return _rtr;

}

int parser::Ast_Node_Function_Declaration::getTypeSize(utils::LinkedList <Type_Information*>* _typeTable) {

    return (*_typeTable)[typeRtrPos]->getByteSize();

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* _astCntrl, Type_Information* _typeRtr) {

    std::cout << "--> Node Function Declaration <--" << std::endl;
    parser::Ast_Node_Function_Declaration* _ = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    int _typeRtrPos = _astCntrl->storage->addNewType(_typeRtr), _declId;
    parser::Ast_Node* _body = NULL;

    if (!_astCntrl->crrntBlock->addNewName((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->phr)) { std::cout << "Error" << std::endl; exit(-1); /*TODO*/ }

    _declId = _astCntrl->crrntBlock->getDeclarationId((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)++]->phr); 

    utils::LinkedList <char*>* _namesDeclrs = new utils::LinkedList <char*>();

    utils::LinkedList <parser::Ast_Node*>* _params = getParameters(_astCntrl, _namesDeclrs);

    if ((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_OPENCURLYBRACKET) {

        _astCntrl->crrntTkPos++;
        _body = parser::Ast_Node_Code_Block::generate(_astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMEMT_FUNCTION, _namesDeclrs);

    }

    new (_) parser::Ast_Node_Function_Declaration(
        _typeRtrPos, _declId, _params, _body
    );

    return _;

}

/*      Ast Node Function Call     */

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(int _declId, utils::LinkedList <Ast_Node*>* _params) 
    : Ast_Node(AST_NODE_FUNCTION_CALL), declId(_declId), parameters(_params) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getFunctionCallParameters(Ast_Control* _astCntrl) {

    utils::LinkedList <Ast_Node*>* _parameters = new utils::LinkedList <Ast_Node*>();

    _astCntrl->crrntTkPos++;

    while((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id != TOKEN_CLOSEPARENTHESES) {

        _parameters->add(
            parser::Ast_Node_Expression::generate(
                _astCntrl
            )
        );

        if ((*_astCntrl->tokensColl)[(_astCntrl->crrntTkPos)]->id == TOKEN_COMMA) (_astCntrl->crrntTkPos)++;

    }

    _astCntrl->crrntTkPos++;

    return _parameters;

}

int parser::Ast_Node_Function_Call::getTypeSize(utils::LinkedList <Ast_Node_Function_Declaration*>* _funcDeclTable, utils::LinkedList <Type_Information*>* _typeTable) {

    for (int _ = 0; _ < _funcDeclTable->count; _++)

        if ((*_funcDeclTable)[_]->declId == declId) return (*_funcDeclTable)[_]->getTypeSize(_typeTable);

    std::cout << "Error get size function call <" << std::endl; exit(-1);

    return -1;

}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Control* _astCntrl) {

    std::cout << "--> Ast Node Function Call <--" << std::endl;

    parser::Ast_Node_Function_Call* _ = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));

    int _declId;

    if (
        (_declId = _astCntrl->crrntBlock->getDeclarationId((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++]->phr))
        == -1
    ) { std::cout << "Error" << std::endl; exit(-1); /*TODO*/ }

    new (_) parser::Ast_Node_Function_Call(
        _declId, getFunctionCallParameters(_astCntrl)
    );

    return _;

}

/*      Ast Node Struct Declaration     */

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(int _declId, bool _isCntr, Ast_Node* _body) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION), declId(_declId), isContract(_isCntr), body(_body) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate(Ast_Control* _astCntrl) { // TODO struct {} name;
    std::cout << "--> Struct Declaration <--" << std::endl;

    parser::Ast_Node_Struct_Declaration* _ = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));
    bool _isCntr = (*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++]->id == TOKEN_CONTRACT;
    int _declId;

    if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_IDENTIFIER) {

        if (
            !_astCntrl->crrntBlock->addNewName((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->phr)
        ) { std::cout << "Error" << std::endl; exit(-1); /*TODO*/}

        _declId = _astCntrl->crrntBlock->getDeclarationId((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos++]->phr);

    }
    
    else if ((*_astCntrl->tokensColl)[_astCntrl->crrntTkPos]->id == TOKEN_OPENCURLYBRACKET) {

        // TODO 

        exit(-1);

    }

    else {exit(-1); /* TODO */}

    _astCntrl->crrntTkPos++;

    new (_) parser::Ast_Node_Struct_Declaration(
        _declId, _isCntr, parser::Ast_Node_Code_Block::generate(_astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMEMT_STRUCT, NULL)
    );

    return _;

}

/*      Ast Node End     */

parser::Ast_Node_End::Ast_Node_End() : Ast_Node(AST_NODE_END) {}


