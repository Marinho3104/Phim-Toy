#include "./ast_nodes.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __id) : node_id(__id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { if (declarations) delete declarations;  }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __nmSpc, utils::LinkedList <Ast_Node*>* __decl) 
    : Ast_Node(AST_NODE_NAME_SPACE), name_space(__nmSpc), declarations(__decl) {}

void parser::Ast_Node_Name_Space::generate(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    __astCntrl->printDebugInfo("--> Ast Node Name Space <--");

    parser::Ast_Node_Name_Space* _ = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));

    Name_Space* _prevNameSpace = __astCntrl->current_name_space;

    __astCntrl->current_name_space = __nmSpc;

    utils::LinkedList <parser::Ast_Node*>* _declrs = getDeclarations(__astCntrl);

    new (_) parser::Ast_Node_Name_Space(
        __nmSpc, _declrs
    );

    __astCntrl->current_name_space = _prevNameSpace;

    __astCntrl->name_spaces->add(_);

    __astCntrl->printDebugInfo("--> Ast Node Name Space End <--");

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarations(Ast_Control* __astCntrl) {

    utils::LinkedList <Ast_Node*>* _body, *body = new utils::LinkedList <Ast_Node*>();
    bool _sts;

    while(__astCntrl->current_token_position < __astCntrl->tokens_collection->count) {
        
        _body = getDeclarationsSingle(__astCntrl);

        _sts = body->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

    return body;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarationsSingle(Ast_Control* __astCntrl) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Token* _tk = __astCntrl->getToken(0);
    parser::Token* _tkNext = __astCntrl->getToken(1);

    // std::cout << "Token 1 -> " << _tk->id << std::endl;
    // std::cout << "Token 2 -> " << _tkNext->id << std::endl;

    switch (_tk->id)
    {
        case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
        case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getDeclarationsSingle(__astCntrl); break;
        case TOKEN_NAMESPACE:
            {
                __astCntrl->current_token_position++;

                Name_Space* _nameSpace = Name_Space::getNameSpace(
                    __astCntrl
                );

                __astCntrl->current_token_position++;

                Ast_Node_Name_Space::generate(
                    __astCntrl, _nameSpace
                );

                delete _;

                _ = getDeclarationsSingle(__astCntrl);

                break;

            }
        default: goto cont;
    }

    return _;

cont:

    if (parser::isPrimativeType(_tk)) {
        Type_Information* _typeInformation = Type_Information::generate(__astCntrl);

        switch (__astCntrl->getToken(1)->id)
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
    
    else new Ast_Execption("Declaration not allowed");

    return _;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete body; delete name_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>* _body) : Ast_Node(AST_NODE_CODE_BLOCK) {}

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* _prevBlock, Name_Space* _nameSpace, int _tp) 
    : Ast_Node(AST_NODE_CODE_BLOCK), previous_block(_prevBlock), name_space(_nameSpace), declaration_type(_tp) {
        body = new utils::LinkedList <Ast_Node*>();
        name_tracker = new Name_Tracker();
        count_off = getCountOffBefore();
    }

void parser::Ast_Node_Code_Block::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Block Code <--");

    __astCntrl->current_code_block->generateBody(__astCntrl);

    __astCntrl->current_code_block = __astCntrl->current_code_block->previous_block;    

    __astCntrl->printDebugInfo("--> Ast Node Block Code End <--");

}

void parser::Ast_Node_Code_Block::setUp(Ast_Control* __astCntrl, int __tp) {

    __astCntrl->printDebugInfo("--> Set Code Block Up <--");

    parser::Ast_Node_Code_Block* _  = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));

    new (_) parser::Ast_Node_Code_Block(
        __astCntrl->current_code_block, __astCntrl->current_name_space, __tp
    );

    __astCntrl->current_code_block = _;

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

    switch (__astCntrl->getToken(0)->id)
    {
    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
    case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getNewNodes(__astCntrl); break;
    default: goto cont;
    }

    return _;

