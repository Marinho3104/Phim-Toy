#include "./ast_nodes.h"

#include "./../utils/linkedList.h" // Linked List
#include "./parser_definitions.h" // Parser definitions
#include "./ast.h" // Ast Control
#include "./token.h"
#include "./parser_helper.h"
#include "./../utils/commonFunctions.h"
#include "./ast_helper.h"

#include <iostream>

/*      Ast Node     */

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int _nodeId) : node_id(_nodeId) {}

/*      Ast Node Block Code     */

parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() {
    delete names_used;
    delete body;
}

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(int __envId, utils::LinkedList <char*>* __nmsDeclBef, Ast_Node_Code_Block* __prevBlock, int __namesCount) 
    : Ast_Node(AST_NODE_CODE_BLOCK), environment_id(__envId), names_used(__nmsDeclBef), previous_block(__prevBlock), names_count(__namesCount) {
        if (!names_used) names_used = new utils::LinkedList <char*>(); 
        body = new utils::LinkedList <Ast_Node*>();
    }

parser::Ast_Node_Code_Block* parser::Ast_Node_Code_Block::generate(Ast_Control* __astCntrl, int __envId, utils::LinkedList <char*>* __nmsDecl) {

    __astCntrl->printDebugInfo("--> Ast Node Block Code <--");

    int _prevNamesCount = 0;

    if (__envId == AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL) 
        _prevNamesCount = __astCntrl->current_block->names_count + __astCntrl->current_block->names_used->count;

    parser::Ast_Node_Code_Block* _  = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_) parser::Ast_Node_Code_Block(__envId, __nmsDecl, __astCntrl->current_block, _prevNamesCount);

    __astCntrl->current_block = _;
    
    _->generateBody(__astCntrl); 

    if (__envId != AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL) __astCntrl->code_blocks->add(_);

    __astCntrl->printDebugInfo("--> End of Ast Node Block Code <--");

    __astCntrl->current_block = _->previous_block;

    return _;

}

void parser::Ast_Node_Code_Block::generateBody(Ast_Control* __astCntrl) {

    utils::LinkedList <Ast_Node*>* _body;
    bool _sts;

    while(__astCntrl->current_token_position < __astCntrl->tokens_collection->count) {
        
        _body = getNewNodes(__astCntrl);

        _sts = body->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Code_Block::getNewNodes(Ast_Control* __astCntrl) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];
    parser::Token* _tkNext = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position + 1];

    // std::cout << "Get node ->" << _tk->id << std::endl;

    switch (_tk->id)
    {
    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
    case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getNewNodes(__astCntrl); break;
    case TOKEN_OPENCURLYBRACKET: 

        __astCntrl->current_token_position++;

        _->add(
            Ast_Node_Code_Block::generate(
                __astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL, NULL
            )
        );

        break;

    case TOKEN_STRUCT: case TOKEN_CONTRACT:

        _->add(
            Ast_Node_Struct_Declaration::generate(
                __astCntrl
            )
        );

        break;

    default: goto cont;

    }

    return _;

cont:

    bool glb;

    if (parser::isPrimativeType(_tk) || __astCntrl->current_block->getDeclarationId(_tk->phr, glb) != -1) {

        if (_tkNext->id != TOKEN_IDENTIFIER) goto expressionGen;

        Type_Information* _typeInformation = Type_Information::generate(__astCntrl);

        switch ((*__astCntrl->tokens_collection)[__astCntrl->current_token_position + 1]->id)
        {
        case TOKEN_OPENPARENTHESES: 
        
            _->add(
                parser::Ast_Node_Function_Declaration::generate(
                    __astCntrl, _typeInformation
                )
            );

            break;        
        default: 

            delete _;

            _ = Ast_Node_Variable_Declaration::generate(
                    __astCntrl, _typeInformation
                );

            break;
        }
   
    } 
    
    else {

    expressionGen:

        _->add(
            Ast_Node_Expression::generate(__astCntrl)
        );

    }

    return _;

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __n, bool& __glb) {
    if (!__n) return -1;
    int _rtr;
    if ((_rtr = names_used->getObjectPosition(__n, NULL)) == -1)
        if (previous_block) return previous_block->getDeclarationId(__n, __glb);
    if (environment_id == AST_NODE_CODE_BLOCK_ENVIRONMEMT_GLOBAL) __glb = 1;
    return _rtr + names_count;
}

bool parser::Ast_Node_Code_Block::addNewName(char* __n, bool __cpy) {

    if (__cpy) __n = utils::copyString(__n, utils::getStringSize(__n));

    if (names_used->getObjectPosition(__n, NULL) != -1) return false;
    names_used->add(__n);
    return true;
}

/*      Ast Node Expression     */

parser::Ast_Node_Expression::~Ast_Node_Expression() {
    
    if (first) first->~Ast_Node(); free(first);
    if (second) second->~Ast_Node_Expression(); free(second);
}

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node*__frst, Ast_Node_Expression* __scnd, int _expId) 
    : Ast_Node(AST_NODE_EXPRESSION), first(__frst), second(__scnd), expression_id(_expId) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Expression <--");

    parser::Ast_Node_Expression* _ = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    parser::Ast_Node* _frstValue;
    parser::Ast_Node_Expression*_scndValue;
    int _expId;

    _frstValue = getFirstNode(__astCntrl);
    _expId = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->id;
    _scndValue = getSecondNode(__astCntrl);

    new (_) parser::Ast_Node_Expression(
        _frstValue, _scndValue, _scndValue ? _expId : -1
    );

    return _;

}

int parser::Ast_Node_Expression::getByteSize() {

    int _ = 0;

    if (!first) return _;

    switch (first->node_id)
    {
    case AST_NODE_VALUE: _ = ((Ast_Node_Value*) first)->getByteSize(); break;
    default: break;
    }

    if (!second) return _;

    if (
        int _n = ((Ast_Node_Expression*) second)->getByteSize() > _
    ) _ = _n;

    return _;

}

parser::Ast_Node* parser::Ast_Node_Expression::getFirstNode(Ast_Control* __astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Node_Parenthesis
    *   Ast_Node_Function_Call
    *   Ast_Node_Pointer_Operations
    */

    parser::Ast_Node* _;
    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];
    parser::Token* _tkNext = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position + 1];

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER: return parser::Ast_Node_Variable::generate(__astCntrl);
    case TOKEN_POINTER: case TOKEN_ADDRESS:

        {
            utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

            parser_helper::setPointerOperators(__astCntrl, _operators);

            if (parser::isAssignment((*__astCntrl->tokens_collection)[__astCntrl->current_token_position + 1])) {
                
                __astCntrl->current_token_position -= _operators->count;

                delete _operators;
            
                _ = parser::Ast_Node_Variable_Assignment::generate(__astCntrl);

                break;

            }

            _ = parser::Ast_Node_Pointer_Operators::generate(
                __astCntrl, _operators
            );

            delete _operators;

            break;
        }

    default: goto cont;
    }

    return _;

cont:
    if (parser::isImplicitValue(_tk)) return parser::Ast_Node_Value::generate(__astCntrl);

    new Ast_Exception("Ast Node Expression get first -> Something went wrong");

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::getSecondNode(Ast_Control* __astCntrl) {

    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];

    switch (_tk->id)
    {
    case TOKEN_ENDINSTRUCTION: case TOKEN_CLOSEPARENTHESES: return NULL;
    default: __astCntrl->current_token_position++; break;
    }
    
    return parser::Ast_Node_Expression::generate(__astCntrl); 

}

/*      Ast Node Value     */

parser::Ast_Node_Value::Ast_Node_Value(int _valuePos, int _tkId) 
    : Ast_Node(AST_NODE_VALUE), value_position(_valuePos), token_id(_tkId) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Value <--");

    parser::Ast_Node_Value* _ = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    int _valPos =  __astCntrl->storage->addNewValue(
        (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->phr, 1
    );

    new(_) parser::Ast_Node_Value(
        _valPos, (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)++]->id
    );

    return _;

}

int parser::Ast_Node_Value::getByteSize() { return parser_helper::getSizeImplicitValue(token_id); }

parser::Type_Information* parser::Ast_Node_Value::getType() {

    parser::Type_Information* _ = (parser::Type_Information*) malloc(sizeof(parser::Type_Information));

    new (_) parser::Type_Information(
        parser_helper::getTokenIdTypeFromTokenIdImplicitValue(token_id),
        NULL
    );

    return _;

}

/*      Ast Node Variable Declaration     */

parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete variable_type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* _type, int _declId) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), variable_type(_type), declaration_id(_declId) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* __astCntrl, Type_Information* _type) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _varDecl;
    parser::Token* _tk;
    int _declId;
    bool __;

    while((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id != TOKEN_ENDINSTRUCTION) {

        __astCntrl->printDebugInfo("--> Ast Node Variable Declaration <--");

        __astCntrl->current_block->addNewName(
            (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->phr, 1
        );

        _declId =  __astCntrl->current_block->getDeclarationId((*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->phr, __);

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(_type, _declId);

        _->add(_varDecl);

        _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];

        if (_tk->id == TOKEN_EQUAL) {

            __astCntrl->current_token_position--;

            _->add(
                parser::Ast_Node_Variable_Assignment::generate(
                    __astCntrl
                )
            );

        }
        
        if (_tk->id == TOKEN_COMMA) {

            __astCntrl->current_token_position++;

            _type = _type->generateDifferentPointersOperations(__astCntrl);

        }

    }

    return _;

}

int parser::Ast_Node_Variable_Declaration::getByteSize() { return variable_type->getByteSize(); }

parser::Type_Information* parser::Ast_Node_Variable_Declaration::getType(utils::LinkedList <Type_Information*>*) { return variable_type; }

/*      Ast Node Variable     */

parser::Ast_Node_Variable::Ast_Node_Variable(int _declId, bool _isGlbVar) : Ast_Node(AST_NODE_VARIABLE), declaration_id(_declId), is_global_variable(_isGlbVar) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Variable <--");

    parser::Ast_Node_Variable* _ = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));

    bool _isGlbVar = false;

    int _declId = __astCntrl->current_block->getDeclarationId(
        (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)++]->phr, _isGlbVar
    );

    if (_declId == -1) new Ast_Exception("No name declared with given name");

    new (_) parser::Ast_Node_Variable(
        _declId, _isGlbVar
    );

    return _;

}

int parser::Ast_Node_Variable::getByteSize() { if (variable_declaration) return variable_declaration->getByteSize(); return -1; }

/*      Ast Node Pointer Operators     */

parser::Ast_Node_Pointer_Operators::~Ast_Node_Pointer_Operators() { value->~Ast_Node(); free(value); }

parser::Ast_Node_Pointer_Operators::Ast_Node_Pointer_Operators(utils::LinkedList <int>* __oprs, Ast_Node* __value) 
    : Ast_Node(AST_NODE_POINTER_OPERATORS), value(__value), pntrLvl(0) {
        for (int _ = 0; _ < __oprs->count; _++)
            pntrLvl += ((*__oprs)[_] == TOKEN_POINTER) ? 1 : -1;
}

parser::Ast_Node_Pointer_Operators* parser::Ast_Node_Pointer_Operators::generate(Ast_Control* __astCntrl, utils::LinkedList <int>* __oprs) {

    __astCntrl->printDebugInfo("--> Ast Node Pointer Operators <--");

    parser::Ast_Node* _value = getValue(__astCntrl);

    if (!_value) new Ast_Exception("Pointer Operators error getting value inside");

    parser::Ast_Node_Pointer_Operators* _ = (parser::Ast_Node_Pointer_Operators*) malloc(sizeof(parser::Ast_Node_Pointer_Operators));
    new (_) parser::Ast_Node_Pointer_Operators(
        __oprs, _value
    );

    return _;

}

parser::Ast_Node* parser::Ast_Node_Pointer_Operators::getValue(Ast_Control* __astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Parenthesis //
    */

    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];

    if (_tk->id == TOKEN_IDENTIFIER) return parser::Ast_Node_Variable::generate(__astCntrl);

    if (parser::isImplicitValue(_tk)) return parser::Ast_Node_Value::generate(__astCntrl);

    new Ast_Exception("Ast Node Pointer Operators -> Something went wrong");

}

int parser::Ast_Node_Pointer_Operators::getTypeSize() { return 0; // TODO
    
}

/*      Ast Node Variable assignment     */

parser::Ast_Node_Variable_Assignment::~Ast_Node_Variable_Assignment() { 
    if (value_before_assign) value_before_assign->~Ast_Node(); free(value_before_assign);
    if (value) value->~Ast_Node_Expression(); free(value); 
}

parser::Ast_Node_Variable_Assignment::Ast_Node_Variable_Assignment(bool _isLft, Ast_Node* _valBef, int _expId, Ast_Node_Expression* _value) : 
    Ast_Node(AST_NODE_VARIABLE_ASSIGNMENT), operation_is_left(_isLft), value_before_assign(_valBef), expression_id(_expId), value(_value) {}

parser::Ast_Node_Variable_Assignment* parser::Ast_Node_Variable_Assignment::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Variable Assignment <--");

    parser::Ast_Node_Variable_Assignment* _ = (parser::Ast_Node_Variable_Assignment*) malloc(sizeof(parser::Ast_Node_Variable_Assignment));
    parser::Ast_Node_Expression* _value;
    parser::Ast_Node* _valBef;
    bool _isLft = false;

    parser::Token* _expTk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];

    if (parser::isAssignment(_expTk)) {
        if (!parser::isSingleAssignment(_expTk)) new Ast_Exception("Not single assignment not allowed before variable");
        __astCntrl->current_token_position++;
        _isLft = true;
    }

    _valBef = getValueBeforeAssign(__astCntrl);

    if (!_isLft) { _expTk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]; _value = getValue(__astCntrl); }
    else _value = NULL;

    new (_) parser::Ast_Node_Variable_Assignment(
        _isLft, _valBef, _expTk->id, _value
    );    

    return _;

}

parser::Ast_Node* parser::Ast_Node_Variable_Assignment::getValueBeforeAssign(Ast_Control* __astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Variable 
    *   Ast_Parenthesis //
    *   Ast_Pointer_Operators 
    */

    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];
    parser::Ast_Node* _;

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER: _ = parser::Ast_Node_Variable::generate(__astCntrl); break;
    case TOKEN_POINTER: case TOKEN_ADDRESS: 
        {

            utils::LinkedList <int>* _operators = new utils::LinkedList <int>();

            parser_helper::setPointerOperators(__astCntrl, _operators);

            _ = parser::Ast_Node_Pointer_Operators::generate(
                __astCntrl, _operators
            );

            delete _operators;

            break;

        }
    // case TOKEN_OPENPARENTHESES: return parser::Ast_Node_Parenthesis::generate(__astCntrl);

    default: goto exception;
    }

    return _;

exception:

    new Ast_Exception("Ast Node Variable Assignment get value before assignment -> Something went wrong");

    return NULL;

}

parser::Ast_Node_Expression* parser::Ast_Node_Variable_Assignment::getValue(Ast_Control* __astCntrl) { return parser::Ast_Node_Expression::generate(__astCntrl); }

/*      Ast Node Parenthesis     */

parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() { if (value) value->~Ast_Node_Expression(); free(value); }

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node_Expression* _value) : Ast_Node(AST_NODE_PARENTHESIS), value(_value) {}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Generate Parenthesis <--");

    parser::Ast_Node_Parenthesis* _ = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));

    __astCntrl->current_token_position++;

    new (_) parser::Ast_Node_Parenthesis(
        parser::Ast_Node_Expression::generate(__astCntrl)
    );

    __astCntrl->printDebugInfo("--> End Parenthesis <--");

    __astCntrl->current_token_position++;

    return _;

}

int parser::Ast_Node_Parenthesis::getByteSize() { return value->getByteSize(); }

/*      Ast Node Function Declaration     */

parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() {
    delete return_type;
    delete parameters;
    // if (body) body->~Ast_Node_Code_Block(); free(body); is in code_blocks field so no need to free here
}       

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    Type_Information* _rtrType, int _declId, utils::LinkedList <Ast_Node*>* _params, Ast_Node_Code_Block* _body) : Ast_Node(AST_NODE_FUNCTION_DECLARATION), return_type(_rtrType),
        declaration_id(_declId), parameters(_params), body(_body) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* __astCntrl, Type_Information* _rtrType) {

    __astCntrl->printDebugInfo("--> Node Function Declaration <--");

    parser::Ast_Node_Function_Declaration* _ = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    parser::Ast_Node_Code_Block* _body = NULL;
    int _declId;

    __astCntrl->current_block->addNewName(
        (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->phr, 1
    );

    bool _glb;
    _declId = __astCntrl->current_block->getDeclarationId((*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->phr, _glb);

    utils::LinkedList <char*>* _namesDeclrs = new utils::LinkedList <char*>();

    utils::LinkedList <parser::Ast_Node*>* _params = getParameters(__astCntrl, _namesDeclrs);

    if ((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id == TOKEN_OPENCURLYBRACKET) {

        __astCntrl->current_token_position++;
        _body = parser::Ast_Node_Code_Block::generate(__astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMEMT_FUNCTION, _namesDeclrs);

    }

    new (_) parser::Ast_Node_Function_Declaration(
        _rtrType, _declId, _params, _body
    );

    return _;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(Ast_Control* __astCntrl, utils::LinkedList <char*>* __nmsDecl) {

    __astCntrl->printDebugInfo("--> Ast Node Function Parameters <--");
    utils::LinkedList <Ast_Node*>* _ = new utils::LinkedList <Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _varDecl;
    Type_Information* _typeInfo;
    int _declId;
    char* _name;

    __astCntrl->current_token_position++;

    while((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id != TOKEN_CLOSEPARENTHESES) {

        __astCntrl->printDebugInfo("--> Ast Node Function Parameter Added <--");

        _typeInfo = Type_Information::generate(__astCntrl);

        switch ((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id)
        {
        case TOKEN_COMMA:
            __astCntrl->current_token_position++;
        case TOKEN_CLOSEPARENTHESES:        
            {
                _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
                new (_varDecl) parser::Ast_Node_Variable_Declaration(
                    _typeInfo, -1
                );

                _->add(_varDecl);

                continue;
            }
 
        default: break;
        }

        if (
            __nmsDecl->getObjectPosition(
                (*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->phr, NULL
            ) != -1
        ) new Ast_Exception("Redefinition of name in function scope");

        _name = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->phr;

        _name = utils::copyString(
            _name, 
            utils::getStringSize(_name)
        );

        _declId = __nmsDecl->add(
            _name
        );

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(
            _typeInfo, _declId
        );

        _->add(
            _varDecl
        );

        if (
            (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->id == TOKEN_EQUAL
        ) { new Ast_Exception("TODO get parameters function"); }

        if (
            (*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->id == TOKEN_COMMA
        ) __astCntrl->current_token_position ++;

    }

    __astCntrl->current_token_position++;

    return _;

}

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }

/*      Ast Node Function Call     */

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(int _declId, utils::LinkedList <Ast_Node*>* _params, bool _glb) :
    Ast_Node(AST_NODE_FUNCTION_CALL), declaration_id(_declId), parameters(_params), is_global_function(_glb) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Function Call <--");

    parser::Ast_Node_Function_Call* _ = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));

    int _declId;
    bool _glb;

    if (
        (_declId = __astCntrl->current_block->getDeclarationId((*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->phr, _glb))
        == -1
    ) new Ast_Exception("No declaration found with given name");

    new (_) parser::Ast_Node_Function_Call(
        _declId, getFunctionCallParameters(__astCntrl), _glb
    );

    return _;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getFunctionCallParameters(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Function Call Parameters <--");

    utils::LinkedList <Ast_Node*>* _parameters = new utils::LinkedList <Ast_Node*>();

    __astCntrl->current_token_position++;

    while((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id != TOKEN_CLOSEPARENTHESES) {

        __astCntrl->printDebugInfo("--> Ast Node Function Call Parameter Added <--");

        _parameters->add(
            parser::Ast_Node_Expression::generate(
                __astCntrl
            )
        );

        if ((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id == TOKEN_COMMA) (__astCntrl->current_token_position)++;

    }

    __astCntrl->current_token_position++;

    return _parameters;


}

int parser::Ast_Node_Function_Call::getByteSize() { if (function_declaration) return function_declaration->getByteSize(); return -1; }

/*      Ast Node Struct Declaration     */

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(
    int _declId, bool _isCntrc) : Ast_Node(AST_NODE_STRUCT_DECLARATION), declaration_id(_declId), is_contract(_isCntrc) {
        functions = new utils::LinkedList <Ast_Node_Function_Declaration*>();
        fields = new utils::LinkedList <Ast_Node*>();
        body_info = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));
}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Struct Declaration <--");

    parser::Ast_Node_Struct_Declaration* _ = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));
    bool _isCntr = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->id == TOKEN_CONTRACT;
    int _declId;
    bool _gbl;

    parser::Token* _tk = (*__astCntrl->tokens_collection)[__astCntrl->current_token_position];

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER:

        __astCntrl->current_block->addNewName(
            _tk->phr, 1
        );

        _declId = __astCntrl->current_block->getDeclarationId(
            _tk->phr, _gbl
        );

        __astCntrl->current_token_position++;

        break;

    case TOKEN_OPENCURLYBRACKET: new Ast_Exception("TODO struct no identifier at begining");
    default: break;
    }

    new (_) parser::Ast_Node_Struct_Declaration(
        _declId, _isCntr
    );

    new (_->body_info) Ast_Node_Code_Block(AST_NODE_CODE_BLOCK_ENVIRONMEMT_STRUCT, NULL, __astCntrl->current_block, 0);

    __astCntrl->current_block = _->body_info;

    _->getFields(__astCntrl);
    _->getFunctions(__astCntrl);

    __astCntrl->current_block = _->body_info->previous_block;

    return _;

}

void parser::Ast_Node_Struct_Declaration::getFields(Ast_Control* __astCntrl) {

    std::cout << "Fields" << std::endl;

}

void parser::Ast_Node_Struct_Declaration::getFunctions(Ast_Control* __astCntrl) {
    std::cout << "Functions" << std::endl;

} 