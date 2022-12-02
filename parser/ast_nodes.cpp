#include "./ast_nodes.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./parser_helper.h"
#include "./ast_helper.h"
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

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generateByOperator(Ast_Control* __astCntrl, Name_Space* __nmSpc) {
    new Ast_Execption("TODO ast node name space");



}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarations(Ast_Control* __astCntrl) {

    utils::LinkedList <Ast_Node*>* _body, *body = new utils::LinkedList <Ast_Node*>();
    bool _sts;

    while(__astCntrl->current_token_position < __astCntrl->tokens_collection->count) {
        
        _body = getDeclarationsSingle(__astCntrl, NULL);

        _sts = body->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

    return body;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getDeclarationsSingle(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();
    parser::Token* _tk = __astCntrl->getToken(0);
    parser::Token* _tkNext = __astCntrl->getToken(1);
    Name_Space* _name_space;

    // std::cout << "Token 1 -> " << _tk->id << std::endl;
    // std::cout << "Token 2 -> " << _tkNext->id << std::endl;

    if(
        (_name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL))
    ) { delete _; return getDeclarationsSingle(__astCntrl, _name_space); }

    switch (_tk->id)
    {
        case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
        case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getDeclarationsSingle(__astCntrl, __nmSpc); break;
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

                _ = getDeclarationsSingle(__astCntrl, __nmSpc);

                break;

            }
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

    if (parser::isPrimativeType(_tk) || _tk->id == TOKEN_IDENTIFIER) {

        Type_Information* _typeInformation = Type_Information::generate(__astCntrl, __nmSpc);

        if (__astCntrl->getToken(0)->id != TOKEN_IDENTIFIER && __astCntrl->getToken(0)->id != TOKEN_NAMESPACE_OPERATOR) 
            new Ast_Execption("Declaration not allowed - Name Space Space - Idententifier");

        int _strctDecl = -1;

        __nmSpc = Name_Space::checkIfNameSpace(__astCntrl, &_strctDecl);

        switch (__astCntrl->getToken(1)->id)
        {
        case TOKEN_OPENPARENTHESES: 
        
            _->add(
                parser::Ast_Node_Function_Declaration::generate(
                    __astCntrl, _typeInformation, __nmSpc, _strctDecl 
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
    
    else new Ast_Execption("Declaration not allowed - Name Space Space");

    return _;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete body; delete name_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>* _body) : Ast_Node(AST_NODE_CODE_BLOCK) {}

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* _prevBlock, Name_Space* _nameSpace, int _tp) 
    : Ast_Node(AST_NODE_CODE_BLOCK), previous_block(_prevBlock), name_space(_nameSpace), declaration_type(_tp) {
        body = new utils::LinkedList <Ast_Node*>();
        name_tracker = new Name_Tracker(NULL);
        name_tracker->count = _prevBlock ? _prevBlock->name_tracker->count : 0;
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
        
        _body = getNewNodes(__astCntrl, NULL);

        _sts = body->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Code_Block::getNewNodes(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    utils::LinkedList <parser::Ast_Node*>* _ = new utils::LinkedList <parser::Ast_Node*>();

    parser::Token* _tk = __astCntrl->getToken(0);
    Name_Space* _name_space;

    if(
        (_name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL))
    ) { delete _; return getNewNodes(__astCntrl, _name_space); }

    switch (__astCntrl->getToken(0)->id)
    {
    case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __astCntrl->current_token_position++; break;
    case TOKEN_ENDINSTRUCTION: delete _; __astCntrl->current_token_position++; _ = getNewNodes(__astCntrl, _name_space); break;
    case TOKEN_OPENCURLYBRACKET: 
        __astCntrl->current_token_position++; setUp(__astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_SINGLE); _->add(__astCntrl->current_code_block);
        generate(__astCntrl);  break;
    default: goto cont;
    }

    return _;

cont:

    if (parser::isPrimativeType(_tk) || _tk->id == TOKEN_IDENTIFIER) {

        int _inicial_point = __astCntrl->current_token_position;

        Type_Information* _typeInformation = Type_Information::generate(__astCntrl, __nmSpc);

        _name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL);

        switch (__astCntrl->getToken(1)->id)
        {
        case TOKEN_OPENPARENTHESES: 
        
            _->add(
                parser::Ast_Node_Function_Declaration::generate(
                    __astCntrl, _typeInformation, _name_space, -1
                )
            );

            break;        
        case TOKEN_EQUAL: case TOKEN_COMMA: case TOKEN_ENDINSTRUCTION: 

            delete _;

            _ = Ast_Node_Variable_Declaration::generate(
                    __astCntrl, _typeInformation
                );

            break;
        default:

            __astCntrl->current_token_position = _inicial_point;
            delete _typeInformation;
            goto expressionGen;
            break;

        }

    } 
    
    else {

    expressionGen:
        _->add(
            Ast_Node_Expression::generate(
                __astCntrl, __nmSpc
            )
        );

    }

    return _;

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __n) {

    int _;

    if (
        (_ = name_tracker->getDeclarationId(__n)) == -1
    )

        if (previous_block && previous_block->declaration_type != AST_NODE_CODE_BLOCK_ENVIRONMENT_STRUCT) _ = previous_block->getDeclarationId(__n);

        else return name_space->getDeclarationId(__n);

    return _;

}

bool parser::Ast_Node_Code_Block::isDeclarationIdGlobal(char* __n) {

    if (
        name_tracker->getDeclarationId(__n) == -1
    )

        if (previous_block  && previous_block->declaration_type != AST_NODE_CODE_BLOCK_ENVIRONMENT_STRUCT) return previous_block->isDeclarationIdGlobal(__n);

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

    __varTp->name_space = __astCntrl->current_name_space;

    while(__astCntrl->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        __astCntrl->printDebugInfo("--> Ast Node Variable Declaration <--");

        _declId =  Name_Tracker::addToCorrectNameTracker(__astCntrl);

        __astCntrl->current_token_position++;

        std::cout << "Declaration id -> " << _declId << std::endl;

        _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(__varTp, _declId);

        _->add(_varDecl);

        if (__astCntrl->getToken(0)->id == TOKEN_EQUAL) {
            
            __astCntrl->current_token_position--;

            _->add(
                Ast_Node_Expression::generate(
                    __astCntrl, NULL
                )
            );

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

    Name_Space* _name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL);

    _type = Type_Information::generate(__astCntrl, _name_space);
        
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

    if (__astCntrl->getToken(0)->id == TOKEN_EQUAL) {

        __astCntrl->current_token_position--;

        _->add(
            Ast_Node_Expression::generate(
                __astCntrl, NULL
            )
        );

    }

    return _;

}

parser::Type_Information* parser::Ast_Node_Variable_Declaration::getType() { return variable_type; }

int parser::Ast_Node_Variable_Declaration::getByteSize() { return variable_type->getByteSize(); }


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

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* __astCntrl, Type_Information* __rtrTp, Name_Space* __nmSpc, int _structDeclId) {

    __astCntrl->printDebugInfo("--> Node Function Declaration <--\n");

    if (__nmSpc) {

        __astCntrl->saveState();

        __astCntrl->current_code_block = NULL;
        __astCntrl->current_name_space = __nmSpc;

    }

    parser::Ast_Node_Function_Declaration* _funcDecl = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    Ast_Node_Code_Block* function_body;
    int _declId;

    _declId = Name_Tracker::addToCorrectNameTracker(__astCntrl);

    __astCntrl->current_token_position++;

    Ast_Node_Code_Block::setUp(__astCntrl, AST_NODE_CODE_BLOCK_ENVIRONMENT_FUNCTION);
    function_body = __astCntrl->current_code_block;

    __astCntrl->current_token_position++;

    utils::LinkedList <parser::Ast_Node*>* _params = new utils::LinkedList <parser::Ast_Node*>();

    if (_structDeclId != -1) {

        __astCntrl->current_code_block->name_tracker->addNewName(
            (char*) "this", 1
        );

        int _thisDeclId = __astCntrl->current_code_block->getDeclarationId("this");

        utils::LinkedList <int>* _oprs = new utils::LinkedList <int>(); _oprs->add(TOKEN_POINTER);

        Type_Information* _thisType = new Type_Information(__astCntrl, TOKEN_IDENTIFIER, _structDeclId, _oprs, NULL); // Should be pointer TODO

        delete _oprs;

        parser::Ast_Node_Variable_Declaration* _varDecl = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_varDecl) parser::Ast_Node_Variable_Declaration(
            _thisType, _thisDeclId
        );

        _params->add(_varDecl);

        std::cout << "This declaration id -> " << _thisDeclId << std::endl;

    }

    utils::LinkedList <parser::Ast_Node*>* __t = getParameters(__astCntrl);

    _params->join(
        __t
    );

    __t->destroy_content = 0; delete __t;

    if (__astCntrl->getToken(0)->id == TOKEN_ENDINSTRUCTION) {

        __astCntrl->current_code_block = function_body->previous_block;

        function_body->~Ast_Node_Code_Block(); free(function_body); function_body = NULL; __astCntrl->current_token_position++;

    } else { __astCntrl->current_token_position++; Ast_Node_Code_Block::generate(__astCntrl); }

    new (_funcDecl) parser::Ast_Node_Function_Declaration(
        _params, function_body, __rtrTp, _declId
    );

    // __astCntrl->current_code_block = __astCntrl->current_code_block->previous_block;

    std::cout << "Function declaration id - >" << _declId << std::endl;
    // std::cout << "Function parameters count - >" << _params->count << std::endl;

    __astCntrl->printDebugInfo("--> Node Function Declaration End <--\n");

    return _funcDecl;

}

parser::Type_Information* parser::Ast_Node_Function_Declaration::getType() { return return_type; }

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }


parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() { if (own_name_space) own_name_space->~Name_Space(); free(own_name_space); delete fields; delete functions; delete struct_type; }

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(
    int __declId, bool __cntr, Name_Space* _ownNameSpace, Type_Information* _strcType, utils::LinkedList <Ast_Node*>* __fields, utils::LinkedList <Ast_Node_Function_Declaration*>* __funcs) 
        : Ast_Node(AST_NODE_STRUCT_DECLARATION), declaration_id(__declId), is_contract(__cntr), own_name_space(_ownNameSpace), struct_type(_strcType), fields(__fields), functions(__funcs) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Struct Declaration <--");
    utils::LinkedList <parser::Ast_Node_Function_Declaration*>* _functions = NULL;
    bool _isCntr = __astCntrl->getToken(0)->id == TOKEN_CONTRACT;
    utils::LinkedList <parser::Ast_Node*>* _fields = NULL;
    Name_Space* _struct_name_space;
    int _declId, _inicial_position;
    Type_Information* _strcType;

    __astCntrl->current_token_position++;

    if (__astCntrl->getToken(0)->id == TOKEN_IDENTIFIER) {

        _declId = Name_Tracker::addToCorrectNameTracker(__astCntrl);

        __astCntrl->current_token_position++;

    } else if (__astCntrl->getToken(0)->id == TOKEN_OPENCURLYBRACKET) {
        new Ast_Execption("TODO struct no identifier at begining");
    } else new Ast_Execption("Unexpected token");

    _struct_name_space = __astCntrl->name_space_control->getNameSpaceStruct();

    __astCntrl->struct_name_space = _struct_name_space;

    if (__astCntrl->getToken(0)->id == TOKEN_OPENCURLYBRACKET) {

        _inicial_position = __astCntrl->current_token_position;

        _fields = getFields(__astCntrl);

        __astCntrl->current_token_position = _inicial_position;

        _functions = getFunctions(__astCntrl, _declId);

        __astCntrl->current_token_position += 2;

        _strcType = new Type_Information(__astCntrl, TOKEN_IDENTIFIER, _declId, NULL, NULL);

    }
    
    else if (__astCntrl->getToken(0)->id == TOKEN_ENDINSTRUCTION);

    else new Ast_Execption("Unexpected token in struct declaration");
    
    parser::Ast_Node_Struct_Declaration* _strctDecl = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));

    new (_strctDecl) parser::Ast_Node_Struct_Declaration(
        _declId, _isCntr, _struct_name_space, _strcType, _fields, _functions
    );

    __astCntrl->struct_name_space = NULL;

    std::cout << "Struct fields count -> " << _fields->count << std::endl;
    std::cout << "Struct functions count -> " << _functions->count << std::endl;
    std::cout << "Struct id -> " << _declId << std::endl;

    return _strctDecl;

}

