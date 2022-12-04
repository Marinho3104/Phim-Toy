#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct LinkedList; }

namespace parser {

    // Forward
    struct Type_Information;
    struct Name_Tracker;
    struct Ast_Control;
    struct Name_Space;

    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int node_id; /* Node type */ virtual ~Ast_Node(); Ast_Node(int); };

    struct Ast_Node_Name_Space : public Ast_Node {

        utils::LinkedList <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        static void generate(Ast_Control*, Name_Space*);

        void setDeclarations(Ast_Control*);

        static utils::LinkedList <parser::Ast_Node*>* getNodes(Ast_Control*, Name_Space*);

    };

    struct Ast_Node_Code_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* code;
        Ast_Node_Code_Block* previous_block;
        Name_Tracker* name_tracker;
        Name_Space* name_space;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(Name_Space*, Ast_Node_Code_Block*);

        static void generate(Ast_Control*);

        static void setUp(Ast_Control*);

        void generateBody(Ast_Control*);

        static utils::LinkedList <parser::Ast_Node*>* getNodes(Ast_Control*, Name_Space*);

        int getDeclarationId(char*);

        bool isDeclarationIdGlobal(char*);

    };

    struct Ast_Node_Variable_Declaration : public Ast_Node {

        Type_Information* type;
        int declaration_id;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(Type_Information*, int);

        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

        static utils::LinkedList <Ast_Node*>* generateFunctionParameter(Ast_Control*, Type_Information*);

    };

    struct Ast_Node_Function_Declaration : public Ast_Node {

        utils::LinkedList <Ast_Node*>* parameters;
        Type_Information* return_type;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;
        int declaration_id;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(Name_Space*, Type_Information*, utils::LinkedList <Ast_Node*>*, Ast_Node_Code_Block*);

        static Ast_Node_Function_Declaration* generate(Ast_Control*, Name_Space*, Type_Information*);

        static utils::LinkedList <Ast_Node*>* getParameters(Ast_Control*);

    };

    struct Ast_Node_Struct_Declaration : public Ast_Node {

        Name_Space* own_name_space;
        int declaration_id;
        bool is_contract;

        utils::LinkedList <Ast_Node*>* fields;
        utils::LinkedList <Ast_Node_Function_Declaration*>* functions;

        ~Ast_Node_Struct_Declaration(); Ast_Node_Struct_Declaration(Name_Space*, int, bool, utils::LinkedList <Ast_Node*>*, utils::LinkedList <Ast_Node_Function_Declaration*>*);

        static Ast_Node_Struct_Declaration* generate(Ast_Control*);

        static void ignoreCodeBlock(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getFields(Ast_Control*);

        static utils::LinkedList <Ast_Node_Function_Declaration*>* getFunctions(Ast_Control*, Type_Information*);

        static void setNewNameSpaceForStruct(Ast_Control*);

    };

}

#endif