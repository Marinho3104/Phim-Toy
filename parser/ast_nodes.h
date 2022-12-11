#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

namespace utils { template <typename> struct Linked_List; }
namespace parser_helper { struct Type_Information; }

namespace parser {

    // Forward
    struct Name_Tracker;
    struct Ast_Control;
    struct Name_Space;

    struct Ast_Node { int node_id; virtual ~Ast_Node(); Ast_Node(int); };

    struct Ast_Node_Name_Space : public Ast_Node {

        utils::Linked_List <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        static void generate(Ast_Control*, Name_Space*);

        static void generate(Ast_Control*);

        void setDeclarations(Ast_Control*);

    };
    
    struct Ast_Node_Code_Block : public Ast_Node {

        Ast_Node_Code_Block* previous_code_block;
        utils::Linked_List <Ast_Node*>* code;
        Name_Tracker* name_tracker;
        Name_Space* name_space;
        int* declarations_off;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*);

        int getDeclarationId(char*);

        static void generate(Ast_Control*);

        static void setUp(Ast_Control*);

        void setCode(Ast_Control*);

    };

    struct Ast_Node_Variable_Declaration : public Ast_Node {

        parser_helper::Type_Information* type;
        int declaration_id;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(parser_helper::Type_Information*, int);

        static utils::Linked_List <Ast_Node*>* generate(Ast_Control*);

        static utils::Linked_List <Ast_Node*>* generateFunctionParameter(Ast_Control*);

        int getByteSize();

    };

    struct Ast_Node_Function_Declaration : public Ast_Node {

        parser_helper::Type_Information* return_type;
        utils::Linked_List <Ast_Node*>* parameters;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;
        int declaration_id;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(parser_helper::Type_Information*, utils::Linked_List <Ast_Node*>*, Ast_Node_Code_Block*, Name_Space*, int);

        static Ast_Node_Function_Declaration* generate(Ast_Control*);

        static utils::Linked_List <Ast_Node*>* getParameters(Ast_Control*);

        int getByteSize();

    };

    struct Ast_Node_Struct_Declaration : public Ast_Node {

        Ast_Node_Name_Space* functions;
        Ast_Node_Code_Block* fields;

        int declaration_id;

        ~Ast_Node_Struct_Declaration(); 
        
        Ast_Node_Struct_Declaration(int, Ast_Node_Name_Space*, Ast_Node_Code_Block*);

        static Ast_Node_Struct_Declaration* generate(Ast_Control*);

        static void setFieldsAndFunctions(Ast_Control*, Ast_Node_Name_Space*, Ast_Node_Code_Block*);

        int getByteSize();

    };

}

#endif