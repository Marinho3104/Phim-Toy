#include "./ast_nodes.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./parser_helper.h"
#include "./ast_helper.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_id) : node_id(__node_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE), name_space(__name_space) {
    declarations = new utils::Linked_List <Ast_Node*>();
}

void parser::Ast_Node_Name_Space::generate(Ast_Control* __ast_control, Name_Space* __name_space) {

    __ast_control->printDebugInfo("Ast Node Name Space Created");

    parser::Ast_Node_Name_Space* _name_space_node = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));
    new (_name_space_node) parser::Ast_Node_Name_Space(__name_space);

    __ast_control->addNameSpace(__name_space);

    _name_space_node->setDeclarations(__ast_control);

    __ast_control->name_space_nodes->add(_name_space_node);

    __ast_control->popNameSpace();

    __ast_control->printDebugInfo("Ast Node Name Space End");

}

void parser::Ast_Node_Name_Space::generate(Ast_Control* __ast_control) {

    __ast_control->current_token_position++; // namespace token

    if (__ast_control->getToken(0)->id == TOKEN_NAMESPACE_OPERATOR) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected token identifier");

    utils::Linked_List <char*>* _scope = parser_helper::getNameSpaceScope(__ast_control);

    Name_Space* _name_space = __ast_control->name_space_control->getNameSpaceOrAdd(_scope);

    delete _scope;

    if (__ast_control->getToken(0)->id != TOKEN_OPENCURLYBRACKET) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected token '{'");

    __ast_control->current_token_position++;

    generate(__ast_control, _name_space);

}

void parser::Ast_Node_Name_Space::setDeclarations(Ast_Control* __ast_control) {

    utils::Linked_List <Ast_Node*>* _temp;

    while(__ast_control->current_token_position < __ast_control->tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        switch (parser_helper::getNodeType(__ast_control))
        {
        case 0: __ast_control->current_token_position++; goto out;
        case AST_NODE_NAME_SPACE: parser::Ast_Node_Name_Space::generate(__ast_control); continue;
        case AST_NODE_VARIABLE_DECLARATION: _temp = parser::Ast_Node_Variable_Declaration::generate(__ast_control); break;
        case AST_NODE_FUNCTION_DECLARATION: declarations->add(parser::Ast_Node_Function_Declaration::generate(__ast_control)); continue;
        default: break;
        }

        if (!_temp) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Node not supportd in Name Space");

        declarations->join(_temp);

        _temp->destroy_content = 0; delete _temp;

    }

out:;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete code; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* __previous_code_block, Name_Space* __name_space)
    : Ast_Node(AST_NODE_CODE_BLOCK), previous_code_block(__previous_code_block), name_space(__name_space) { code = new utils::Linked_List <Ast_Node*>(); }

void parser::Ast_Node_Code_Block::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("Ast Node Code Block Generating");

    __ast_control->code_block_chain->last->object->setCode(__ast_control);

    __ast_control->popCodeBlockNode();

    __ast_control->printDebugInfo("Ast Node Code Block Generating End");

}

void parser::Ast_Node_Code_Block::setUp(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("Set Code Block Up");

    utils::Data_Linked_List <Ast_Node_Code_Block*>* _data_linked_list_previous_code_block = __ast_control->code_block_chain->last;

    Ast_Node_Code_Block* _previous_code_block = _data_linked_list_previous_code_block ? _data_linked_list_previous_code_block->object : NULL;

    parser::Ast_Node_Code_Block* _node_code_block = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_node_code_block) parser::Ast_Node_Code_Block(_previous_code_block, __ast_control->name_space_chain->last->object);

    __ast_control->addCodeBlockNode(_node_code_block);

    __ast_control->printDebugInfo("Set Code Block Up End");  

}

