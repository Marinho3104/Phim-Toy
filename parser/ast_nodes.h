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

        int getDeclarationId(char*); bool isGlobalDeclaration(char*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Ast_Node_Variable_Declaration : public Ast_Node {

        parser_helper::Type_Information* type;
        bool delete_type;

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

        int body_position;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(int, parser_helper::Type_Information*, utils::Linked_List <Ast_Node*>*, Ast_Node_Code_Block*, Name_Space*);

        static Ast_Node_Function_Declaration* generate();

        static utils::Linked_List <Ast_Node*>* getParameters();

        int getByteSize();

    };

    struct Ast_Node_Struct_Declaration : public Ast_Node {

        Ast_Node_Name_Space* functions;
        Ast_Node_Code_Block* fields;
        bool defined;

        ~Ast_Node_Struct_Declaration(); Ast_Node_Struct_Declaration(int, Ast_Node_Name_Space*, Ast_Node_Code_Block*, bool);

        static void set(Ast_Node_Name_Space*, Ast_Node_Code_Block*);

        static Ast_Node_Struct_Declaration* generate();

        static void ignoreCodeBlock();

        int getByteSize();

    };

    struct Ast_Node_Expression : public Ast_Node {

        Ast_Node_Variable_Declaration* result_type;
        Ast_Node_Expression* expression;
        Ast_Node* value;
        int operator_id;

        ~Ast_Node_Expression(); Ast_Node_Expression(Ast_Node*, Ast_Node_Expression*, int);

        static Ast_Node_Expression* generate(int);

        static Ast_Node* getValue(int);

        static bool checkNext(Ast_Node**);

    };

    struct Ast_Node_Value : public Ast_Node {

        int implicit_value_position, token_id;
        parser_helper::Type_Information* type;

        ~Ast_Node_Value(); Ast_Node_Value(int, int);

        static Ast_Node_Value* generate();

    };

    struct Ast_Node_Variable : public Ast_Node {

        Ast_Node_Variable_Declaration* declaration;
        bool is_global;

        ~Ast_Node_Variable(); Ast_Node_Variable(int, Ast_Node_Variable_Declaration*, bool);

        static Ast_Node_Variable* generate();

    };

    struct Ast_Node_Function_Call : public Ast_Node {

        utils::Linked_List <Ast_Node_Expression*>* parameters;
        Ast_Node_Function_Declaration* declaration;
        
        ~Ast_Node_Function_Call(); Ast_Node_Function_Call(int, utils::Linked_List <Ast_Node_Expression*>*, Ast_Node_Function_Declaration*);

        static Ast_Node_Function_Call* generate();

        static utils::Linked_List <Ast_Node_Expression*>* getParameters();

        static utils::Linked_List <Ast_Node*>* getParametersFromExpressions(utils::Linked_List <Ast_Node_Expression*>*);

    };

    struct Ast_Node_Pointer_Operator : public Ast_Node {

        int pointer_level;
        Ast_Node* value;

        parser_helper::Type_Information* type;

        ~Ast_Node_Pointer_Operator(); Ast_Node_Pointer_Operator(int, Ast_Node*, parser_helper::Type_Information*);

        static Ast_Node_Pointer_Operator* generate();

    };

    struct Ast_Node_Parenthesis : public Ast_Node {

        Ast_Node_Expression* expression;

        ~Ast_Node_Parenthesis(); Ast_Node_Parenthesis(Ast_Node_Expression*);

        static Ast_Node_Parenthesis* generate();

    };

    struct Ast_Node_Assignment : public Ast_Node {

        Ast_Node* target;
        Ast_Node_Expression* expression;
        int token_id;

        ~Ast_Node_Assignment(); Ast_Node_Assignment(Ast_Node*, Ast_Node_Expression*, int);

        static Ast_Node_Assignment* generate(Ast_Node*);

        static Ast_Node_Assignment* generate();

    };

}

#endif