void parser::Ast_Node_Struct_Declaration::ignoreCodeBlock(Ast_Control* __astCntrl) {

    __astCntrl->current_token_position++;

    while(__astCntrl->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        if (__astCntrl->getToken(0)->id == TOKEN_OPENCURLYBRACKET) ignoreCodeBlock(__astCntrl);

        __astCntrl->current_token_position++;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Struct_Declaration::getFields(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Struct Fields <--\n");

    utils::LinkedList <parser::Ast_Node*>* _fields = new utils::LinkedList <parser::Ast_Node*>(), *_varDecl;
    Type_Information* _type;
    Name_Space* _name_space;

    __astCntrl->current_token_position++;

    while(__astCntrl->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        _name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL);

        _type = Type_Information::generate(__astCntrl, _name_space);

        if (__astCntrl->getToken(1)->id == TOKEN_OPENPARENTHESES) {

            delete _type;

            // Until it reaches the end of scope ')'
            while(__astCntrl->getToken(0)->id != TOKEN_CLOSEPARENTHESES) __astCntrl->current_token_position++;

            switch (__astCntrl->getToken(1)->id)
            {
            case TOKEN_ENDINSTRUCTION: __astCntrl->current_token_position+=2; break;
            case TOKEN_OPENCURLYBRACKET: __astCntrl->current_token_position++; ignoreCodeBlock(__astCntrl); __astCntrl->current_token_position++; break;
            default: new Ast_Execption("Unexpected token at struct body");
            }

            continue;

        }

        _varDecl = Ast_Node_Variable_Declaration::generate(
            __astCntrl, _type
        );

        __astCntrl->current_token_position++;

        _fields->join(_varDecl);

        _varDecl->destroy_content = 0; delete _varDecl;

    }

    return _fields;

}

utils::LinkedList <parser::Ast_Node_Function_Declaration*>* parser::Ast_Node_Struct_Declaration::getFunctions(Ast_Control* __astCntrl, int _declId) {

    __astCntrl->printDebugInfo("--> Ast Node Struct Functions <--\n");

    utils::LinkedList <parser::Ast_Node_Function_Declaration*>* _functions = new utils::LinkedList <parser::Ast_Node_Function_Declaration*>();
    Type_Information* _type;
    Name_Space* _name_space;

    __astCntrl->current_token_position++;

    while(__astCntrl->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        _name_space = Name_Space::checkIfNameSpace(__astCntrl, NULL);

        _type = Type_Information::generate(__astCntrl, _name_space);

        if ((*__astCntrl->tokens_collection)[__astCntrl->current_token_position + 1]->id != TOKEN_OPENPARENTHESES) {

            delete _type;

            // Until it reaches the end of of instruction ';'
            while((*__astCntrl->tokens_collection)[__astCntrl->current_token_position++]->id != TOKEN_ENDINSTRUCTION);

            continue;

        }

        _functions->add(
            parser::Ast_Node_Function_Declaration::generate(
                __astCntrl, _type, NULL, _declId
            )
        );

        // __astCntrl->current_token_position++;

    }

    return _functions;

}

parser::Type_Information* parser::Ast_Node_Struct_Declaration::getType() { return struct_type; }

int parser::Ast_Node_Struct_Declaration::getByteSize() {

    int _byte_size = 0;

    for (int _ = 0; _ < fields->count; _++) 
    
        if ((*fields)[_]->node_id != AST_NODE_VARIABLE_DECLARATION) continue;

        else _byte_size += ((Ast_Node_Variable_Declaration*)(*fields)[_])->getByteSize();

    return _byte_size;

}


parser::Ast_Node_Expression::~Ast_Node_Expression() { if (first) first->~Ast_Node(); free(first); if (second) second->~Ast_Node_Expression(); free(second); } 

parser::Ast_Node_Expression::Ast_Node_Expression(int _expId, Ast_Node* _frst, Ast_Node_Expression* _scnd) 
    : Ast_Node(AST_NODE_EXPRESSION), expression_id(_expId), first(_frst), second(_scnd) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    __astCntrl->printDebugInfo("--> Ast Node Expression <--");

    parser::Ast_Node_Expression* _ = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    parser::Ast_Node* _frstValue;
    parser::Ast_Node_Expression*_scndValue;
    parser::Token* _expTk;


    _frstValue = getFirstNode(__astCntrl, __nmSpc);
    _expTk = __astCntrl->getToken(0);

    
    if (parser::isExpressionOperator(_expTk)) { __astCntrl->current_token_position++; _scndValue = getSecondNode(__astCntrl); }

    else _scndValue = NULL;

    new (_) parser::Ast_Node_Expression(
        _scndValue ? _expTk->id : -1, _frstValue, _scndValue
    );

    return _;

}

