#pragma once

#ifndef PARSER_COMPILER_HELPER_H
#define PARSER_COMPILER_HELPER_H

#include "./ast_nodes.h"

namespace utils { template <typename> struct LinkedList; }

namespace parser { struct Compiler_Control; struct Type_Information; }

namespace parser_helper {

    bool confirmFunctionCallParameters(parser::Compiler_Control*, utils::LinkedList <parser::Ast_Node*>*, utils::LinkedList <parser::Ast_Node_Expression*>*);

    int getSizeOfExpression(parser::Compiler_Control*,parser::Ast_Node_Expression*);

    utils::LinkedList <char*>* getScopeOfPrimitiveType(parser::Type_Information*);

}

#endif
