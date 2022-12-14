#include "./ast_nodes.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./ast_helper.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {} 

parser::Ast_Node::Ast_Node(int __node_id, int __declaration_id) : node_id(__node_id), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; } 

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) {
    declarations = new utils::Linked_List <Ast_Node*>();
}

void parser::Ast_Node_Name_Space::setDeclarations() {

    utils::Linked_List <Ast_Node*>* _temp;

    while(ast_control->current_token_position < ast_control->tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        // std::cout << parser_helper::getNodeType() << std::endl;

        switch (parser_helper::getNodeType())
        {     
        case -1: ast_control->current_token_position++; goto out;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); continue;  
        case AST_NODE_VARIABLE_DECLARATION: _temp = Ast_Node_Variable_Declaration::generate(); break; 
        case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate()); continue;
        default: break;
        }

        if (!_temp) new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Name Space");

        declarations->join(_temp);

        _temp->destroy_content = 0; delete _temp;
    
    }

out:;

}

void parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    ast_control->printDebugInfo("Ast Node Name Space Created");

    parser::Ast_Node_Name_Space* _name_space_node = (parser::Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));
    new (_name_space_node) parser::Ast_Node_Name_Space(__name_space);

    ast_control->addNameSpaceNode(_name_space_node);
    ast_control->addNameSpaceToChain(_name_space_node);

    _name_space_node->setDeclarations();

    ast_control->popNameSpaceFromChain();

    ast_control->printDebugInfo("Ast Node Name Space End");

}

void parser::Ast_Node_Name_Space::generate() {

    ast_control->current_token_position++;

    if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token identifier");

    Name_Space* _name_space = parser_helper::getNameSpace(1);

    if (ast_control->getToken(0)->id != TOKEN_OPENCURLYBRACKET) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token '{'");

    ast_control->current_token_position++;

    generate(_name_space);

}

int parser::Ast_Node_Name_Space::getDeclarationId(char* __name) {

    int _declaration_id = name_space->declaration_tracker->getDeclarationId(__name);

    if (_declaration_id != -1) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) return _previous->declaration_tracker->getDeclarationId(__name);

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Name_Space::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = name_space->declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) return _previous->declaration_tracker->getVariableDeclaration(__declaration_id);

    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Name_Space::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = name_space->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) return _previous->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Name_Space::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = name_space->declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) return _previous->declaration_tracker->getStructDeclaration(__declaration_id);

    }

    return _struct_declaration;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { free(declarations_off); delete code; delete declaration_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* __previous_block, Name_Space* __name_space) 
    : Ast_Node(AST_NODE_CODE_BLOCK, -1), previous_code_block(__previous_block), name_space(__name_space) { 
        declarations_off = (int*) malloc(4);
        *declarations_off = previous_code_block ? *previous_code_block->declarations_off : 0;
        declaration_tracker = new Declaration_Tracker(); declaration_tracker->off = declarations_off;
        code = new utils::Linked_List <Ast_Node*>(); 
}

void parser::Ast_Node_Code_Block::setCode() {

    utils::Linked_List <Ast_Node*>* _temp;

    while(ast_control->current_token_position < ast_control->tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        // std::cout << parser_helper::getNodeType() << std::endl;

        switch (parser_helper::getNodeType())
        {     
        case -1: ast_control->current_token_position++; goto out;
        case AST_NODE_VARIABLE_DECLARATION: _temp = Ast_Node_Variable_Declaration::generate(); break; 
        default: break;
        }

        if (!_temp) new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Code Block");

        code->join(_temp);

        _temp->destroy_content = 0; delete _temp;
    
    }

out:;


}

void parser::Ast_Node_Code_Block::setUp() {

    ast_control->printDebugInfo("Set Code Block up");

    utils::Data_Linked_List <Ast_Node_Code_Block*>* _data_linked_list_previous_code_block = ast_control->code_block_chain->last;
    Ast_Node_Code_Block* _previous_code_block = _data_linked_list_previous_code_block ? _data_linked_list_previous_code_block->object : NULL;

    parser::Ast_Node_Code_Block* _node_code_block = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_node_code_block) parser::Ast_Node_Code_Block(_previous_code_block, ast_control->name_space_chain->last->object->name_space);

    if (!_previous_code_block) *_node_code_block->declarations_off = name_space_control->declarations_off;

    ast_control->addCodeBlockToChain(_node_code_block);

    ast_control->printDebugInfo("Set Code Block up end");

}

