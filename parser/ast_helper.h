#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Control;
    struct Ast_Node;

    struct Name_Space;

}

namespace parser_helper {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* declaration;
        int pointer_level, reference_level, token_id;
        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Name_Space*, parser::Ast_Node_Struct_Declaration*, int, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        static Type_Information* generate();

        static Type_Information* generate(Type_Information*);

        int getByteSize();

    };


    utils::Linked_List <int>* getPointerOperations(bool);

    int getNodeType();

    utils::Linked_List <char*>* getScope();

    parser::Name_Space* getNameSpace(bool);

    int addName(char*);

    parser::Ast_Node_Variable_Declaration* getVariableDeclaration(int);

    parser::Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <parser::Ast_Node*>*);
    
    parser::Ast_Node_Struct_Declaration* getStructDeclaration(int);

    void addVariableDeclaration(parser::Ast_Node_Variable_Declaration*);

    void addFunctionDeclaration(parser::Ast_Node_Function_Declaration*);
    
    void addStructDeclaration(parser::Ast_Node_Struct_Declaration*);

    

}

#endif