void parser::Ast_Node_Code_Block::setCode(Ast_Control* __ast_control) {

    utils::Linked_List <Ast_Node*>* _temp;

    __ast_control->current_token_position++;

    while(__ast_control->current_token_position < __ast_control->tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        switch (parser_helper::getNodeType(__ast_control))
        {
        case 0: __ast_control->current_token_position++; goto out;
        case AST_NODE_VARIABLE_DECLARATION: _temp = parser::Ast_Node_Variable_Declaration::generate(__ast_control); break;
        default: break;
        }

        if (!_temp) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Node not supportd in Code Block");

        code->join(_temp);

        _temp->destroy_content = 0; delete _temp;

    }
    
    __ast_control->popCodeBlockNode();

out:;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(parser_helper::Type_Information* __type, int __declaration_id)
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION), type(__type), declaration_id(__declaration_id) {}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast_Control* __ast_control) {

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;

    parser::Name_Space* _type_information_name_space = parser_helper::getNameSpace(__ast_control);

    parser_helper::Type_Information* _type = parser_helper::Type_Information::generate(__ast_control, _type_information_name_space);

    while (__ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        __ast_control->printDebugInfo("Ast Node Variable Declaration Created");

        if (__ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected token indentifier");

        _declaration_id = parser_helper::addName(__ast_control, __ast_control->getToken(0)->phr);
        __ast_control->current_token_position++;

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(_type, _declaration_id);

        _nodes->add(_variable_declaration);

        switch (__ast_control->getToken(0)->id)
        {
        case TOKEN_EQUAL: std::cout << "TODO assign in variable declaration" << std::endl; exit(1); break;
        case TOKEN_COMMA: __ast_control->current_token_position++; _type = parser_helper::Type_Information::generate(__ast_control, _type); break;
        case TOKEN_ENDINSTRUCTION: break;
        default: new Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token");
        }

        __ast_control->printDebugInfo("Ast Node Variable Declaration End");

    }

    __ast_control->current_token_position++;

    return _nodes;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("Function Parameter Created");
    
    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>();

    Name_Space* _type_name_space = parser_helper::getNameSpace(__ast_control);

    parser_helper::Type_Information* _type = parser_helper::Type_Information::generate(__ast_control, _type_name_space);

    int _declaration_id = -1;

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER:
            _declaration_id = parser_helper::addName(__ast_control, __ast_control->getToken(0)->phr);
            __ast_control->current_token_position++;
            break;
    case TOKEN_COMMA: case TOKEN_CLOSEPARENTHESES: break;
    default: new Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token");
    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration) Ast_Node_Variable_Declaration(_type, _declaration_id);

    _nodes->add(
        _variable_declaration
    );    

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_EQUAL: new Exception_Handle("Not done equal Function Parameter generate"); break;
    case TOKEN_COMMA: case TOKEN_CLOSEPARENTHESES: break;
    default: new Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token"); break;
    }
    
    __ast_control->printDebugInfo("Function Parameter End");

    return _nodes;

}

int parser::Ast_Node_Variable_Declaration::getByteSize() { return type->getByteSize(); }


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() { delete return_type; delete parameters; if (body) body->~Ast_Node_Code_Block(); free(body); }

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    parser_helper::Type_Information* __return_type, utils::Linked_List <Ast_Node*>* __parameters, Ast_Node_Code_Block* __body, Name_Space* __name_space, int __declaration_id)
        : Ast_Node(AST_NODE_FUNCTION_DECLARATION), return_type(__return_type), parameters(__parameters), body(__body), name_space(__name_space), declaration_id(__declaration_id) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("Ast Node Function Declaration Created");
    
    Name_Space* _return_type_name_space = parser_helper::getNameSpace(__ast_control);
    parser_helper::Type_Information* _return_type = parser_helper::Type_Information::generate(__ast_control, _return_type_name_space);

    Name_Space* _function_declaration_name_space = parser_helper::getNameSpace(__ast_control);

    if (__ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(__ast_control, __ast_control->getToken(0), "Expected token identifier");

    if (_function_declaration_name_space) __ast_control->addNameSpace(_function_declaration_name_space);

    int _declaration_id = parser_helper::addName(__ast_control, __ast_control->getToken(0)->phr);
    __ast_control->current_token_position++;

    parser::Ast_Node_Code_Block::setUp(__ast_control);
    Ast_Node_Code_Block* _body = __ast_control->code_block_chain->last->object; 

    utils::Linked_List <Ast_Node*>* _parameters = getParameters(__ast_control);

    switch (__ast_control->getToken(0)->id)
    {
    case TOKEN_ENDINSTRUCTION: 
        _body->~Ast_Node_Code_Block(); free(_body); _body = NULL; __ast_control->popCodeBlockNode(); 
        __ast_control->current_token_position++; break;
    case TOKEN_OPENCURLYBRACKET: parser::Ast_Node_Code_Block::generate(__ast_control); break;
    default: new Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token");
    }

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(_return_type, _parameters, _body, _function_declaration_name_space, _declaration_id);
    
    if (_function_declaration_name_space) __ast_control->popNameSpace();

    __ast_control->printDebugInfo("Ast Node Function Declaration End");
    
    return _function_declaration;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(Ast_Control* __ast_control) {

    __ast_control->printDebugInfo("Generating Function Parameters");

    utils::Linked_List <parser::Ast_Node*>* _parameters = new utils::Linked_List <parser::Ast_Node*>(), *_temp;
    parser_helper::Type_Information* _type;
    Name_Space* _name_space;

    __ast_control->current_token_position++;

    while(__ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _temp = parser::Ast_Node_Variable_Declaration::generateFunctionParameter(__ast_control);

        _parameters->join(_temp);

        _temp->destroy_content = 0; delete _temp;

        switch (__ast_control->getToken(0)->id)
        {
        case TOKEN_COMMA: __ast_control->current_token_position++; break;
        case TOKEN_CLOSEPARENTHESES: break;
        default: new Exception_Handle(__ast_control, __ast_control->getToken(0), "Unexpected token");
        }

    }

    __ast_control->current_token_position++;

    __ast_control->printDebugInfo("Generating Function Parameters End");

    return _parameters;

}

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }

