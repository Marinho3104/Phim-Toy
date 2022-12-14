#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct Linked_List; }

namespace parser_helper {

    struct Type_Information;

}

namespace parser {

    // Forward
    struct Declaration_Tracker;
    struct Name_Space;

    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;


    struct Ast_Node {
        int node_id, declaration_id;

        virtual ~Ast_Node(); Ast_Node(int, int);

    };

    struct Ast_Node_Name_Space : public Ast_Node {

        utils::Linked_List <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        void setDeclarations();

        static void generate(Name_Space*);

        static void generate();

        int getDeclarationId(char*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Ast_Node_Code_Block : public Ast_Node {

        Ast_Node_Code_Block* previous_code_block;
        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <Ast_Node*>* code;
        Name_Space* name_space;
        int* declarations_off;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*);

        void setCode();

        static void setUp();

        static void generate();

        int getDeclarationId(char*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Ast_Node_Variable_Declaration : public Ast_Node {

        parser_helper::Type_Information* type;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(int, parser_helper::Type_Information*);

        static utils::Linked_List <Ast_Node*>* generate();

        static utils::Linked_List <Ast_Node*>* generateFunctionParameter();

        int getByteSize();

    };

    struct Ast_Node_Function_Declaration : public Ast_Node {

        parser_helper::Type_Information* return_type;
        utils::Linked_List <Ast_Node*>* parameters;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(int, parser_helper::Type_Information*, utils::Linked_List <Ast_Node*>*, Ast_Node_Code_Block*, Name_Space*);

        static Ast_Node_Function_Declaration* generate();

        static utils::Linked_List <Ast_Node*>* getParameters();

        int getByteSize();

    };

    struct Ast_Node_Struct_Declaration : public Ast_Node {};

}

#endif