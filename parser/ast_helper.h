#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct Linked_List; }
namespace parser { struct Ast_Control; struct Name_Space; struct Ast_Node; }

namespace parser_helper {

    struct Type_Information {

        int pointer_level, reference_level;
        int token_id, declaration_id;

        parser::Ast_Control* ast_control;
        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Ast_Control*, parser::Name_Space*, int, int, utils::Linked_List <int>*);

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

    int getSizePrimitiveType(int);

}

#endif
