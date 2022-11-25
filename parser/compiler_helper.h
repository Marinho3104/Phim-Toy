#pragma once

#ifndef PARSER_COMPILER_HELPER_H
#define  PARSER_COMPILER_HELPER_H

namespace parser { struct Ast_Node; struct Ast_Node_Expression; }
namespace utils { template <typename> struct LinkedList; }

namespace parser_helper {

    bool confirmFunctionParameters(utils::LinkedList<parser::Ast_Node *>*, utils::LinkedList<parser::Ast_Node_Expression *>*);

}

#endif

