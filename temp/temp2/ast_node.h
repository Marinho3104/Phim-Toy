#pragma once

#ifndef PARSER_AST_NODE_H
#define PARSER_AST_NODE_H

namespace utils { template <typename> struct Linked_List; }

namespace parser {

    // Forward ast
    struct Name_Space;

    struct Ast_Node {

        int node_id /* Node Type */, 
        declaration_id /* Node Declaration Id */;

        virtual ~Ast_Node(); Ast_Node(int, int);

    };

    struct Ast_Node_Name_Space : public Ast_Node {

        utils::Linked_List <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        static void generate(Name_Space*);

        static void generate();

        void setDeclarations();

    };

    struct Ast_Node_Code_Block : public Ast_Node {};

    struct Ast_Node_Variable_Declaration : public Ast_Node {

        

    };

    struct Ast_Node_Function_Declaration : public Ast_Node {};

    struct Ast_Node_Struct_Declaration : public Ast_Node {};

}

#endif