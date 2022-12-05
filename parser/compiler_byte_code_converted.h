#pragma once

#ifndef PARSER_COMPILER_BYTE_CODE_CONVERTED_H
#define PARSER_COMPILER_BYTE_CODE_CONVERTED_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Code_Block;
    struct Compiler_Control;
    struct Ast_Node;

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeOfNode(Compiler_Control*, Compiler_Code_Block*, Ast_Node*);

    byte_code::Byte_Code* getByteCodeFromVariableDeclaration(Ast_Node_Variable_Declaration*, Compiler_Code_Block*, Compiler_Control*);

}

#endif
