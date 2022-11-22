#pragma once

#ifndef PARSER_COMPILER_HELPER_H
#define PARSER_COMPILER_HELPER_H

namespace parser { struct Ast_Node; struct Compiler_Control; }
namespace utils { template <typename> struct LinkedList; }

namespace parser_helper {

    bool confirmFunctionParameters(utils::LinkedList<parser::Ast_Node *>*, utils::LinkedList<parser::Ast_Node *>*, parser::Compiler_Control*);

}

#endif