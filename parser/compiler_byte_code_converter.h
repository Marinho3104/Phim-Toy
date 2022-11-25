#pragma once

#ifndef PARSER_COMPILER_BYTE_CODE_CONVERTER_H
#define PARSER_COMPILER_BYTE_CODE_CONVERTER_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }
namespace byte_code { struct Byte_Code; }

namespace parser {

    // Forward
    struct Compiler_Control;

    utils::LinkedList <byte_code::Byte_Code*>* getByteCodeFromNode(Ast_Node*, Compiler_Control*);

}

#endif