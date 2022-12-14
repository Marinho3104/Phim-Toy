#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Ast node's
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node;

    // Ast 
    struct Name_Space;
}

namespace parser_helper {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* declaration;
        int pointer_level, reference_level;
        int token_id;

        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Name_Space*, int, parser::Ast_Node_Struct_Declaration*, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        static Type_Information* generate();

        static Type_Information* generate(Type_Information*);

        int getByteSize();

        static int getByteSize(int);

    };

    utils::Linked_List <int>* getPointerOperations(bool);

    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>*
        getVariableDeclarationsFromLinkedList(utils::Linked_List <parser::Ast_Node*>*);

    int getNodeType();

    utils::Linked_List <char*>* getScope();

    parser::Name_Space* getNameSpace();

}

#endif