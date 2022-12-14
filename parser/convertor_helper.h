#pragma once

#ifndef PARSER_CONVERTOR_HELPER_H
#define PARSER_CONVERTOR_HELPER_H

#include "./ast_node.h"

namespace utils { template <typename> struct Linked_List; }
namespace byte_code { struct Byte_Code; }
namespace parser { struct Convertor; }

namespace parser_helper {

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeFromNode(parser::Ast_Node*, parser::Convertor*);

    byte_code::Byte_Code* getByteCodeFromNodeVariableDeclaration(parser::Ast_Node_Variable_Declaration*, parser::Convertor*);
    
    void getByteCodeFromNodeFunctionDeclaration(parser::Ast_Node_Function_Declaration*, parser::Convertor*);

}

#endif