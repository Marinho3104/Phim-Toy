#include "./ast_nodes.h"

#include "./../utils/commonFunctions.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./ast_helper.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __id) : node_id(__id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) 
    : Ast_Node(AST_NODE_NAME_SPACE), name_space(__name_space) { declarations = new utils::LinkedList <Ast_Node*>(); }

void parser::Ast_Node_Name_Space::generate(Ast_Control* __ast_control, Name_Space* __name_space) {

    __ast_control->printDebugInfo("--> Ast Node Name Space <--");

    __ast_control->saveState();

    parser::Ast_Node_Name_Space* _ast_name_space = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));

    new (_ast_name_space) parser::Ast_Node_Name_Space(__name_space);

    __ast_control->nodes_name_spaces->add(_ast_name_space);

    __ast_control->current_name_space = __name_space;

    _ast_name_space->setDeclarations(__ast_control);

    __ast_control->setLastSavedState();

    __ast_control->printDebugInfo("--> Ast Node Name Space End <--");

}

void parser::Ast_Node_Name_Space::setDeclarations(Ast_Control* __ast_control) {

    utils::LinkedList <Ast_Node*>* _body;
    bool _sts;

    while(__ast_control->current_token_position < __ast_control->tokens_collection->count) {
        
        _body = getNodes(__ast_control, NULL);

        _sts = declarations->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Name_Space::getNodes(Ast_Control* __ast_control, Name_Space* __name_space) {

    utils::LinkedList <parser::Ast_Node*>* _declarations = new utils::LinkedList <parser::Ast_Node*>();

    // std::cout << "Token 1 -> " << _first_token->id << std::endl;
    // std::cout << "Token 2 -> " << _second_token->id << std::endl;

    if (Name_Space* _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control)) __name_space = _name_space;
    
    switch (__ast_control->getToken(0)->id)
    {
        case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __ast_control->current_token_position++; goto cont;
        case TOKEN_NAMESPACE:
            {

                __ast_control->current_token_position++;

                utils::LinkedList <char*>* _new_scope = parser_helper::getNameSpaceScope(__ast_control);

                Name_Space* _name_space = __ast_control->name_space_control->getNameSpaceOrAdd(_new_scope);

                delete _new_scope;

                __ast_control->current_token_position++;

                Ast_Node_Name_Space::generate(
                    __ast_control, _name_space
                );

                delete _declarations;

                _declarations = getNodes(__ast_control, NULL);

                goto cont;

            }
        case TOKEN_STRUCT: case TOKEN_CONTRACT:
            
            _declarations->add(
                Ast_Node_Struct_Declaration::generate(
                    __ast_control
                )
            );

            goto cont;

        default: break;
    }

    if (parser::isPrimativeType(__ast_control->getToken(0)) || __ast_control->getToken(0)->id == TOKEN_IDENTIFIER) {

        Type_Information* _typeInformation = Type_Information::generate(__ast_control, __name_space);

        __name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

        if (__ast_control->getToken(0)->id != TOKEN_IDENTIFIER) goto excep;

        switch (__ast_control->getToken(1)->id)
        {
        case TOKEN_OPENPARENTHESES: 
        
            _declarations->add(
                parser::Ast_Node_Function_Declaration::generate(
                    __ast_control, __name_space, _typeInformation
                )
            );

            break;        
        default: 

            delete _declarations;

            _declarations = Ast_Node_Variable_Declaration::generate(
                    __ast_control, _typeInformation
                );

            break;
        }

        goto cont;

    }    

excep:

    new Ast_Exception("Unexpected token - getNodes() -- Ast Node Name Space");

cont:

    return _declarations;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { free(name_tracker->off); delete name_tracker; delete code; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Name_Space* __name_space, Ast_Node_Code_Block* __previous_block) 
    : Ast_Node(AST_NODE_CODE_BLOCK), name_space(__name_space), previous_block(__previous_block) { 
        code = new utils::LinkedList <Ast_Node*>();
        int* _off = (int*) malloc(sizeof(4));
        *_off = __previous_block ? *__previous_block->name_tracker->off : 0;
        name_tracker = new Name_Tracker(_off); 
    }

void parser::Ast_Node_Code_Block::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Ast Node Block Code <--");

    __ast_control->current_code_block->generateBody(__ast_control);

    __ast_control->setLastSavedState();

    __ast_control->printDebugInfo("--> Ast Node Block Code End <--");

}

void parser::Ast_Node_Code_Block::setUp(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Set Code Block Up <--");

    __ast_control->saveState();

    parser::Ast_Node_Code_Block* _code_block = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));

    new (_code_block) parser::Ast_Node_Code_Block(
        __ast_control->current_name_space, __ast_control->current_code_block    
    );

    __ast_control->current_code_block = _code_block;

    __ast_control->printDebugInfo("--> Set Code Block Up End <--");

}

