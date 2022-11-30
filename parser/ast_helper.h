#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct LinkedList; }

namespace parser { struct Ast_Control; struct Name_Space; }

namespace parser_helper {

    void setPointerOperators(parser::Ast_Control*, utils::LinkedList<int>*, bool);

    parser::Name_Space* getNameSpaceOfStructByDeclarationId(parser::Ast_Control*, parser::Name_Space*, int);

}

#endif