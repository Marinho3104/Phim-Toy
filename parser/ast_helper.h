#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct LinkedList; }

namespace parser { struct Ast_Control; }

namespace parser_helper {

    void setPointerOperators(parser::Ast_Control*, utils::LinkedList<int>*, bool);

}

#endif