void parser::Ast_Node_Code_Block::generateBody(Ast_Control* __ast_control) {

    utils::LinkedList <Ast_Node*>* _body;
    bool _sts;

    while(__ast_control->current_token_position < __ast_control->tokens_collection->count) {
        
        _body = getNodes(__ast_control, NULL);

        _sts = code->join(_body);

        _body->destroy_content = 0;

        delete _body;

        if (!_sts) break;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Code_Block::getNodes(Ast_Control* __ast_control, Name_Space* __name_space) {

    utils::LinkedList <parser::Ast_Node*>* _code = new utils::LinkedList <parser::Ast_Node*>();

    // std::cout << "Token 1 -> " << _first_token->id << std::endl;
    // std::cout << "Token 2 -> " << _second_token->id << std::endl;

    if (Name_Space* _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control)) __name_space = _name_space;
    
    switch (__ast_control->getToken(0)->id)
    {
        case TOKEN_END_CODE: case TOKEN_CLOSECURLYBRACKET: __ast_control->current_token_position++; goto cont;
        case TOKEN_ENDINSTRUCTION: __ast_control->current_token_position++; delete _code; return getNodes(__ast_control, NULL);
        case TOKEN_STRUCT: case TOKEN_CONTRACT:
            
            _code->add(
                Ast_Node_Struct_Declaration::generate(
                    __ast_control
                )
            );

            goto cont;

        default: break;
    }

    if (parser::isPrimativeType(__ast_control->getToken(0)) || __ast_control->getToken(0)->id == TOKEN_IDENTIFIER) {

        int _back_up_point = __ast_control->current_token_position;

        Type_Information* _typeInformation = Type_Information::generate(__ast_control, __name_space);

        Name_Space* _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

        if (__ast_control->getToken(0)->id != TOKEN_IDENTIFIER) 
            { delete _typeInformation; __ast_control->current_token_position = _back_up_point; goto expression; }

        switch (__ast_control->getToken(1)->id)
        {
        case TOKEN_OPENPARENTHESES: 
        
            _code->add(
                parser::Ast_Node_Function_Declaration::generate(
                    __ast_control, _name_space, _typeInformation
                )
            );

            break;        
        default: 

            delete _code;

            _code = Ast_Node_Variable_Declaration::generate(
                    __ast_control, _typeInformation
                );

            break;
        }

        goto cont;

    }

    else 

    expression:
        _code->add(
            Ast_Node_Expression::generate(
                __ast_control, __name_space
            )
        );

cont:

    return _code;

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __n) {

    int _;

    if (
        (_ = name_tracker->getDeclarationId(__n)) == -1
    )

        if (previous_block) _ = previous_block->getDeclarationId(__n);

        else return name_space->getDeclarationId(__n);

    return _;

}

bool parser::Ast_Node_Code_Block::isDeclarationIdGlobal(char* __n) {

    if (
        name_tracker->getDeclarationId(__n) == -1
    )

        if (previous_block) return previous_block->isDeclarationIdGlobal(__n);

        else return true;

    return false;


}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* __type, int __declaration_id)
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), type(__type), declaration_id(__declaration_id) {}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* __ast_control, Type_Information* __type) {

    utils::LinkedList <parser::Ast_Node*>* _nodes = new utils::LinkedList <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;

    while(__ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        __ast_control->printDebugInfo("--> Ast Node Variable Declaration <--");

        _declaration_id = parser_helper::addToNameTracker(__ast_control);
        __ast_control->current_token_position++;

        // std::cout << "Declaration id -> " << _declaration_id << std::endl;

        _variable_declaration = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
        new (_variable_declaration) parser::Ast_Node_Variable_Declaration(__type, _declaration_id);

        _nodes->add(_variable_declaration);

        switch (__ast_control->getToken(0)->id)
        {
        case TOKEN_EQUAL: new Ast_Exception("TODO assign in declaration"); break;
        case TOKEN_COMMA: __ast_control->current_token_position++; __type = Type_Information::generate(__ast_control, __type); break;
        default: break;
        }

        __ast_control->printDebugInfo("--> Ast Node Variable Declaration End <--");

    }

    __ast_control->current_token_position++;

    return _nodes;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter(Ast_Control* __ast_control, Type_Information* __type) {

    __ast_control->printDebugInfo("--> Node Function Parameter <--");

    utils::LinkedList <parser::Ast_Node*>* _nodes = new utils::LinkedList <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id = -1;

    if (__ast_control->getToken(0)->id == TOKEN_IDENTIFIER) {

        _declaration_id = parser_helper::addToNameTracker(__ast_control);
        __ast_control->current_token_position++;

    }

    // std::cout << "Declaration id -> " << _declaration_id << std::endl;
    
    _variable_declaration = (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));
    new (_variable_declaration) parser::Ast_Node_Variable_Declaration(__type, _declaration_id);

    _nodes->add(_variable_declaration);

    if (__ast_control->getToken(0)->id == TOKEN_EQUAL) new Ast_Exception("TODO Exception");

    return _nodes;

}


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() { 
    delete return_type; 
    delete parameters; 
    if (body) body->~Ast_Node_Code_Block(); free(body); }

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    Name_Space* __name_space, Type_Information* __return_type, utils::LinkedList <Ast_Node*>* __params, Ast_Node_Code_Block* __body) 
        : Ast_Node(AST_NODE_FUNCTION_DECLARATION), name_space(__name_space), return_type(__return_type), parameters(__params), body(__body) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* __ast_control, Name_Space* __name_space, Type_Information* __return_type) {

    __ast_control->printDebugInfo("--> Node Function Declaration <--");

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    parser::Ast_Node_Code_Block* _function_body;

    if (__name_space) { __ast_control->saveState(); __ast_control->current_name_space = __name_space; }

    int _declaration_id = parser_helper::addToNameTracker(__ast_control);
    __ast_control->current_token_position++;

    Ast_Node_Code_Block::setUp(__ast_control);
    _function_body = __ast_control->current_code_block;

    utils::LinkedList <Ast_Node*>* _params = getParameters(__ast_control);

    if (__ast_control->getToken(0)->id == TOKEN_ENDINSTRUCTION) {

        __ast_control->setLastSavedState(); _function_body->~Ast_Node_Code_Block(); free(_function_body); _function_body = NULL;

        __ast_control->current_token_position++;

    } else { __ast_control->current_token_position++; Ast_Node_Code_Block::generate(__ast_control); }

    new (_function_declaration) parser::Ast_Node_Function_Declaration(
        __name_space, __return_type, _params, _function_body
    );

    // std::cout << "Function declaration id - >" << _declaration_id << std::endl;
    // std::cout << "Params count -> " << _params->count << std::endl;

    if (__name_space) __ast_control->setLastSavedState();

    __ast_control->printDebugInfo("--> Node Function Declaration End <--");

    return _function_declaration;

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Ast Node Function Parameters <--");
    utils::LinkedList <Ast_Node*>* _params = new utils::LinkedList <Ast_Node*>(), *_temp;
    Type_Information* _type;
    Name_Space* _name_space;

    __ast_control->current_token_position++;

    while(__ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

        _type = Type_Information::generate(__ast_control, _name_space);

        _temp = Ast_Node_Variable_Declaration::generateFunctionParameter(__ast_control, _type); 

        _params->join(_temp);

        _temp->destroy_content = 0; delete _temp;

        if (__ast_control->getToken(0)->id == TOKEN_COMMA) __ast_control->current_token_position++;

    }

    __ast_control->current_token_position++;

    return _params;

}


parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() { 
    if (own_name_space) own_name_space->~Name_Space(); free(own_name_space); 
    delete fields; 
    delete functions; }

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(Name_Space* __own_name_space, int __declaration_id, bool __is_contract, utils::LinkedList <Ast_Node*>* __fields, utils::LinkedList <Ast_Node_Function_Declaration*>* __functions) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION), own_name_space(__own_name_space), declaration_id(__declaration_id), is_contract(__is_contract), fields(__fields), functions(__functions) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Struct Declaration <--");

    utils::LinkedList <parser::Ast_Node_Function_Declaration*>* _functions;
    utils::LinkedList <Ast_Node*>* _fields;

    bool _is_contract = __ast_control->getToken(0)->id == TOKEN_CONTRACT;
    __ast_control->current_token_position++;

    int _declaration_id;

    if (__ast_control->getToken(0)->id == TOKEN_IDENTIFIER) {

        _declaration_id = parser_helper::addToNameTracker(__ast_control);

        __ast_control->current_token_position++;

    } else if (__ast_control->getToken(0)->id == TOKEN_OPENCURLYBRACKET) {
        new Ast_Exception("TODO struct no identifier at begining");
    } else new Ast_Exception("Unexpected token");

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_OPENCURLYBRACKET:
        {
            int _inicial_position = __ast_control->current_token_position;

            _fields = getFields(__ast_control);

            __ast_control->current_token_position = _inicial_position;

            setNewNameSpaceForStruct(__ast_control);

            Type_Information* _type_struct = new Type_Information(
                __ast_control, TOKEN_IDENTIFIER, _declaration_id, NULL, __ast_control->current_name_space
            );

            _functions = getFunctions(__ast_control, _type_struct);

            delete _type_struct;

        }
    case TOKEN_ENDINSTRUCTION: break;
    default: new Ast_Exception("Unexpected token in struct declaration");
    }

    // std::cout << "Struct fields count -> " << _fields->count << std::endl;
    // std::cout << "Struct functions count -> " << _functions->count << std::endl;
    // std::cout << "Struct id -> " << _declaration_id << std::endl;
    // std::cout << "Struct own name space -> " << __ast_control->current_name_space << std::endl;

    Ast_Node_Struct_Declaration* _struct_declaration = (Ast_Node_Struct_Declaration*) malloc(sizeof(Ast_Node_Struct_Declaration));

    new (_struct_declaration) parser::Ast_Node_Struct_Declaration(
        __ast_control->current_name_space, _declaration_id, _is_contract, _fields, _functions
    );

    __ast_control->setLastSavedState(); __ast_control->current_token_position++;

    __ast_control->printDebugInfo("--> Node Struct Declaration End <--");

    return _struct_declaration;

}

