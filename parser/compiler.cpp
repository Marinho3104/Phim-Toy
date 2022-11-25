#include "./compiler.h"

#include "./../byteCode/byteCode.h" // Byte Code  
#include "./../utils/linkedList.h" // Linked List
#include "./ast_nodes.h" // Ast Nodes
#include "./parser_definitions.h" // Parser definitions
#include "./compiler_byte_code_converter.h"
#include "./ast.h"

#include <iostream>

parser::Compiler_Declarations::~Compiler_Declarations() {
    functions_declaraction->destroy_content = 0; delete functions_declaraction;
    variable_declaration->destroy_content = 0; delete variable_declaration;
    structs_declaration->destroy_content = 0; delete structs_declaration;
}

parser::Compiler_Declarations::Compiler_Declarations() {
    functions_declaraction = new utils::LinkedList <Ast_Node_Function_Declaration*>();
    variable_declaration = new utils::LinkedList <Ast_Node_Variable_Declaration*>();
    structs_declaration = new utils::LinkedList <Ast_Node_Struct_Declaration*>();
}

bool parser::Compiler_Declarations::isDeclared(int __d) {

    return (
        getVariableDeclaration(__d) ||
        getFunctionDeclaration(__d) ||
        getStructDeclaration(__d)
    );

}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Declarations::getVariableDeclaration(int __d) {
    parser::Ast_Node_Variable_Declaration* _astVarDecl = NULL;
    for (int _ = 0; _ < variable_declaration->count && !_astVarDecl; _++)
        if (
            (*variable_declaration)[_]->declaration_id == __d
        ) _astVarDecl = (*variable_declaration)[_];
    return _astVarDecl;
}

parser::Ast_Node_Function_Declaration* parser::Compiler_Declarations::getFunctionDeclaration(int __d) {
    parser::Ast_Node_Function_Declaration* _astFuncDecl = NULL;
    for (int _ = 0; _ < functions_declaraction->count && !_astFuncDecl; _++)
        if (
            (*functions_declaraction)[_]->declaration_id == __d
        ) _astFuncDecl = (*functions_declaraction)[_];
    return _astFuncDecl;
}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Declarations::getStructDeclaration(int __d) {
    parser::Ast_Node_Struct_Declaration* _astStrctDecl = NULL;
    for (int _ = 0; _ < structs_declaration->count && !_astStrctDecl; _++)
        if (
            (*structs_declaration)[_]->declaration_id == __d
        ) _astStrctDecl = (*structs_declaration)[_];
    return _astStrctDecl;
}

parser::Compiler_Code_Block::~Compiler_Code_Block() { delete compiler_declarations; }

parser::Compiler_Code_Block::Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* _byteCode) : byte_code(_byteCode) {}

parser::Ast_Node_Variable_Declaration* parser::Compiler_Code_Block::getVariableDeclaration(int __d) {

    parser::Ast_Node_Variable_Declaration* _ = compiler_declarations->getVariableDeclaration(__d);

    return (_ || !previous_block) ? _ : previous_block->getVariableDeclaration(__d);

}

parser::Ast_Node_Function_Declaration* parser::Compiler_Code_Block::getFunctionDeclaration(int __d) {

    parser::Ast_Node_Function_Declaration* _ = compiler_declarations->getFunctionDeclaration(__d);

    return (_ || !previous_block) ? _ : previous_block->getFunctionDeclaration(__d);

}

parser::Ast_Node_Struct_Declaration* parser::Compiler_Code_Block::getStructDeclaration(int __d) {

    parser::Ast_Node_Struct_Declaration* _ = compiler_declarations->getStructDeclaration(__d);

    return (_ || !previous_block) ? _ : previous_block->getStructDeclaration(__d);

}

void parser::Compiler_Code_Block::addParameters(Compiler_Control* __comCntrl, utils::LinkedList <Ast_Node*>* _toAdd) {

    parser::Compiler_Code_Block* _prev = __comCntrl->current_compiler_code_block;
    __comCntrl->current_compiler_code_block = this;
    utils::LinkedList <byte_code::Byte_Code*>* _t;

    for (int _ = 0; _ < _toAdd->count; _++) {

        _t = parser::getByteCodeFromNode(
            (*_toAdd)[_], __comCntrl
        );

        byte_code->addFrst(
            _t->frst->object
        );

        _t->destroy_content = 0; delete _t;

    }

    __comCntrl->current_compiler_code_block = _prev;

}

void parser::Compiler_Code_Block::generate(Compiler_Control* __comCntrl, Ast_Node_Code_Block* __astNodeCodeBlock) {

    utils::LinkedList <byte_code::Byte_Code*>* _byteCode = new utils::LinkedList <byte_code::Byte_Code*>();
    parser::Compiler_Code_Block* _ = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block)), *_prev = __comCntrl->current_compiler_code_block;
    utils::LinkedList <byte_code::Byte_Code*>* _nodeByteCode;

    _->environment_id = __astNodeCodeBlock->environment_id;
    _->compiler_declarations = new Compiler_Declarations();
    __comCntrl->current_compiler_code_block = _;

    switch (_->environment_id)
    {
    case AST_NODE_CODE_BLOCK_ENVIRONMENT_NORMAL: case AST_NODE_CODE_BLOCK_ENVIRONMENT_STRUCT:
        _->previous_block = _prev; break;
    default: _->previous_block = NULL; break;
    }

    for (int _i = 0; _i < __astNodeCodeBlock->body->count; _i++) {

        _nodeByteCode = parser::getByteCodeFromNode((*__astNodeCodeBlock->body)[_i], __comCntrl);

        if (!_nodeByteCode) continue;

        _byteCode->join(
            _nodeByteCode
        );

        _nodeByteCode->destroy_content = 0;

        delete _nodeByteCode;

    }

    new (_) Compiler_Code_Block(
        _byteCode
    );

    __comCntrl->current_compiler_code_block = _prev;

    __comCntrl->compiled_code_blocks->add(_);

}

parser::Compiled_Code_Block::~Compiled_Code_Block() { delete compiled_code_block; }

parser::Compiled_Code_Block::Compiled_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* _comByteCode) : compiled_code_block(_comByteCode) {}

parser::Compiled_Output::~Compiled_Output() {
    delete compiled_code_blocks;
    delete implicit_values;
}

parser::Compiled_Output::Compiled_Output(utils::LinkedList <Compiled_Code_Block*>* _comBlocks, utils::LinkedList <char*>* _implValues) : 
    compiled_code_blocks(_comBlocks), implicit_values(_implValues) {}

void parser::Compiled_Output::printByteCode() {

    for (int _ = 0; _ < compiled_code_blocks->count; _++) {

        std::cout << "Code Block: " << _ << "\n" << std::endl;

        for (int __ = 0; __ < (*compiled_code_blocks)[_]->compiled_code_block->count; __++)

            std::cout << "\tByte code -> " << (int) (unsigned char) (*(*compiled_code_blocks)[_]->compiled_code_block)[__]->code << " " << (*(*compiled_code_blocks)[_]->compiled_code_block)[__]->argument << std::endl;

        std::cout << std::endl;

    }

}

parser::Compiler_Exception::Compiler_Exception(const char* _desc) : description(_desc) {
    std::cout << "Compiler generation error: " << description << std::endl; exit(-1);
} 

parser::Compiler_Control::~Compiler_Control() { delete compiled_code_blocks; delete storage; forward_declarations_byte_code->destroy_content = 0; forward_declarations_declaration->destroy_content = 0; delete forward_declarations_byte_code; delete forward_declarations_declaration; }

parser::Compiler_Control::Compiler_Control(utils::LinkedList <Ast_Node_Code_Block*>* __cd, Storage* __strg, bool __dbgInfo) 
    : code_blocks(__cd), storage(__strg), debug_info(__dbgInfo), current_compiler_code_block(NULL) {
        compiled_code_blocks = new utils::LinkedList <Compiler_Code_Block*>();
        forward_declarations_byte_code = new utils::LinkedList <byte_code::Byte_Code*>();
        forward_declarations_declaration = new utils::LinkedList <Ast_Node*>();
}

void parser::Compiler_Control::printDebugInfo(const char* _) {

    if (debug_info) std::cout << _ << std::endl;

}

void parser::Compiler_Control::reviewForwardDeclarations() {

    parser::Ast_Node_Function_Declaration* _funcDecl;

    for (int _ = 0; _ < forward_declarations_declaration->count; _++) {

        switch ((*forward_declarations_declaration)[_]->node_id)
        {
        case AST_NODE_FUNCTION_DECLARATION:

            _funcDecl = (Ast_Node_Function_Declaration*) (*forward_declarations_declaration)[_];
            
            if (!_funcDecl->body) new Compiler_Exception("Function body undefined");

            (*forward_declarations_byte_code)[_]->argument = _funcDecl->body_pos;

            break;
        
        default: new Compiler_Exception("Unexpected Node type in forward delcaration");
        }

    }

}

void parser::Compiler_Control::generateByteCodeBlocks() {

    parser::Compiler_Code_Block::generate(
        this, (*code_blocks)[code_blocks->count - 1] // Global block
    );

    reviewForwardDeclarations();

}

parser::Compiled_Output* parser::Compiler_Control::generateOutPut() {

    parser::Compiled_Output* _rtr;
    utils::LinkedList <Compiled_Code_Block*>* _blocks = new utils::LinkedList <Compiled_Code_Block*>();
    Compiled_Code_Block*  _block;

    for (int _ = 0; _ < compiled_code_blocks->count; _++) {

        _block = (Compiled_Code_Block*) malloc(sizeof(Compiled_Code_Block));

        new (_block) Compiled_Code_Block(
            (*compiled_code_blocks)[_]->byte_code
        );

        _blocks->add(
            _block
        );
    
    }

    _rtr = new Compiled_Output(
        _blocks, storage->implicit_values
    );

    return _rtr;
    
} 


