#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

// Forward
namespace utils { template <typename> struct LinkedList; }
namespace parser {

    struct Name_Space;
    struct Ast_Control;

}

namespace parser_helper {

    void setPointerOperators(parser::Ast_Control*, utils::LinkedList<int>*, bool);

    parser::Name_Space* checkIfIsNameSpaceChanging(parser::Ast_Control*);

    utils::LinkedList <char*>* getNameSpaceScope(parser::Ast_Control*);

    int getDeclarationId(parser::Ast_Control*);

    int addToNameTracker(parser::Ast_Control*);

    int addToNameTracker(parser::Ast_Control*, char*);

}  


#endif