void parser::Ast_Node_Struct_Declaration::ignoreCodeBlock(Ast_Control* __ast_control) {

    __ast_control->current_token_position++;

    while(__ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        if (__ast_control->getToken(0)->id == TOKEN_OPENCURLYBRACKET) ignoreCodeBlock(__ast_control);

        __ast_control->current_token_position++;

    }

}

utils::LinkedList <parser::Ast_Node*>* parser::Ast_Node_Struct_Declaration::getFields(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Ast Node Struct Fields <--"); __ast_control->current_token_position++;

    utils::LinkedList <parser::Ast_Node*>* _fields = new utils::LinkedList <parser::Ast_Node*>(), *_varDecl;
    Type_Information* _type; Name_Space* _name_space;

    Ast_Node_Code_Block::setUp(__ast_control);

    while(__ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

        _type = Type_Information::generate(__ast_control, _name_space);

        if (__ast_control->getToken(1)->id == TOKEN_OPENPARENTHESES) {

            delete _type;

            while(__ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) __ast_control->current_token_position++;

            __ast_control->current_token_position++;

            switch (__ast_control->getToken(0)->id)
            {
            case TOKEN_ENDINSTRUCTION: __ast_control->current_token_position++; break;
            case TOKEN_OPENCURLYBRACKET: ignoreCodeBlock(__ast_control); __ast_control->current_token_position++; break;
            default: new Ast_Exception("Unexpected token at struct body");
            }

            continue;

        }

        _varDecl = Ast_Node_Variable_Declaration::generate(
            __ast_control, _type
        );

        _fields->join(_varDecl);

        _varDecl->destroy_content = 0; delete _varDecl;

    }

    __ast_control->current_code_block->~Ast_Node_Code_Block(); free(__ast_control->current_code_block);

    __ast_control->setLastSavedState();

    __ast_control->printDebugInfo("--> Ast Node Struct Fields End <--");

    return _fields;

}

utils::LinkedList <parser::Ast_Node_Function_Declaration*>* parser::Ast_Node_Struct_Declaration::getFunctions(Ast_Control* __ast_control, Type_Information* __type_struct) {

    __ast_control->printDebugInfo("--> Ast Node Struct Functions <--");

    utils::LinkedList <parser::Ast_Node_Function_Declaration*>* _functions = new utils::LinkedList <parser::Ast_Node_Function_Declaration*>();
    Type_Information* _type; Name_Space* _name_space;

    __ast_control->current_token_position++;

    while(__ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

        _type = Type_Information::generate(__ast_control, _name_space);

        if (__ast_control->getToken(1)->id != TOKEN_OPENPARENTHESES) {

            // Until it reaches the end of of instruction ';'
            while((*__ast_control->tokens_collection)[__ast_control->current_token_position++]->id != TOKEN_ENDINSTRUCTION);

            continue;

        }

        _functions->add(
            parser::Ast_Node_Function_Declaration::generate(
                __ast_control, NULL, _type
            )
        );

    }

    __ast_control->current_token_position++;

    return  _functions;

}

void parser::Ast_Node_Struct_Declaration::setNewNameSpaceForStruct(Ast_Control* __ast_control) {

    utils::LinkedList <char*>* _scope = new utils::LinkedList <char*>();

    parser::Name_Space* _name_space = (parser::Name_Space*) malloc(sizeof(parser::Name_Space));

    new (_name_space) parser::Name_Space(
        __ast_control->name_space_control, _scope
    );

    _name_space->updateOff(NULL);

    __ast_control->saveState();

    __ast_control->current_name_space = _name_space;

}


parser::Ast_Node_Expression::~Ast_Node_Expression() {
    if (first_expression) first_expression->~Ast_Node(); free(first_expression);
    if (second_expression) second_expression->~Ast_Node_Expression(); free(second_expression);
}

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* __first_expression, Ast_Node_Expression* __second_expression, int __expression_operator_id) 
    : Ast_Node(AST_NODE_EXPRESSION), first_expression(__first_expression), second_expression(__second_expression), expression_operator_id(__expression_operator_id) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Control* __ast_control, Name_Space* __name_space) {

    __ast_control->printDebugInfo("--> Node Expression <--");

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    parser::Ast_Node* _first_expression = getFirstExpression(__ast_control, __name_space);
    parser::Token* _token_operator = __ast_control->getToken(0);
    __ast_control->current_token_position++;

    parser::Ast_Node_Expression* _second_expression = 
        parser::isExpressionOperator(_token_operator) ? parser::Ast_Node_Expression::generate(__ast_control, NULL) : NULL;

    if (!_second_expression) __ast_control->current_token_position--;

    new (_expression_node) parser::Ast_Node_Expression(
        _first_expression, _second_expression, _second_expression ? _token_operator->id : -1
    );

    __ast_control->printDebugInfo("--> Node Expression End <--");

    return _expression_node;

}

parser::Ast_Node* parser::Ast_Node_Expression::getFirstExpression(Ast_Control* __ast_control, Name_Space* __name_space) {

    /* Supported Nodes:
    *   Ast_Node_Value //
    *   Ast_Node_Variable //
    *   Ast_Node_Parenthesis //
    *   Ast_Node_Function_Call //
    *   Ast_Node_Pointer_Operations //
    */

   parser::Ast_Node* _node = NULL;

    if (!__name_space) __name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

    __ast_control->printDebugInfo("--> Node Expression -- First Expression <--");

    switch (__ast_control->getToken(0)->id)
    {
        case TOKEN_IDENTIFIER:
            
            switch (__ast_control->getToken(1)->id)
            {
            case TOKEN_OPENPARENTHESES: _node = parser::Ast_Node_Function_Call::generate(__ast_control, __name_space); break;
            default: _node = parser::Ast_Node_Variable::generate(__ast_control, __name_space); break;
            }

            if (parser::isAssignment(__ast_control->getToken(0))) _node = Ast_Node_Assignment::generate(__ast_control, _node);

            else if (parser::isAccessingExpression(__ast_control->getToken(0))) _node = Ast_Node_Accessing::generator(__ast_control, _node);

            goto rtr;

        case TOKEN_INCREMENT: case TOKEN_DECREMENT: _node = parser::Ast_Node_Assignment::generate(__ast_control); goto rtr;
        case TOKEN_POINTER: case TOKEN_ADDRESS: 
        
            _node = parser::Ast_Node_Pointer_Operator::generate(__ast_control); 

            if (parser::isAssignment(__ast_control->getToken(0))) _node = Ast_Node_Assignment::generate(__ast_control, _node);
            
            goto rtr;

        case TOKEN_OPENPARENTHESES:

            _node = parser::Ast_Node_Parenthesis::generate(__ast_control);

            if (parser::isAssignment(__ast_control->getToken(0))) _node = Ast_Node_Assignment::generate(__ast_control, _node);
            
            goto rtr;

        default: break;
    }


    if (parser::isImplicitValue(__ast_control->getToken(0))) _node = Ast_Node_Value::generate(__ast_control);

    else new Ast_Exception("Ast Node Expression get first -> Something went wrong");

    __ast_control->printDebugInfo("--> Node Expression -- First Expression End <--");

rtr:

    return _node;

}


parser::Ast_Node_Value::~Ast_Node_Value() {}

parser::Ast_Node_Value::Ast_Node_Value(int __value_position, int __token_id) : Ast_Node(AST_NODE_VALUE), value_position(__value_position), token_id(__token_id) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Value <--");

    parser::Ast_Node_Value* _value_node = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new (_value_node) parser::Ast_Node_Value(
        __ast_control->storage->addNewValue(__ast_control->getToken(0)->phr), __ast_control->getToken(0)->id
    );

    __ast_control->current_token_position++;

    __ast_control->printDebugInfo("--> Node Value End <--");

    return _value_node;

}


parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(Name_Space* __name_space, int __declaration_id, bool __is_global) 
    : Ast_Node(AST_NODE_VARIABLE), name_space(__name_space), declaration_id(__declaration_id), is_global(__is_global) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate(Ast_Control* __ast_control, Name_Space* __name_space) {

    __ast_control->printDebugInfo("--> Node Variable <--");

    parser::Ast_Node_Variable* _value_node = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));

    if (__name_space) { __ast_control->saveState(); __ast_control->current_name_space = __name_space; __ast_control->current_code_block = NULL; }

    int _declaration_id = parser_helper::getDeclarationId(__ast_control);

    if (_declaration_id == -1) new Ast_Exception("Name not declared");

    bool _is_global = parser_helper::isDeclarationIdGlobal(__ast_control);

    __ast_control->current_token_position++;

    new (_value_node) parser::Ast_Node_Variable(
        __name_space, _declaration_id, _is_global
    );

    if (__name_space) __ast_control->setLastSavedState();

    std::cout << "Variable is " << (_is_global ? "global" : "local") << std::endl << std::endl;
    
    __ast_control->printDebugInfo("--> Node Variable End <--");

    return _value_node;

}


parser::Ast_Node_Assignment::~Ast_Node_Assignment() {
    if (value_before_assign) value_before_assign->~Ast_Node(); free(value_before_assign);
    if (value_after_assign) value_after_assign->~Ast_Node_Expression(); free(value_after_assign);
}

parser::Ast_Node_Assignment::Ast_Node_Assignment(Ast_Node* __value_before, Ast_Node_Expression* __value_after, int __expression_id) 
    : Ast_Node(AST_NODE_ASSIGNMENT), value_before_assign(__value_before), value_after_assign(__value_after), expression_operator_id(__expression_id) {}

parser::Ast_Node_Assignment* parser::Ast_Node_Assignment::generate(Ast_Control* __ast_control, Ast_Node* __value_before) {

    __ast_control->printDebugInfo("--> Node Assignment <--");

    parser::Token* _token_expression = __ast_control->getToken(0);

    __ast_control->current_token_position++;

    parser::Ast_Node_Assignment* _assignment_node = (parser::Ast_Node_Assignment*) malloc(sizeof(parser::Ast_Node_Assignment));

    parser::Ast_Node_Expression* _value_after = NULL;

    if (!parser::isSingleAssignment(_token_expression)) _value_after = parser::Ast_Node_Expression::generate(__ast_control, NULL);

    new (_assignment_node) parser::Ast_Node_Assignment(
        __value_before, _value_after, _token_expression->id
    );

    __ast_control->printDebugInfo("--> Node Assignment End <--");

    return _assignment_node;

}

parser::Ast_Node_Assignment* parser::Ast_Node_Assignment::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Assignment Single <--");

    parser::Token* _token_expression = __ast_control->getToken(0);

    __ast_control->current_token_position++;

    if (!parser::isSingleAssignment(_token_expression)) new Ast_Exception("Not single assignment expression allowed");

    parser::Ast_Node_Assignment* _assignment_node = (parser::Ast_Node_Assignment*) malloc(sizeof(parser::Ast_Node_Assignment));

    new (_assignment_node) parser::Ast_Node_Assignment(
        parser::Ast_Node_Expression::getFirstExpression(__ast_control, NULL), NULL, _token_expression->id
    );

    __ast_control->printDebugInfo("--> Node Assignment Single End <--");

    return _assignment_node;

}


parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(Name_Space* __name_space, int __declaration_id, utils::LinkedList <parser::Ast_Node_Expression*>* __parameters) 
    : Ast_Node(AST_NODE_FUNCTION_CALL), name_space(__name_space), declaration_id(__declaration_id), parameters(__parameters) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Control* __ast_control, Name_Space* __name_space) {

    __ast_control->printDebugInfo("--> Node Function Call <--");

    parser::Ast_Node_Function_Call* _function_call_node = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));

    if (__name_space) { __ast_control->saveState(); __ast_control->current_name_space = __name_space; __ast_control->current_code_block = NULL; }

    int _declaration_id = parser_helper::getDeclarationId(__ast_control);
    __ast_control->current_token_position++;

    if (_declaration_id == -1) new Ast_Exception("No name declared");

    utils::LinkedList <parser::Ast_Node_Expression*>* _parameters = getParameters(__ast_control);

    new (_function_call_node) parser::Ast_Node_Function_Call(
        __name_space, _declaration_id, _parameters
    );

    if (__name_space) __ast_control->setLastSavedState();

    __ast_control->printDebugInfo("--> Node Function Call End <--");

    return _function_call_node;

}

utils::LinkedList <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getParameters(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Function Call - Parameters <--");

    utils::LinkedList <parser::Ast_Node_Expression*>* _parameters = new utils::LinkedList <parser::Ast_Node_Expression*>();

    __ast_control->current_token_position++;

    while (__ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _parameters->add(
            parser::Ast_Node_Expression::generate(
                __ast_control, NULL
            )
        );

        if (__ast_control->getToken(0)->id == TOKEN_COMMA) __ast_control->current_token_position++; 

    }

    __ast_control->current_token_position++;

    __ast_control->printDebugInfo("--> Node Function Call - Parameters End <--");

    return _parameters;

}


parser::Ast_Node_Pointer_Operator::~Ast_Node_Pointer_Operator() {
    if (value) value->~Ast_Node(); free(value);
}

parser::Ast_Node_Pointer_Operator::Ast_Node_Pointer_Operator(Ast_Node* __value, utils::LinkedList <int>* __pointer_operations) 
    : Ast_Node(AST_NODE_POINTER_OPERATOR), value(__value), pointer_level(0) {
        for (int _ = 0; _ < __pointer_operations->count; _++) pointer_level += (*__pointer_operations)[_] == TOKEN_POINTER ? 1 : -1; 
    }

parser::Ast_Node_Pointer_Operator* parser::Ast_Node_Pointer_Operator::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Pointer Operator <--");

    parser::Ast_Node_Pointer_Operator* _pointer_operator_node = (parser::Ast_Node_Pointer_Operator*) malloc(sizeof(parser::Ast_Node_Pointer_Operator));

    utils::LinkedList <int>* _pointer_operations = new utils::LinkedList <int>();

    while(__ast_control->getToken(0)->id == TOKEN_POINTER || __ast_control->getToken(0)->id == TOKEN_ADDRESS) {
        _pointer_operations->add(__ast_control->getToken(0)->id); __ast_control->current_token_position++;
    }

    new (_pointer_operator_node) parser::Ast_Node_Pointer_Operator(
        getValue(__ast_control), _pointer_operations
    );

    __ast_control->printDebugInfo("--> Node Pointer Operator End <--");

    delete _pointer_operations;

    return _pointer_operator_node;

}

