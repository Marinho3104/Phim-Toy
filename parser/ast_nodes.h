#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Type_Information;
    struct Ast_Control;
    struct Name_Space;

    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int node_id; /* Node type */ virtual ~Ast_Node(); Ast_Node(int); };

    /* Node that wraps all code inside a namespace */
    struct Ast_Node_Name_Space : public Ast_Node {

        utils::LinkedList <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*, utils::LinkedList <Ast_Node*>*);

        static void generate(Ast_Control*, Name_Space*);

        static utils::LinkedList <Ast_Node*>* getDeclarations(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getDeclarationsSingle(Ast_Control*);
        

    };

    /* Node Variable Declaration represents a variable declaration to Stack memory */
    struct Ast_Node_Variable_Declaration : public Ast_Node {

        Type_Information* variable_type;
        int declaration_id;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(Type_Information*, int);

        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

    };

}

#endif