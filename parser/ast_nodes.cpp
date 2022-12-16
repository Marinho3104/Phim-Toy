#include "./ast_nodes.h"

#include "./../compiler/built_in.h"
#include "./../utils/linkedList.h"
#include "./parser_definitions.h"
#include "./exception_handle.h"
#include "./ast_helper.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./ast.h"

#include <iostream>
#include <string.h>

parser::Ast_Node::~Ast_Node() {} 

parser::Ast_Node::Ast_Node(int __node_id, int __declaration_id) : node_id(__node_id), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; } 

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) {
    declarations = new utils::Linked_List <Ast_Node*>();
}

void parser::Ast_Node_Name_Space::setDeclarations() {

    utils::Linked_List <Ast_Node*>* _temp;
    int _node_type;

    while(ast_control->current_token_position < tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        // std::cout << parser_helper::getNodeType() << std::endl;

        switch (_node_type = parser_helper::getNodeType())
        {     
        case -1: ast_control->current_token_position++; goto out;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); continue;  
        case AST_NODE_VARIABLE_DECLARATION: _temp = Ast_Node_Variable_Declaration::generate(); break; 
        case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate()); continue;
        case AST_NODE_STRUCT_DECLARATION: declarations->add(Ast_Node_Struct_Declaration::generate()); continue;
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

    if (_declaration_id == -1) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) {

            
            Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(_previous);

            return _name_space_node->getDeclarationId(__name);
        
        }   
    
    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Name_Space::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = name_space->declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) {

            Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(_previous);

            return _name_space_node->getVariableDeclaration(__declaration_id);
        
        }   
    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Name_Space::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = name_space->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);
        
        if (_previous) {

            
            Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(_previous);

            return _name_space_node->getFunctionDeclaration(__declaration_id, __parameters);
        
        }   
    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Name_Space::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = name_space->declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        parser::Name_Space* _previous = name_space_control->getPreviousNameSpace(name_space);

        if (_previous) {

            Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(_previous);

            return _name_space_node->getStructDeclaration(__declaration_id);
        
        }   
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
    int _node_type;

    while(ast_control->current_token_position < tokenizer_control->tokens_collection->count) {

        _temp = NULL;

        // std::cout << parser_helper::getNodeType() << std::endl;

        switch (_node_type = parser_helper::getNodeType())
        {     
        case -1: ast_control->current_token_position++; goto out;
        case -2: ast_control->current_token_position++; 
            Ast_Node_Code_Block::setUp(); code->add(ast_control->code_block_chain->last->object); 
            Ast_Node_Code_Block::generate(); continue;
        case AST_NODE_VARIABLE_DECLARATION: _temp = Ast_Node_Variable_Declaration::generate(); break; 
        case AST_NODE_VALUE: case AST_NODE_VARIABLE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATOR: case AST_NODE_PARENTHESIS:
        case AST_NODE_ASSIGNMENT:
            code->add(
                Ast_Node_Expression::generate(
                    _node_type
                )
            ); 
            goto check;
        default: break;
        }

        if (!_temp) new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Code Block");

        code->join(_temp);

        _temp->destroy_content = 0; delete _temp;

        continue;

    check:

        if (ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION)

            parser::Exception_Handle(ast_control, ast_control->getToken(0), "Expected ';' token Code Block");

        ast_control->current_token_position++;
    
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

        Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(name_space);

        return _name_space_node->getDeclarationId(__name);

    }

    return _declaration_id;

}

bool parser::Ast_Node_Code_Block::isGlobalDeclaration(char* __name) {

    if (declaration_tracker->getDeclarationId(__name) == -1) {

        if (previous_code_block) return previous_code_block->isGlobalDeclaration(__name);

        return 1;

    }

    return 0;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        if (previous_code_block) return previous_code_block->getVariableDeclaration(__declaration_id);

        Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(name_space);

        return _name_space_node->getVariableDeclaration(__declaration_id);

    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Code_Block::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        if (previous_code_block) return previous_code_block->getFunctionDeclaration(__declaration_id, __parameters);

        Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(name_space);

        return _name_space_node->getFunctionDeclaration(__declaration_id, __parameters);
    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Code_Block::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        if (previous_code_block) return previous_code_block->getStructDeclaration(__declaration_id);


        Ast_Node_Name_Space* _name_space_node = ast_control->getNameSpaceNodeFromNameSpace(name_space);

        return _name_space_node->getStructDeclaration(__declaration_id);
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

       //std::cout << "Declaration -> " << _declaration_id << std::endl;

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

        parser_helper::addVariableDeclaration(_variable_declaration);
        _nodes->add(_variable_declaration);

        switch (ast_control->getToken(0)->id)
        {
        case TOKEN_EQUAL://std::cout << "TODO assign in variable declaration" << std::endl; exit(1); break;
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

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(_declaration_id, _return_type, _parameters, _body, _function_declaration_name_space);
    
    ast_control->addCodeBlockToChain(NULL);

    parser_helper::addFunctionDeclaration(_function_declaration);

    ast_control->popCodeBlockFromChain();

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_ENDINSTRUCTION: 
        _body->~Ast_Node_Code_Block(); free(_body); _function_declaration->body = NULL; ast_control->popCodeBlockFromChain(); 
        ast_control->current_token_position++; break;
    case TOKEN_OPENCURLYBRACKET: ast_control->current_token_position++; parser::Ast_Node_Code_Block::generate(); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token");
    }


    if (_function_declaration_name_space) ast_control->popNameSpaceFromChain();

   //std::cout << "Function declaration id -> " << _declaration_id << std::endl;

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


parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() {
    if (!defined) return;
    if (fields) fields->~Ast_Node_Code_Block(); free(fields);
    // if (functions) delete functions->name_space;
}

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(int __declaration_id, Ast_Node_Name_Space* __functions, Ast_Node_Code_Block* __fields, bool __defined) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION, __declaration_id), functions(__functions), fields(__fields),  defined(__defined) {}

void parser::Ast_Node_Struct_Declaration::set(Ast_Node_Name_Space* __functions, Ast_Node_Code_Block* __fields) {

    ast_control->printDebugInfo("Set Fields and Functions of Struct Created");

    int _backup = ast_control->current_token_position;
    utils::Linked_List <Ast_Node*>* _temp;

    // Fields
   //std::cout << "Fields " << std::endl;
    while(ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

       //std::cout << parser_helper::getNodeType() << std::endl;

        switch (parser_helper::getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION:

            ast_control->addCodeBlockToChain(__fields);

            _temp = Ast_Node_Variable_Declaration::generate();

            ast_control->popCodeBlockFromChain();

            __fields->code->join(
                _temp
            );

            _temp->destroy_content = 0; delete _temp;

            break;

        case AST_NODE_FUNCTION_DECLARATION:

            while(ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) ast_control->current_token_position++;

            ast_control->current_token_position++;

            if (ast_control->getToken(0)->id == TOKEN_OPENCURLYBRACKET) ignoreCodeBlock();

            ast_control->current_token_position++;

            break;
        
        default: new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in struct declaration"); break;
        }

    }

    ast_control->current_token_position = _backup;

    // Functions
   //std::cout << "Functions " << std::endl;
    while(ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        switch (parser_helper::getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION:

            while(ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION) ast_control->current_token_position++;

            ast_control->current_token_position++; 

            break;

        case AST_NODE_FUNCTION_DECLARATION:

            ast_control->addNameSpaceToChain(__functions);
            ast_control->addCodeBlockToChain(NULL);

            __functions->declarations->add(
                parser::Ast_Node_Function_Declaration::generate()
            );

            ast_control->popNameSpaceFromChain();
            ast_control->popCodeBlockFromChain();

            break;
        
        default: new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in struct declaration"); break;
        }

    }

    ast_control->current_token_position++;

    ast_control->printDebugInfo("Set Fields and Functions of Struct Created");

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate() {

    ast_control->printDebugInfo("Ast Node Struct Declaration Creation");

    Ast_Node_Struct_Declaration* _struct_declaration;
    int _declaration_id;
    bool _defined = 1;

    ast_control->current_token_position++;

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_IDENTIFIER:
        
        _declaration_id = parser_helper::addName(ast_control->getToken(0)->phr); // ast_control->current_token_position++;
    
        _struct_declaration = parser_helper::getStructDeclaration(_declaration_id, 0);

        if (_struct_declaration && _struct_declaration->defined) 

            new Exception_Handle(ast_control, ast_control->getToken(0), "Redefinition Struct name");

        break;
    case TOKEN_OPENCURLYBRACKET: new Exception_Handle("Not done struct declaration"); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token"); break;
    }

    Ast_Node_Name_Space* _functions;
    Ast_Node_Code_Block* _fields;

    if (!_struct_declaration) {

        utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>(),
        *_current_name_space_scope = ast_control->name_space_chain->last->object->name_space->scope;

        Name_Space* _struct_name_space = (Name_Space*) malloc(sizeof(Name_Space));
        new (_struct_name_space) Name_Space(_scope);

        for (int _ = 0; _ < _current_name_space_scope->count; _++) {

            _scope->add(NULL);

            utils::Data_Linked_List <char*>* _data_linked_list = _scope->getDataLinkedList(_);

            _data_linked_list->object = (char*) malloc(strlen(_current_name_space_scope->operator[](_)) + 1);

            strcpy(_data_linked_list->object, _current_name_space_scope->operator[](_));

        }

        _scope->add(NULL);

        utils::Data_Linked_List <char*>* _data_linked_list = _scope->getDataLinkedList(_scope->count - 1);

        _data_linked_list->object = (char*) malloc(strlen(ast_control->getToken(0)->phr) + 1);

        strcpy(_data_linked_list->object, ast_control->getToken(0)->phr);

        _functions = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));
        new (_functions) Ast_Node_Name_Space(_struct_name_space);
        _functions->name_space->declaration_tracker->off = &name_space_control->declarations_off;

        _fields = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));
        new (_fields) Ast_Node_Code_Block(
            ast_control->code_block_chain->last ? ast_control->code_block_chain->last->object : NULL,
            ast_control->name_space_chain->last->object->name_space
        );

        ast_control->name_space_nodes->add(_functions);
        name_space_control->addNameSpace(_struct_name_space);

        _struct_name_space->scope->printContent();
 
    } else {

        _functions = _struct_declaration->functions;
        _fields = _struct_declaration->fields;

    }

    ast_control->current_token_position++;

    switch (ast_control->getToken(0)->id)
    {
    case TOKEN_OPENCURLYBRACKET: ast_control->current_token_position++; set(_functions, _fields); break; 
    case TOKEN_ENDINSTRUCTION: _defined = false; break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token"); break;
    }

    if (ast_control->getToken(0)->id != TOKEN_ENDINSTRUCTION) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token ';'");
    ast_control->current_token_position++;

    parser::Ast_Node_Struct_Declaration* _struct_declaration_return = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));
    new (_struct_declaration_return) parser::Ast_Node_Struct_Declaration(_declaration_id, _functions, _fields, _defined);

    parser_helper::addStructDeclaration(_struct_declaration_return);

    ast_control->printDebugInfo("Ast Node Struct Declaration End");

    return _struct_declaration_return;

}

void parser::Ast_Node_Struct_Declaration::ignoreCodeBlock() {

    while(ast_control->getToken(0)->id != TOKEN_CLOSECURLYBRACKET) {

        ast_control->current_token_position++;
    
        if (ast_control->getToken(0)->id == TOKEN_OPENCURLYBRACKET) ignoreCodeBlock();

    }

}

int parser::Ast_Node_Struct_Declaration::getByteSize() {

    int _byte_size = 0;

    for (int _ = 0; _ < fields->code->count; _++) {

        if (fields->code->operator[](_)->node_id != AST_NODE_VARIABLE_DECLARATION) continue;

        _byte_size += ((Ast_Node_Variable_Declaration*) fields->code->operator[](_))->getByteSize();

    }

    return _byte_size;

}


parser::Ast_Node_Expression::~Ast_Node_Expression() {
    if (value) value->~Ast_Node(); free(value);
    if (expression) expression->~Ast_Node_Expression(); free(expression);
}
 
parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* __value, Ast_Node_Expression* __expression, int __operation)
    : Ast_Node(AST_NODE_EXPRESSION, -1), value(__value), expression(__expression), operator_id(__operation) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(int __value_node_type) {

    ast_control->printDebugInfo("Ast Node Expression Created");
    
    parser::Ast_Node_Expression* _expression = NULL;
    Ast_Node* _value = getValue(__value_node_type);
    int _operator_id;

    while(checkNext(&_value));

    _operator_id = ast_control->getToken(0)->id;

    switch (parser_helper::getNodeType())
    {
    case AST_NODE_EXPRESSION:
        
        ast_control->current_token_position++;

        _expression = Ast_Node_Expression::generate(
            parser_helper::getNodeType()
        );
    
    default: break;
    }

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    new (_expression_node) parser::Ast_Node_Expression(_value, _expression, _operator_id);

    ast_control->printDebugInfo("Ast Node Expression End");

    return _expression_node;

}

parser::Ast_Node* parser::Ast_Node_Expression::getValue(int __value_node_type) {

    switch (__value_node_type)
    {
    case AST_NODE_VALUE: return Ast_Node_Value::generate(); break;
    case AST_NODE_VARIABLE: return Ast_Node_Variable::generate(); break;
    case AST_NODE_FUNCTION_CALL: return Ast_Node_Function_Call::generate(); break;
    case AST_NODE_POINTER_OPERATOR: return Ast_Node_Pointer_Operator::generate(); break;
    case AST_NODE_PARENTHESIS: return Ast_Node_Parenthesis::generate(); break;
    case AST_NODE_ASSIGNMENT: return Ast_Node_Assignment::generate(); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Expression dont support given node");
    }

    return NULL;

}

bool parser::Ast_Node_Expression::checkNext(Ast_Node** __value) {

    switch (parser_helper::getNodeType())
    {
    case AST_NODE_ASSIGNMENT: *__value = Ast_Node_Assignment::generate(*__value); break;
    default: return 0; break;
    }

    return 1;

}


parser::Ast_Node_Value::~Ast_Node_Value() {} 

parser::Ast_Node_Value::Ast_Node_Value(int __implicit_value_position, int __token_id)
    : Ast_Node(AST_NODE_VALUE, -1), implicit_value_position(__implicit_value_position), token_id(__token_id) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate() {

    ast_control->printDebugInfo("Ast Node Value Created");
    
    parser::Ast_Node_Value* _value_node = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new (_value_node) parser::Ast_Node_Value(
        ast_control->addImplicitValue(ast_control->getToken(0)->phr),
        ast_control->getToken(0)->id
    );

    ast_control->current_token_position++;

    ast_control->printDebugInfo("Ast Node Value End");

    return _value_node;

}


parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(int __declaration_id, Ast_Node_Variable_Declaration* __declaration, bool __is_global)
    : Ast_Node(AST_NODE_VARIABLE, __declaration_id), declaration(__declaration), is_global(__is_global) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate() {

    ast_control->printDebugInfo("Ast Node Variable Created");
        
    Name_Space* _name_space = parser_helper::getNameSpace(0);

    if (_name_space) { ast_control->addNameSpaceToChain(_name_space); ast_control->addCodeBlockToChain(NULL); }

    if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new parser::Exception_Handle(ast_control, ast_control->getToken(0), "Expected token identifier");

    int _declaration_id = parser_helper::getDeclarationId(ast_control->getToken(0)->phr, 0);
    Ast_Node_Variable_Declaration* _declaration = parser_helper::getVariableDeclaration(_declaration_id, 0);

    if (_declaration_id == -1 || !_declaration) new Exception_Handle(ast_control, ast_control->getToken(0), "Undefined variable with given name");

    bool _is_global = parser_helper::isGlobalDeclaration(ast_control->getToken(0)->phr);
    ast_control->current_token_position++;

    parser::Ast_Node_Variable* _variable_node = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));
    new (_variable_node) parser::Ast_Node_Variable(
        _declaration_id, _declaration, _is_global
    );

   //std::cout << "Variable declaration id -> " << _declaration_id << std::endl;
   //std::cout << "Variable is -> " << (_is_global ? "global" : "local") << std::endl;

    if (_name_space) { ast_control->popNameSpaceFromChain(); ast_control->popCodeBlockFromChain(); }

    ast_control->printDebugInfo("Ast Node Variable End");

    return _variable_node;

}


parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(
    int __declaration_id, utils::Linked_List <Ast_Node_Expression*>* __parameters, Ast_Node_Function_Declaration* __declaration) 
        : Ast_Node(AST_NODE_FUNCTION_CALL, __declaration_id), parameters(__parameters), declaration(__declaration) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate() {

    ast_control->printDebugInfo("Ast Node Function Call Created");
    
    Name_Space* _function_call_name_space = parser_helper::getNameSpace(0);

    if (_function_call_name_space) { ast_control->addNameSpaceToChain(_function_call_name_space); ast_control->addCodeBlockToChain(NULL); }

    if (ast_control->getToken(0)->id != TOKEN_IDENTIFIER) new Exception_Handle(ast_control, ast_control->getToken(0), "Expected token identifier");

    int _declaration_id = parser_helper::getDeclarationId(ast_control->getToken(0)->phr, 0);
    ast_control->current_token_position++;
    
    utils::Linked_List <Ast_Node_Expression*>* _parameters_expressions = getParameters();

    utils::Linked_List <Ast_Node*>* _parameters_variable_declarations = getParametersFromExpressions(_parameters_expressions);

    Ast_Node_Function_Declaration* _function_declaration = parser_helper::getFunctionDeclaration(_declaration_id, _parameters_variable_declarations, 0);

    if (_declaration_id == -1 || !_function_declaration)
    
        new Exception_Handle(ast_control, ast_control->getToken(0), "Undefined function with given name and parameters");

    parser::Ast_Node_Function_Call* _function_call_node = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));
    new (_function_call_node) parser::Ast_Node_Function_Call(_declaration_id, _parameters_expressions, _function_declaration);
    
    if (_function_call_name_space) { ast_control->popNameSpaceFromChain(); ast_control->popCodeBlockFromChain(); }

    delete _parameters_variable_declarations; // normal delete or destroy_content

    ast_control->printDebugInfo("Ast Node Function Call End");

    return _function_call_node;

}

utils::Linked_List <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getParameters() {

    utils::Linked_List <parser::Ast_Node_Expression*>* _parameters = new utils::Linked_List <parser::Ast_Node_Expression*>();

    ast_control->current_token_position++;

    while(ast_control->getToken(0)->id != TOKEN_CLOSEPARENTHESES) {

        _parameters->add(
            parser::Ast_Node_Expression::generate(
                parser_helper::getNodeType()
            )
        );

        switch (parser_helper::getNodeType())
        {
        case -4: break;
        case -5: ast_control->current_token_position++; break;
        default: new Exception_Handle(ast_control, ast_control->getToken(0), "Unexpected token"); break;
        }


    }

    ast_control->current_token_position++;

    return _parameters;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getParametersFromExpressions(utils::Linked_List <Ast_Node_Expression*>* __expression_parameters) {


    utils::Linked_List <parser::Ast_Node*>* _parameters_variable_declarations = new utils::Linked_List <parser::Ast_Node*>();
    parser_helper::Type_Information* _type_information;

    for (int _ = 0; _ < __expression_parameters->count; _++) {

        _type_information = compiler::Built_In::getReturnTypeOfExpression(__expression_parameters->operator[](_));

    }

    return _parameters_variable_declarations;

}


parser::Ast_Node_Pointer_Operator::~Ast_Node_Pointer_Operator() { if (value) value->~Ast_Node(); free(value); }

parser::Ast_Node_Pointer_Operator::Ast_Node_Pointer_Operator(int __pointer_level, Ast_Node* __value) 
    : Ast_Node(AST_NODE_POINTER_OPERATOR, -1), pointer_level(__pointer_level), value(__value) {}

parser::Ast_Node_Pointer_Operator* parser::Ast_Node_Pointer_Operator::generate() {

    ast_control->printDebugInfo("Ast Node Pointer Operator Created");
    
    int _pointer_level = 0;
    Ast_Node* _value;

    while(ast_control->getToken(0)->id == TOKEN_POINTER || ast_control->getToken(0)->id == TOKEN_ADDRESS) {

        _pointer_level += ast_control->getToken(0)->id == TOKEN_POINTER ? 1 : -1;

        ast_control->current_token_position++;

    }
    
    switch (parser_helper::getNodeType())
    {
    case AST_NODE_VARIABLE: _value = Ast_Node_Variable::generate(); break;
    // case AST_NODE_VALUE: _value = Ast_Node_Value::generate(); break;
    case AST_NODE_FUNCTION_CALL: _value = Ast_Node_Function_Call::generate(); break;
    case AST_NODE_PARENTHESIS: _value = Ast_Node_Parenthesis::generate(); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Pointer Operator");
    }

    parser::Ast_Node_Pointer_Operator* _pointer_operator_node = (parser::Ast_Node_Pointer_Operator*) malloc(sizeof(parser::Ast_Node_Pointer_Operator));
    new (_pointer_operator_node) parser::Ast_Node_Pointer_Operator(
        _pointer_level, _value
    );

    ast_control->printDebugInfo("Ast Node Pointer Operator End");

    return _pointer_operator_node;

}


parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() { if (expression) expression->~Ast_Node_Expression(); free(expression); }

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node_Expression* __expression) : Ast_Node(AST_NODE_PARENTHESIS, -1), expression(__expression) {}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate() {

    ast_control->printDebugInfo("Ast Node Parenthesis Created");

    ast_control->current_token_position++;

    parser::Ast_Node_Parenthesis* _parenthesis_node = (parser::Ast_Node_Parenthesis*) malloc(sizeof(parser::Ast_Node_Parenthesis));

    new (_parenthesis_node) parser::Ast_Node_Parenthesis(
        Ast_Node_Expression::generate(
            parser_helper::getNodeType()
        )
    );

    ast_control->current_token_position++;

    ast_control->printDebugInfo("Ast Node Parenthesis End");

    return _parenthesis_node;

}


parser::Ast_Node_Assignment::~Ast_Node_Assignment() {
    if (target) target->~Ast_Node(); free(target);
    if (expression) expression->~Ast_Node_Expression(); free(expression);
}

parser::Ast_Node_Assignment::Ast_Node_Assignment(Ast_Node* __target, Ast_Node_Expression* __expression, int __token_id) 
    : Ast_Node(AST_NODE_ASSIGNMENT, -1), target(__target), expression(__expression), token_id(__token_id) {}

parser::Ast_Node_Assignment* parser::Ast_Node_Assignment::generate(Ast_Node* __target) {

    ast_control->printDebugInfo("Ast Node Assignment Created");

    int _token_id = ast_control->getToken(0)->id;
    ast_control->current_token_position++;

    parser::Ast_Node_Assignment* _assignment_node = (parser::Ast_Node_Assignment*) malloc(sizeof(parser::Ast_Node_Assignment)); 
    new (_assignment_node) parser::Ast_Node_Assignment(
        __target,
        parser::isSingleAssignmentOperator(_token_id) ? NULL : parser::Ast_Node_Expression::generate(parser_helper::getNodeType()),
        _token_id
    );

    ast_control->printDebugInfo("Ast Node Assignment End");

    return _assignment_node;

}

parser::Ast_Node_Assignment* parser::Ast_Node_Assignment::generate() {

    ast_control->printDebugInfo("Ast Node Assignment Created");

    int _token_id = ast_control->getToken(0)->id;
    ast_control->current_token_position++;

    Ast_Node* _value;

    switch (parser_helper::getNodeType())
    {
    case AST_NODE_VARIABLE: _value = Ast_Node_Variable::generate(); break;
    case AST_NODE_VALUE: _value = Ast_Node_Value::generate(); break;
    case AST_NODE_FUNCTION_CALL: _value = Ast_Node_Function_Call::generate(); break;
    case AST_NODE_PARENTHESIS: _value = Ast_Node_Parenthesis::generate(); break;
    default: new Exception_Handle(ast_control, ast_control->getToken(0), "Node not supported in Node Assignment");
    }

    parser::Ast_Node_Assignment* _assignment_node = (parser::Ast_Node_Assignment*) malloc(sizeof(parser::Ast_Node_Assignment)); 
    new (_assignment_node) parser::Ast_Node_Assignment(
        _value,
        NULL,
        _token_id
    );

    ast_control->printDebugInfo("Ast Node Assignment End");

    return _assignment_node;

}