void parser::Ast_Node_Code_Block::generate() {

    ast_control->printDebugInfo("Generate Code Block Body");

    ast_control->code_block_chain->last->object->setCode();

    ast_control->popCodeBlockFromChain();

    ast_control->printDebugInfo("Generate Code Block Body End");

}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __name) {

    int _declaration_id = declaration_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        if (previous_code_block) return previous_code_block->getDeclarationId(__name);

        return name_space->declaration_tracker->getDeclarationId(__name);

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        if (previous_code_block) return previous_code_block->getVariableDeclaration(__declaration_id);

        return name_space->declaration_tracker->getVariableDeclaration(__declaration_id);

    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Code_Block::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        if (previous_code_block) return previous_code_block->getFunctionDeclaration(__declaration_id, __parameters);

        return name_space->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Code_Block::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        if (previous_code_block) return previous_code_block->getStructDeclaration(__declaration_id);

        return name_space->declaration_tracker->getStructDeclaration(__declaration_id);

    }

    return _struct_declaration;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int __declaration_id, parser_helper::Type_Information* __type) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __declaration_id), type(__type) {}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate() {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>();

    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;

    parser_helper::Type_Information* _type = parser_helper::Type_Information::generate();

    while(ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION) {

        ast_control->printDebugInfo("Ast Node Variable Declaration Created");

        if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token indentifier");

        _declaration_id = parser_helper::addName(ast_control->getToken(0)->phr);
        ast_control->current_token_position++;

        if (parser_helper::getVariableDeclaration(_declaration_id, 1)) 
            new Exception_Handle(ast_control, ast_control->getToken(0), "Redefinition of variable name");

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

        parser_helper::addVariableDeclaration(_variable_declaration);
        _nodes->add(_variable_declaration);

        switch (ast_control->getToken(0)->id)
        {
        case TOKEN_EQUAL: std::cout << "TODO assign in variable declaration" << std::endl; exit(1); break;
        case TOKEN_COMMA: ast_control->current_token_position++; _type = parser_helper::Type_Information::generate(_type); break;
        case TOKEN_ENDINSTRUCTION: break;
        default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token");
        }

        ast_control->printDebugInfo("Ast Node Variable Declaration End");

    }

    ast_control->current_token_position++;

    return _nodes;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter() {

    ast_control->printDebugInfo("Function Parameter Created");
    
    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>();

    parser_helper::Type_Information* _type = parser_helper::Type_Information::generate();

    int _declaration_id = -1;

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER:
            _declaration_id = parser_helper::addName(ast_control->getToken(0)->phr);
            ast_control->current_token_position++;
            if (parser_helper::getVariableDeclaration(_declaration_id, 1)) 
                new Exception_Handle(ast_control, ast_control->getToken(0), "Redefinition of variable name");
            break;
    case TOKEN_COMMA: case TOKEN_CLOSEPARENTHESES: break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token");
    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

    _nodes->add(
        _variable_declaration
    );

    parser_helper::addVariableDeclaration(
        _variable_declaration
    );

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_EQUAL: new Exception_Handle("Not done equal Function Parameter generate"); break;
    case TOKEN_COMMA: case TOKEN_CLOSEPARENTHESES: break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token"); break;
    }

    ast_control->printDebugInfo("Function Parameter End");

    return _nodes;

}

int parser::Ast_Node_Variable_Declaration::getByteSize() { return type->getByteSize(); }


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() {
    delete return_type; delete parameters;
    if (body) body->~Ast_Node_Code_Block(); free(body);
}

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    int __declaration_id, parser_helper::Type_Information* __return_type, utils::Linked_List <Ast_Node*>* __parameters, Ast_Node_Code_Block* __body, Name_Space* __name_space)
        : Ast_Node(AST_NODE_FUNCTION_DECLARATION, __declaration_id), return_type(__return_type), parameters(__parameters), body(__body), name_space(__name_space) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate() {

    ast_control->printDebugInfo("Ast Node Function Declaration Created");
    
    parser_helper::Type_Information* _return_type = parser_helper::Type_Information::generate();

    Name_Space* _function_declaration_name_space = parser_helper::getNameSpace(0);

    if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token identifier");

    if (_function_declaration_name_space) ast_control->addNameSpaceToChain(_function_declaration_name_space);

    int _backup = ast_control->current_token_position;

    int _declaration_id = parser_helper::addName(ast_control->getToken(0)->phr);
    ast_control->current_token_position++;

    parser::Ast_Node_Code_Block::setUp();
    Ast_Node_Code_Block* _body = ast_control->code_block_chain->last->object; 

    utils::Linked_List <Ast_Node*>* _parameters = getParameters();

    ast_control->addCodeBlockToChain(NULL);

    parser::Ast_Node_Function_Declaration* _previous_function_declaration = parser_helper::getFunctionDeclaration(_declaration_id, _parameters, 1);

    if (_previous_function_declaration && _previous_function_declaration->body) 
        new Exception_Handle(ast_control, ast_control->getToken(0), "Redefinition of Function Name");

    ast_control->popCodeBlockFromChain();

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_ENDINSTRUCTION: 
        _body->~Ast_Node_Code_Block(); free(_body); _body = NULL; ast_control->popCodeBlockFromChain(); 
        ast_control->current_token_position++; break;
    case TOKEN_OPENCURLYBRACKET: ast_control->current_token_position++; parser::Ast_Node_Code_Block::generate(); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token");
    }

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(_declaration_id, _return_type, _parameters, _body, _function_declaration_name_space);
    
    if (_function_declaration_name_space) ast_control->popNameSpaceFromChain();

    parser_helper::addFunctionDeclaration(_function_declaration);

    std::cout << "Function declaration id -> " << _declaration_id << std::endl;

    ast_control->printDebugInfo("Ast Node Function Declaration End");
    
    return _function_declaration;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters() {

    ast_control->printDebugInfo("Generating Function Parameters");

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>(), *_temp;

    ast_control->current_token_position++;

    while(ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _temp = parser::Ast_Node_Variable_Declaration::generateFunctionParameter();

        _nodes->join(
            _temp
        );

        _temp->destroy_content = 0; delete _temp;

        switch (ast_control->getToken(0)->id)
        {
        case TOKEN_COMMA: ast_control->current_token_position++; break;
        case TOKEN_CLOSEPARENTHESES: break;
        default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token");
        }

    }

    ast_control->current_token_position++;

    ast_control->printDebugInfo("Generating Function Parameters End");

    return _nodes;


}

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }

