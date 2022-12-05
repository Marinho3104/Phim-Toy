#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

// Forward
namespace utils { template <typename> struct LinkedList; template <typename> struct DataLL; }
namespace parser {

    struct Name_Space;
    struct Ast_Control;
    struct Type_Information;

    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;

    struct Ast_Node_Variable;
    struct Ast_Node_Function_Call;
    

}

namespace parser_helper {

    void setPointerOperators(parser::Ast_Control*, utils::LinkedList<int>*, bool);

    parser::Name_Space* checkIfIsNameSpaceChanging(parser::Ast_Control*);

    parser::Name_Space* checkIfIsStructNameSpace(parser::Ast_Control*, utils::LinkedList <char*>*, utils::DataLL <char*>*);

    utils::LinkedList <char*>* getNameSpaceScope(parser::Ast_Control*);

    int getDeclarationId(parser::Ast_Control*);

    bool isDeclarationIdGlobal(parser::Ast_Control*);

    int addToNameTracker(parser::Ast_Control*);

    int addToNameTracker(parser::Ast_Control*, char*);

    parser::Ast_Node_Variable_Declaration* getDeclarationOfVariable(parser::Ast_Control*, parser::Ast_Node_Variable*);

    parser::Ast_Node_Struct_Declaration* getDeclarationOfType(parser::Ast_Control*, parser::Type_Information*);

}  


#endif