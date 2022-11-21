#include "./compiler.h"

#include "./compiler_converter.h" // Convert Nodes into Byte Code
#include "./../utils/linkedList.h" // Linked List
#include "./definitions.h" // Parser Definitions
#include "./ast_nodes.h" // Ast Nodes

#include <iostream>


parser::Compiler_Code_Block::Compiler_Code_Block(utils::LinkedList <byte_code::Byte_Code*>* _byteCode) : byte_code(_byteCode) {}

parser::Compiler_Code_Block* parser::Compiler_Code_Block::generate(Compiler_Control* _comCntrl, int _crrntNode) {

    if ((*_comCntrl->ast)[_crrntNode]->id == AST_NODE_END) return NULL;

    utils::LinkedList <byte_code::Byte_Code*>* _byteCode = new utils::LinkedList <byte_code::Byte_Code*>();
    parser::Compiler_Code_Block* _rtr = (parser::Compiler_Code_Block*) malloc(sizeof(parser::Compiler_Code_Block));

    parser::Ast_Node_Code_Block* _node = (parser::Ast_Node_Code_Block*) (*_comCntrl->ast)[_crrntNode];

    for (int _ = 0; _ < _node->body->count; _++) {

        _byteCode->join(
            parser_helper::getByteCodeFromNode(
                (*_node->body)[_], _comCntrl
            )
        );

    }

    new (_rtr) Compiler_Code_Block(
        _byteCode
    );

    return _rtr;

}


parser::Compiler_Control::Compiler_Control(utils::LinkedList <Ast_Node*>* _ast, Storage* _astStorage) 
    : ast(_ast), ast_storage(_astStorage), code_blocks(new utils::LinkedList <Compiler_Code_Block*>()) {}

void parser::Compiler_Control::generateByteCode() {

    code_blocks->add(
        parser::Compiler_Code_Block::generate(
            this, 0
        )
    );

}