parser::Ast_Node* parser::Ast_Node_Expression::getFirstNode(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Node_Parenthesis
    *   Ast_Node_Function_Call
    *   Ast_Node_Pointer_Operations
    */

    parser::Ast_Node* _;

    if (!__nmSpc) __nmSpc = Name_Space::checkIfNameSpace(__astCntrl, NULL);

    if (__nmSpc) __nmSpc->print();

    switch (__astCntrl->getToken(0)->id)
    {

        case TOKEN_IDENTIFIER:
                if (__astCntrl->getToken(1)->id == TOKEN_OPENPARENTHESES) _ = parser::Ast_Node_Function_Call::generate(__astCntrl, __nmSpc);

                else { // Normal variable load 

                    _ = parser::Ast_Node_Variable::generate(__astCntrl, __nmSpc);

                    switch (__astCntrl->getToken(0)->id)
                    {
                    case TOKEN_EQUAL: _ = Ast_Node_Variable_Assignment::generate(__astCntrl, _); break;
                    // case TOKEN_ACCESSINGVARIABLE: case TOKEN_ACCESSINGVARIABLEPOINTER:
                    //     _ = Ast_Node_Accessing::generate(__astCntrl, _); break;
                    default: break;
                    }

                }

                goto rtr;

        case TOKEN_ADDRESS: case TOKEN_POINTER:

            {
                utils::LinkedList <int>* _operators = new utils::LinkedList <int>();
                parser_helper::setPointerOperators(__astCntrl, _operators, 0);

                _ = parser::Ast_Node_Pointer_Operators::generate(
                    __astCntrl, _operators
                );

                delete _operators;

 
                if (__astCntrl->getToken(0)->id == TOKEN_EQUAL)
                    _ = Ast_Node_Variable_Assignment::generate(__astCntrl, _);

                goto rtr;

            }

        case TOKEN_OPENPARENTHESES:

            {
                _ = parser::Ast_Node_Parenthesis::generate(__astCntrl);

                if (__astCntrl->getToken(0)->id == TOKEN_EQUAL)
                    _ = Ast_Node_Variable_Assignment::generate(__astCntrl, _);

                goto rtr;

            }

    default: break;
    }


    if (parser::isImplicitValue(__astCntrl->getToken(0))) _ = parser::Ast_Node_Value::generate(__astCntrl);

    else if (parser::isAssignment(__astCntrl->getToken(0))) _ = parser::Ast_Node_Variable_Assignment::generate(__astCntrl);

    else new Ast_Execption("Ast Node Expression get first -> Something went wrong");

rtr:

    return _;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::getSecondNode(Ast_Control* __astCntrl) { return parser::Ast_Node_Expression::generate(__astCntrl, NULL); }

int parser::Ast_Node_Expression::getByteSize() {

    int _byte_size = 0, _;

    switch (first->node_id)
    {
    case AST_NODE_VALUE: _byte_size = ((Ast_Node_Value*) first)->getByteSize(); break;
    case AST_NODE_VARIABLE: _byte_size = ((Ast_Node_Variable*) first)->getByteSize(); break;
    case AST_NODE_PARENTHESIS: // _byte_size = ((Ast_Node_Parenthesis*) first)->
        break;
    case AST_NODE_FUNCTION_CALL: _byte_size = ((Ast_Node_Function_Call*) first)->getByteSize(); break;
        break;
    case AST_NODE_POINTER_OPERATORS: //
        break;
    default: break;
    }

    if (second) {

        _ = second->getByteSize();

        if (_byte_size < _) _byte_size = _;

    } 

    return _byte_size;

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

parser::Type_Information* parser::Ast_Node_Value::getType() {

    parser::Type_Information* _;

    _ = new parser::Type_Information(
        NULL,
        parser_helper::getTokenIdTypeFromTokenIdImplicitValue(token_id),
        0, NULL, NULL
    );

    return _;

}

int parser::Ast_Node_Value::getByteSize() {

    parser::Type_Information* _type_information = getType();

    int _byte_size = _type_information->getByteSize();

    delete _type_information;

    return _byte_size;

}


parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(Name_Space* _nmSpc, int _declId, bool _isGbl) : Ast_Node(AST_NODE_VARIABLE), name_space(_nmSpc), declaration_id(_declId), is_global(_isGbl) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    __astCntrl->printDebugInfo("--> Ast Node Variable <--");

    // __nmSpc->print();

    if (__nmSpc) {

        __astCntrl->saveState();

        __astCntrl->current_code_block = NULL;
        __astCntrl->current_name_space = __nmSpc;

    }
    
    parser::Ast_Node_Variable* _ = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));
    
    int _declId = Name_Tracker::getDeclarationId(__astCntrl, __astCntrl->getToken(0)->phr);
    
    if (_declId == -1) new Ast_Execption("No name declared with given name");

    bool _isGbl = __astCntrl->current_code_block ? __astCntrl->current_code_block->isDeclarationIdGlobal(__astCntrl->getToken(0)->phr) : true;

    __astCntrl->current_token_position++;

    new (_) parser::Ast_Node_Variable(
        __nmSpc, _declId, _isGbl
    );

    if (__nmSpc) __astCntrl->setPreviousSavedState();

    std::cout << "Variable is " << ((_isGbl) ? "Global" : "Local") << " and declaration id -> " << _declId << std::endl;

    return _;

}

