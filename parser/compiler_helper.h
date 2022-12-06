#pragma once

#ifndef PARSER_COMPILER_HELPER_H
#define PARSER_COMPILER_HELPER_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }

namespace parser_helper {

    bool confirmFunctionCallParameters(utils::LinkedList <parser::Ast_Node*>*, utils::LinkedList <parser::Ast_Node_Expression*>*);

}

#endif
