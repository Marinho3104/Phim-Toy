#include "./compiler_byte_code_converter.h"

#include "./compiler.h"

#include <iostream>

utils::LinkedList <byte_code::Byte_Code*>* parser::getByteCodeFromNode(Ast_Node* __astNode, Compiler_Control* __comCntrl) {

    std::cout << "Node type -> " << __astNode->node_id << std::endl;

    switch (__astNode->node_id)
    {  
    default: break;
    }

}



