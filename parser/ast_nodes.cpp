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
        case -1: goto out;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); continue;  
        case AST_NODE_VARIABLE_DECLARATION: _temp = Ast_Node_Variable_Declaration::generate(); break; 
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

        if (parser_helper::getVariableDeclaration(_declaration_id)) 
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

int parser::Ast_Node_Variable_Declaration::getByteSize() { return type->getByteSize(); }


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() {
    delete return_type; delete parameters;
    if (body) body->~Ast_Node_Code_Block(); free(body);
}

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    int __declaration_id, parser_helper::Type_Information* __return_type, utils::Linked_List <Ast_Node*>* __parameters, Ast_Node_Code_Block* __body, Name_Space* __name_space)
        : Ast_Node(AST_NODE_FUNCTION_DECLARATION, __declaration_id), return_type(__return_type), parameters(__parameters), body(__body), name_space(__name_space) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate() {

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters() {

}

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }

