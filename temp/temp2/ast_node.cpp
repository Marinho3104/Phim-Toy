#include "./ast_node.h"

#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./token_definitions.h"
#include "./exception_handle.h"
#include "./ast_helper.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>

parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_id, int __declaration_id) : node_id(__node_id), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() {

}

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) {}

void parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    ast_control->printDebugInfo("Ast Node Name Space Created");

    parser::Ast_Node_Name_Space* _name_space_node = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));
    new (_name_space_node) parser::Ast_Node_Name_Space(__name_space);

    ast_control->name_space_nodes->add(_name_space_node);

    ast_control->addNameSpaceNodeToChain(_name_space_node);

    _name_space_node->setDeclarations();

    ast_control->popNameSpaceNodeOfChain();

    ast_control->printDebugInfo("Ast Node Name Space End");

}

void parser::Ast_Node_Name_Space::generate() {

    ast_control->current_token_position++;

    if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token identifier");

    utils::Linked_List <char*>* _scope = parser_helper::getScope();

    Name_Space* _name_space = name_space_control->getNameSpaceOrAdd(_scope);

    delete _scope;

    if (ast_control->getToken(0)->id != TOKEN_OPENCURLYBRACKET) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token '{'");

    ast_control->current_token_position++;

    generate(_name_space);

}

void parser::Ast_Node_Name_Space::setDeclarations() {

    utils::Linked_List <Ast_Node*>* _temp;

    while(ast_control->current_token_position < ast_control->tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        switch (parser_helper::getNodeType())
        {
        case -1: ast_control->current_token_position++; goto out;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); continue;
        default: break;
        }

        if (!_temp) new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Name Space");

        declarations->join(_temp);

        _temp->destroy_content = 0; delete _temp;

    }

out:;
}