parser::Type_Information* parser::Ast_Node_Variable::getType() { return variable_declaration ? variable_declaration->getType() : NULL; }

int parser::Ast_Node_Variable::getByteSize() { return variable_declaration->getByteSize(); }


parser::Ast_Node_Variable_Assignment::~Ast_Node_Variable_Assignment() {
    if (value_before_assign) value_before_assign->~Ast_Node(); free(value_before_assign);
    if (value) value->~Ast_Node_Expression(); free(value);
}

parser::Ast_Node_Variable_Assignment::Ast_Node_Variable_Assignment(Ast_Node* _valBef, Ast_Node_Expression* _val, bool _isLft, int _expId) 
    : Ast_Node(AST_NODE_VARIABLE_ASSIGNMENT), value_before_assign(_valBef), value(_val), operation_is_left(_isLft), expression_id(_expId) {}

parser::Ast_Node_Variable_Assignment* parser::Ast_Node_Variable_Assignment::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Variable Assignment <--");

    parser::Ast_Node* _valBef;

    parser::Token* _expTk = __astCntrl->getToken(0);

    if (!parser::isSingleAssignment(_expTk)) new Ast_Execption("Not single assignment not allowed before variable");
    else _expTk->id = TOKEN_INCREMENT ? TOKEN_INCREMENT_LEFT : TOKEN_DECREMENT_LEFT;
    __astCntrl->current_token_position++;

    _valBef = getValueBeforeAssign(__astCntrl);

    parser::Ast_Node_Variable_Assignment* _varAssign = (parser::Ast_Node_Variable_Assignment*) malloc(sizeof(parser::Ast_Node_Variable_Assignment));
    new (_varAssign) parser::Ast_Node_Variable_Assignment(
        _valBef, NULL, true, _expTk->id
    );

    return _varAssign;


}

parser::Ast_Node_Variable_Assignment* parser::Ast_Node_Variable_Assignment::generate(Ast_Control* __astCntrl, Ast_Node* __valBef) {

    __astCntrl->printDebugInfo("--> Variable Assignment <--");

    parser::Token* _expTk = __astCntrl->getToken(0);
    __astCntrl->current_token_position++;

    parser::Ast_Node_Expression* _value = getValue(__astCntrl);

    parser::Ast_Node_Variable_Assignment* _varAssign = (parser::Ast_Node_Variable_Assignment*) malloc(sizeof(parser::Ast_Node_Variable_Assignment));
    new (_varAssign) parser::Ast_Node_Variable_Assignment(
        __valBef, _value, false, _expTk->id
    );

    return _varAssign;

}

parser::Ast_Node* parser::Ast_Node_Variable_Assignment::getValueBeforeAssign(Ast_Control* __astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Variable 
    *   Ast_Parenthesis //
    *   Ast_Pointer_Operators 
    */

    parser::Token* _tk = __astCntrl->getToken(0);
    parser::Ast_Node* _;

    Name_Space* __nmSpc = Name_Space::checkIfNameSpace(__astCntrl, NULL);

    switch (_tk->id)
    {
    case TOKEN_IDENTIFIER:
            if (__astCntrl->getToken(1)->id == TOKEN_OPENPARENTHESES) _ = parser::Ast_Node_Function_Call::generate(__astCntrl, __nmSpc);

            else _ = parser::Ast_Node_Variable::generate(__astCntrl, __nmSpc);

            goto rtr;

        case TOKEN_ADDRESS: case TOKEN_POINTER:

            {

                utils::LinkedList <int>* _operators = new utils::LinkedList <int>();
                parser_helper::setPointerOperators(__astCntrl, _operators, 0);

                _ = parser::Ast_Node_Pointer_Operators::generate(
                    __astCntrl, _operators
                );

                delete _operators;

                goto rtr;

            }
        case TOKEN_OPENPARENTHESES:
            _ = parser::Ast_Node_Parenthesis::generate(__astCntrl);

            goto rtr;

        default: break;
    }  

    if (parser::isImplicitValue(__astCntrl->getToken(0))) return parser::Ast_Node_Value::generate(__astCntrl);

    else if (parser::isAssignment(__astCntrl->getToken(0))) return parser::Ast_Node_Variable_Assignment::generate(__astCntrl);

    else new Ast_Execption("Ast Node Variable Assignment get value before assignment -> Something went wrong"); 

rtr:

    return _;

}

parser::Ast_Node_Expression* parser::Ast_Node_Variable_Assignment::getValue(Ast_Control* __astCntrl) { return parser::Ast_Node_Expression::generate(__astCntrl, NULL); }

parser::Type_Information* parser::Ast_Node_Variable_Assignment::getType() { if (variable_declaration) return variable_declaration->getType(); return NULL; }

int parser::Ast_Node_Variable_Assignment::getByteSize() { if (variable_declaration) return variable_declaration->getByteSize(); return -1; }


parser::Ast_Node_Pointer_Operators::~Ast_Node_Pointer_Operators() { if (value) value->~Ast_Node(); free(value); }

parser::Ast_Node_Pointer_Operators::Ast_Node_Pointer_Operators(utils::LinkedList <int>* __oprs, Ast_Node* _value) 
    : Ast_Node(AST_NODE_POINTER_OPERATORS), value(_value), pointer_level(0) {
        for (int _ = 0; _ < __oprs->count; _++)
            pointer_level += ((*__oprs)[_] == TOKEN_POINTER) ? 1 : -1;
    }

parser::Ast_Node_Pointer_Operators* parser::Ast_Node_Pointer_Operators::generate(Ast_Control* __astCntrl, utils::LinkedList <int>* __oprs) {

    __astCntrl->printDebugInfo("--> Ast Node Pointer Operators <--");

    parser::Ast_Node* _value = getValue(__astCntrl);

    if (!_value) new Ast_Execption("Pointer Operators error getting value inside");

    parser::Ast_Node_Pointer_Operators* _ = (parser::Ast_Node_Pointer_Operators*) malloc(sizeof(parser::Ast_Node_Pointer_Operators));
    new (_) parser::Ast_Node_Pointer_Operators(
        __oprs, _value
    );

    __astCntrl->printDebugInfo("--> Ast Node Pointer Operators end <--");

    return _;

}

parser::Ast_Node* parser::Ast_Node_Pointer_Operators::getValue(Ast_Control* __astCntrl) {

    /* Supported Nodes:
    *   Ast_Node_Value
    *   Ast_Node_Variable
    *   Ast_Parenthesis //
    */

    Name_Space* _nmSpc = Name_Space::checkIfNameSpace(__astCntrl, NULL);

    if (__astCntrl->getToken(0)->id == TOKEN_OPENPARENTHESES) return parser::Ast_Node_Parenthesis::generate(__astCntrl);

    if (__astCntrl->getToken(0)->id == TOKEN_IDENTIFIER) {
        
        if (__astCntrl->getToken(1)->id == TOKEN_OPENPARENTHESES) return parser::Ast_Node_Function_Call::generate(__astCntrl, _nmSpc);
    
        return parser::Ast_Node_Variable::generate(__astCntrl, _nmSpc);

    }

    if (parser::isImplicitValue(__astCntrl->getToken(0))) return parser::Ast_Node_Value::generate(__astCntrl);

    new Ast_Execption("Ast Node Pointer Operators -> Something went wrong");

}


parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() { if (value) value->~Ast_Node_Expression(); free(value); }

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node_Expression* _value) : Ast_Node(AST_NODE_PARENTHESIS), value(_value) {}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Generate Parenthesis <--");

    parser::Ast_Node_Parenthesis* _ = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));

    __astCntrl->current_token_position++;

    new (_) parser::Ast_Node_Parenthesis(
        parser::Ast_Node_Expression::generate(__astCntrl, NULL)
    );

    __astCntrl->printDebugInfo("--> End Parenthesis <--");

    __astCntrl->current_token_position++;

    return _;

}


parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(int _declId, utils::LinkedList <Ast_Node_Expression*>* _params, Name_Space* __nmSpc) 
    : Ast_Node(AST_NODE_FUNCTION_CALL), declaration_id(_declId), parameters(_params), name_space(__nmSpc) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Control* __astCntrl, Name_Space* __nmSpc) {

    __astCntrl->printDebugInfo("--> Ast Node Function Call <--");

    parser::Ast_Node_Function_Call* _ = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));

    int _declId;

    if (__nmSpc) {
        __astCntrl->saveState();

        __astCntrl->current_code_block = NULL;
        __astCntrl->current_name_space = __nmSpc;
    }

    _declId = Name_Tracker::getDeclarationId(__astCntrl, __astCntrl->getToken(0)->phr);

    __astCntrl->current_token_position++;

    if (_declId == -1) new Ast_Execption("No declaration found with given name");

    if (__nmSpc) __astCntrl->setPreviousSavedState();

    new (_) parser::Ast_Node_Function_Call(
        _declId, getFunctionCallParameters(__astCntrl), __nmSpc
    );

    return _;

}

utils::LinkedList <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getFunctionCallParameters(Ast_Control* __astCntrl) {

    __astCntrl->printDebugInfo("--> Ast Node Function Call Parameters <--");

    utils::LinkedList <Ast_Node_Expression*>* _parameters = new utils::LinkedList <Ast_Node_Expression*>();

    __astCntrl->current_token_position++;

    while((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id != TOKEN_CLOSEPARENTHESES) {

        __astCntrl->printDebugInfo("--> Ast Node Function Call Parameter Added <--");

        _parameters->add(
            parser::Ast_Node_Expression::generate(
                __astCntrl, NULL
            )
        );

        if ((*__astCntrl->tokens_collection)[(__astCntrl->current_token_position)]->id == TOKEN_COMMA) (__astCntrl->current_token_position)++;

    }

    __astCntrl->current_token_position++;

    return _parameters;

}

parser::Type_Information* parser::Ast_Node_Function_Call::getType() { return function_declaration ? function_declaration->getType() : NULL; }

int parser::Ast_Node_Function_Call::getByteSize() { return function_declaration ? function_declaration->getByteSize() : -1; }


