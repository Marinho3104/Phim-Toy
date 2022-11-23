#include "./compiler.h"

#include "./../utils/linkedList.h" // Linked List
#include "./compiler_converter.h" // Convert Nodes into Byte Code
#include "./compiled_byte_code.h" // Compiler Byte Code
#include "./definitions.h" // Parser Definitions
#include "./ast_nodes.h" // Ast Nodes
#include "./ast.h" // Storage

#include <iostream>


parser::Compiler_Declarations::Compiler_Declarations() : funcDecl(new utils::LinkedList <Ast_Node_Function_Declaration*>()),
        structDecl(new utils::LinkedList <Ast_Node_Struct_Declaration*>()), varDecl(new utils::LinkedList <Ast_Node_Variable_Declaration*>()) {}

parser::Compiler_Code_Block::Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* _byteCode) : byte_code(_byteCode) {}

parser::Compiler_Code_Block* parser::Compiler_Code_Block::generate(Compiler_Control* _comCntrl, Ast_Node_Code_Block* _crrntNode, Compiler_Declarations* _comDecl) {

    // if ((*_comCntrl->ast)[_comCntrl->crrntNode]->id == AST_NODE_END) return NULL;

    utils::LinkedList <byte_code::Byte_Code*>* _byteCode = new utils::LinkedList <byte_code::Byte_Code*>();
    parser::Compiler_Code_Block* _rtr = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));

    parser::Compiler_Code_Block* _previous_previous = _comCntrl->crrntCompileCodeBlock;
    _comCntrl->crrntCompileCodeBlock = _rtr;

    if (!_comDecl) {_rtr->compilerDeclarations = (Compiler_Declarations*) malloc(sizeof(Compiler_Declarations)); new (_rtr->compilerDeclarations) Compiler_Declarations(); }
    else _rtr->compilerDeclarations = _comDecl;

    for (int _ = 0; _ < _crrntNode->body->count; _++) {

        _byteCode->join(
            parser_helper::getByteCodeFromNode(
                (*_crrntNode->body)[_], _comCntrl
            )
        );

    }

    new (_rtr) Compiler_Code_Block(
        _byteCode
    );

    _comCntrl->crrntCompileCodeBlock = _previous_previous;

    return _rtr;

}


parser::Compiler_Control::Compiler_Control(utils::LinkedList <Ast_Node*>* _ast, Storage* _astStorage) 
    : ast(_ast), ast_storage(_astStorage), code_blocks(new utils::LinkedList <Compiler_Code_Block*>()), crrntCompileCodeBlock(NULL) {}

void parser::Compiler_Control::generateByteCode() {

    code_blocks->add(
        parser::Compiler_Code_Block::generate(
            this, (Ast_Node_Code_Block*) (*ast)[ast->count - 1], NULL
        )
    );

}

parser::Compiled_Byte_Code* parser::Compiler_Control::getCompiledByteCode() {

    parser::Compiled_Byte_Code* _rtr = (parser::Compiled_Byte_Code*) malloc(sizeof(parser::Compiled_Byte_Code));

    utils::LinkedList <Byte_Code_Block*>* _code_blocks = new utils::LinkedList <Byte_Code_Block*>();

    for (int _ = 0; _ < code_blocks->count; _++) {

        Byte_Code_Block* _block = (Byte_Code_Block*) malloc(sizeof(Byte_Code_Block));

        new (_block) Byte_Code_Block(
            (*code_blocks)[_]->byte_code
        );

        _code_blocks->add(_block);

    }


    new (_rtr) parser::Compiled_Byte_Code(
        _code_blocks, ast_storage->types, ast_storage->implicitValues
    );

    return _rtr;

}