cont:

    if (parser::isPrimativeType(__astCntrl->getToken(0))) {

        Type_Information* _typeInformation = Type_Information::generate(__astCntrl);

        if ((*__astCntrl->tokens_collection)[__astCntrl->current_token_position]->id != TOKEN_IDENTIFIER) 
            { delete _typeInformation; __astCntrl->current_token_position--; goto expressionGen; }

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
            Ast_Node_Expression::generate(
                __astCntrl
            )
        );

    }

    return _;

}

int parser::Ast_Node_Code_Block::getCountOffBefore() { return name_tracker->names_declared->count + previous_block ? previous_block->getCountOffBefore() : 0; }

int parser::Ast_Node_Code_Block::getDeclarationId(char* __n) {

    int _;

    if (
        (_ = name_tracker->getDeclarationId(__n)) == -1
    )

        if (previous_block) _ = previous_block->getDeclarationId(__n);

        else return name_space->getDeclarationId(__n);

    return _ + count_off;

}

bool parser::Ast_Node_Code_Block::isDeclarationIdGlobal(char* __n) {

    if (
        name_tracker->getDeclarationId(__n) == -1
    )

        if (previous_block) return previous_block->isDeclarationIdGlobal(__n);

        else return true;

    return false;

}



parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete variable_type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* __varTp, int __declId) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), variable_type(__varTp), declaration_id(__declId) {
        
    }

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* __astCntrl, Type_Information* __varTp) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _varDecl;
    int _declId;

    while(__astCntrl->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        __astCntrl->printDebugInfo("--> Ast Node Variable Declaration <--");

        _declId =  addToCorrectNameTracker(__astCntrl);

        __astCntrl->current_token_position++;

        std::cout << "Declaration id -> " << _declId << std::endl;

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(__varTp, _declId);

        _->add(_varDecl);

        if (__astCntrl->getToken(0)->id == TOKEN_EQUAL) {
            new Ast_Execption("TODO ");
        }

        if (__astCntrl->getToken(0)->id == TOKEN_COMMA) {
            __astCntrl->current_token_position++;
            __varTp = Type_Information::generate(__astCntrl, __varTp);
        }

    }

    return _;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Function Parameter Variable Declaration <--");

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    Ast_Node_Variable_Declaration* _varDecl;
    Type_Information* _type;
    int _declId;

    _type = Type_Information::generate(__astCntrl);
        
    switch ((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id)
    {
    case TOKEN_COMMA: case TOKEN_CLOSEPARENTHESES:        
        {
            _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
            new (_varDecl) parser::Ast_Node_Variable_Declaration(
                _type, -1
            );

            _->add(_varDecl);

            return _;
        }
    default: break;
    }

    __astCntrl->current_code_block->name_tracker->addNewName(
        __astCntrl->getToken(0)->phr, 1
    );

    _declId = __astCntrl->current_code_block->getDeclarationId(
        __astCntrl->getToken(0)->phr
    );
    __astCntrl->current_token_position++;

    std::cout << "Declaration id -> " << _declId << std::endl;

    _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
    new (_varDecl) parser::Ast_Node_Variable_Declaration(
        _type, _declId
    );
    _->add(_varDecl);

    if (__astCntrl->getToken(0)->id == TOKEN_EQUAL) { new Ast_Execption("TODO"); }

    return _;

}

int parser::Ast_Node_Variable_Declaration::addToCorrectNameTracker(Ast_Control* __astCntrl) {

    int _declId;

    if (__astCntrl->current_code_block) {

        __astCntrl->current_code_block->name_tracker->addNewName(
            __astCntrl->getToken(0)->phr, 1
        );

        _declId =__astCntrl->current_code_block->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

    } else {

        __astCntrl->current_name_space->addNewName(
            __astCntrl->getToken(0)->phr
        );

        _declId =__astCntrl->current_name_space->getDeclarationId(
            __astCntrl->getToken(0)->phr
        );

    }

    return _declId;

}


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() { 
    delete parameters; 
    if (function_body) function_body->~Ast_Node_Code_Block(); free(function_body); 
    delete return_type; 
}

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(utils::LinkedList <Ast_Node*>* _params, Ast_Node_Code_Block* _body, Type_Information* _rtrTp, int _declarationId)
    : Ast_Node(AST_NODE_FUNCTION_DECLARATION), parameters(_params), function_body(_body), return_type(_rtrTp), declaration_id(_declarationId) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Function Parameters <--");
    utils::LinkedList <Ast_Node*>* _params = new utils::LinkedList <Ast_Node*>(), *_t;

    while(__astCntrl->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _t = Ast_Node_Variable_Declaration::generateFunctionParameter(__astCntrl);

        _params->join(_t);

        _t->destroy_content = 0; delete _t;

        if (__astCntrl->getToken(0)->id == TOKEN_COMMA) __astCntrl->current_token_position++;

    }

    __astCntrl->current_token_position++;

    return _params;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* __astCntrl, Type_Information* __rtrTp) {

    __astCntrl->printDebugInfo("--> Node Function Declaration <--");

    parser::Ast_Node_Function_Declaration* _funcDecl = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    Ast_Node_Code_Block* function_body;
    int _declId;

    __astCntrl->current_name_space->addNewName(
        __astCntrl->getToken(0)->phr
    );

    _declId = __astCntrl->current_name_space->getDeclarationId(
        __astCntrl->getToken(0)->phr
    );

    __astCntrl->current_token_position++;

    Ast_Node_Code_Block::setUp(__astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_FUNCTION);
    function_body = __astCntrl->current_code_block;

    __astCntrl->current_token_position++;

    utils::LinkedList <parser::Ast_Node*>* _params = getParameters(__astCntrl);

    if (__astCntrl->getToken(0)->id == TOKEN_ENDINSTRUCTION) {

        __astCntrl->current_code_block = function_body->previous_block;

        function_body->~Ast_Node_Code_Block(); free(function_body); function_body = NULL;

    } else { __astCntrl->current_token_position++; Ast_Node_Code_Block::generate(__astCntrl); }

    new (_funcDecl) parser::Ast_Node_Function_Declaration(
        _params, function_body, __rtrTp, _declId
    );

    return _funcDecl;

}


parser::Ast_Node_Expression::~Ast_Node_Expression() { if (first) first->~Ast_Node(); free(first); if (second) second->~Ast_Node_Expression(); free(second); } 

parser::Ast_Node_Expression::Ast_Node_Expression(int _expId, Ast_Node* _frst, Ast_Node_Expression* _scnd) 
    : Ast_Node(AST_NODE_EXPRESSION), expression_id(_expId), first(_frst), second(_scnd) {}

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
        _scndValue ? _expId : -1, _frstValue, _scndValue
    );

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

    switch (__astCntrl->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER: _ = parser::Ast_Node_Variable::generate(__astCntrl); break;
    default: goto cont;
    }

    return _;

cont:

    if (parser::isImplicitValue(__astCntrl->getToken(0))) return parser::Ast_Node_Value::generate(__astCntrl);

    new Ast_Execption("Ast Node Expression get first -> Something went wrong");

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::getSecondNode(Ast_Control* __astCntrl) {

    switch (__astCntrl->getToken(0)->id)
    {
    case TOKEN_ENDINSTRUCTION: case TOKEN_CLOSEPARENTHESES: case TOKEN_COMMA: return NULL;
    default: __astCntrl->current_token_position++; break;
    }
    
    return parser::Ast_Node_Expression::generate(__astCntrl); 

}


parser::Ast_Node_Value::~Ast_Node_Value() {} 

parser::Ast_Node_Value::Ast_Node_Value(int _valPos, int _tkId) : Ast_Node(AST_NODE_VALUE), value_position(_valPos), token_id(_tkId) {}

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


parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(int _declId, bool _isGbl) : Ast_Node(AST_NODE_VARIABLE), declaration_id(_declId), is_global(_isGbl) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Variable <--");
    
    parser::Ast_Node_Variable* _ = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));
    
    int _declId = __astCntrl->current_code_block->getDeclarationId(
        __astCntrl->getToken(0)->phr
    );

    if (_declId == -1) new Ast_Execption("No name declared with given name");

    bool _isGbl = __astCntrl->current_code_block->isDeclarationIdGlobal(
        __astCntrl->getToken(0)->phr
    );

    __astCntrl->current_token_position++;

    new (_) parser::Ast_Node_Variable(
        _declId, _isGbl
    );

    std::cout << "Variable is " << ((_isGbl) ? "Global" : "Local") << std::endl;

    return _;

}


