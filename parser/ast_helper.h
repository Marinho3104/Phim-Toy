#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct Linked_List; }
namespace parser { struct Ast_Control; struct Name_Space; struct Ast_Node; struct Ast_Node_Variable_Declaration; struct Ast_Node_Function_Declaration; struct Ast_Node_Struct_Declaration; }

namespace parser_helper {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* declaration;
        int pointer_level, reference_level;
        int token_id;

        parser::Ast_Control* ast_control;
        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Ast_Control*, parser::Name_Space*, int, parser::Ast_Node_Struct_Declaration*, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        static Type_Information* generate(parser::Ast_Control*, parser::Name_Space*);

        static Type_Information* generate(parser::Ast_Control*, Type_Information*);

        int getByteSize();

        static int getByteSize(int);

    };

    utils::Linked_List <int>* getPointerOperations(parser::Ast_Control*, bool);

    int getNodeType(parser::Ast_Control*);

    parser::Ast_Node* checkNext(parser::Ast_Control*, parser::Ast_Node*);

    utils::Linked_List <char*>* getNameSpaceScope(parser::Ast_Control*);

    parser::Name_Space* getNameSpace(parser::Ast_Control*);

    int addName(parser::Ast_Control*, char*);

    int getDeclarationId(parser::Ast_Control*, char*);

    bool isGlobalDeclaration(parser::Ast_Control*, char*);

    parser::Ast_Node_Variable_Declaration* getVariableDeclaration(parser::Ast_Control*, int);

    parser::Ast_Node_Function_Declaration* getFunctionDeclaration(parser::Ast_Control*, int, utils::Linked_List <parser::Ast_Node*>*);
    
    parser::Ast_Node_Struct_Declaration* getStructDeclaration(parser::Ast_Control*, int);

    bool isDeclared(parser::Ast_Control*, int, utils::Linked_List <parser::Ast_Node*>*);

    int getSizePrimitiveType(int);

}

#endif