parser::Ast_Node* parser::Ast_Node_Pointer_Operator::getValue(Ast_Control* __ast_control) {

    /* Supported Nodes:
    *   Ast_Node_Value //
    *   Ast_Node_Variable //
    *   Ast_Parenthesis 
    *   Ast_Node_Function_Call //
    */

    Name_Space* _name_space = parser_helper::checkIfIsNameSpaceChanging(__ast_control);

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER:
        
        if (__ast_control->getToken(1)->id == TOKEN_OPENPARENTHESES) return parser::Ast_Node_Function_Call::generate(__ast_control, _name_space);

        return parser::Ast_Node_Variable::generate(__ast_control, _name_space);

    case TOKEN_OPENPARENTHESES: return parser::Ast_Node_Parenthesis::generate(__ast_control);
    
    default: break;
    }

    if (parser::isImplicitValue(__ast_control->getToken(0))) return parser::Ast_Node_Value::generate(__ast_control);

    new Ast_Exception("Ast Node Pointer Operators -> Something went wrong");

}

  
parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() {
    if (value) value->~Ast_Node_Expression(); free(value);
}

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node_Expression* __value) : Ast_Node(AST_NODE_PARENTHESIS), value(__value) {}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("--> Node Parenthesis <--");

    parser::Ast_Node_Parenthesis* _parenthesis_node = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));

    __ast_control->current_token_position++;

    new (_parenthesis_node) parser::Ast_Node_Parenthesis(
        parser::Ast_Node_Expression::generate(__ast_control, NULL)
    );

    __ast_control->printDebugInfo("--> Node Parenthesis End <--");

    __ast_control->current_token_position++;

    return _parenthesis_node;

}


parser::Ast_Node_Accessing::~Ast_Node_Accessing() {
    if (value_before_accessing) value_before_accessing->~Ast_Node(); free(value_before_accessing);
    if (value_after_acessing) value_after_acessing->~Ast_Node(); free(value_after_acessing);
}

parser::Ast_Node_Accessing::Ast_Node_Accessing(Ast_Node* __value_before, Ast_Node* __value_after, bool __is_pointer) 
    : Ast_Node(AST_NODE_ACCESSING), value_before_accessing(__value_before), value_after_acessing(__value_after), pointer_accessing(__is_pointer) {}

parser::Ast_Node_Accessing* parser::Ast_Node_Accessing::generator(Ast_Control* __ast_control, Ast_Node* __value_before) {

    __ast_control->printDebugInfo("--> Node Accessing <--");

    parser::Ast_Node_Accessing* _accessing_node = (parser::Ast_Node_Accessing*) malloc(sizeof(parser::Ast_Node_Accessing));

    setValueBeforeNameSpace(__ast_control, __value_before);

    parser::Ast_Node* _accessing_value = getAccessingValue(__ast_control);

    std::cout << _accessing_value << std::endl;

    __ast_control->printDebugInfo("--> Node Accessing End <--");

    exit(1);

    return _accessing_node;

}

void parser::Ast_Node_Accessing::setValueBeforeNameSpace(Ast_Control* __ast_control, Ast_Node* __value_before) {

    __ast_control->printDebugInfo("--> Set Value Before Name Space <--");
    Name_Space* _name_space;

    switch (__value_before->node_id)
    {
    case AST_NODE_VARIABLE: 
        {
            Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) __value_before;

            Ast_Node_Variable_Declaration* _variable_declaration_node = parser_helper::getDeclarationOfVariable(__ast_control, _variable_node);

            Ast_Node_Struct_Declaration* _struct_declaration = parser_helper::getDeclarationOfType(__ast_control, _variable_declaration_node->type);

            _name_space = _struct_declaration->own_name_space;

            break;

        }     
    default: 
        new Ast_Exception("Unknow Node");
    }
    
    // Name_Space* _value_before_own_name_space = parser_helper::checkIfIsStructNameSpace();

    __ast_control->saveState();

    __ast_control->current_name_space = _name_space;

    __ast_control->printDebugInfo("--> Set Value Before Name Space End <--");
    
}

parser::Ast_Node* parser::Ast_Node_Accessing::getAccessingValue(Ast_Control* __ast_control) {

    /* Supported Nodes:
    *   Ast_Node_Variable 
    *   Ast_Node_Parenthesis 
    *   Ast_Node_Function_Call 
    *   Ast_Node_Pointer_Operations 
    */

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER:
        
        if (__ast_control->getToken(1)->id == TOKEN_OPENPARENTHESES) return parser::Ast_Node_Function_Call::generate(__ast_control, NULL);

        return parser::Ast_Node_Variable::generate(__ast_control, NULL);

    default: new Ast_Exception("Error");
    }

    return NULL;

}




