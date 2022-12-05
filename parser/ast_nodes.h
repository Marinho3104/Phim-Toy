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

        Ast_Node_Code_Block* own_code_block;
        Name_Space* own_name_space;
        int declaration_id;
        bool is_contract;

        utils::LinkedList <Ast_Node*>* fields;
        utils::LinkedList <Ast_Node_Function_Declaration*>* functions;

        ~Ast_Node_Struct_Declaration(); Ast_Node_Struct_Declaration(Name_Space*, Ast_Node_Code_Block*, int, bool, utils::LinkedList <Ast_Node*>*, utils::LinkedList <Ast_Node_Function_Declaration*>*);

        static Ast_Node_Struct_Declaration* generate(Ast_Control*);

        static void ignoreCodeBlock(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getFields(Ast_Control*, Ast_Node_Code_Block**);

        static utils::LinkedList <Ast_Node_Function_Declaration*>* getFunctions(Ast_Control*, Type_Information*);

        static void setNewNameSpaceForStruct(Ast_Control*);

    };

    struct Ast_Node_Expression : public Ast_Node {

        Ast_Node* first_expression;
        Ast_Node_Expression* second_expression;
        int expression_operator_id;

        ~Ast_Node_Expression(); Ast_Node_Expression(Ast_Node*, Ast_Node_Expression*, int);

        static Ast_Node_Expression* generate(Ast_Control*, Name_Space*);

        static Ast_Node* getFirstExpression(Ast_Control*, Name_Space*);

    };

    struct Ast_Node_Value : public Ast_Node {

        int value_position, token_id;

        ~Ast_Node_Value(); Ast_Node_Value(int, int);

        static Ast_Node_Value* generate(Ast_Control*);
 
    };

    struct Ast_Node_Variable : public Ast_Node {

        Name_Space* name_space;
        int declaration_id;
        bool is_global;

        ~Ast_Node_Variable(); Ast_Node_Variable(Name_Space*, int, bool);

        static Ast_Node_Variable* generate(Ast_Control*, Name_Space*, int);

    };

    struct Ast_Node_Assignment : public Ast_Node {

        Ast_Node* value_before_assign;
        Ast_Node_Expression* value_after_assign;
        int expression_operator_id;

        ~Ast_Node_Assignment(); Ast_Node_Assignment(Ast_Node*, Ast_Node_Expression*, int);

        static Ast_Node_Assignment* generate(Ast_Control*, Ast_Node*);

        static Ast_Node_Assignment* generate(Ast_Control*);

    };

    struct Ast_Node_Function_Call : public Ast_Node {

        Name_Space* name_space;
        int declaration_id;

        utils::LinkedList <parser::Ast_Node_Expression*>* parameters;

        ~Ast_Node_Function_Call(); Ast_Node_Function_Call(Name_Space*, int, utils::LinkedList <parser::Ast_Node_Expression*>*);

        static Ast_Node_Function_Call* generate(Ast_Control*, Name_Space*, int);

        static utils::LinkedList <parser::Ast_Node_Expression*>* getParameters(Ast_Control*);

    };

    struct Ast_Node_Pointer_Operator : public Ast_Node {

        Ast_Node* value;
        int pointer_level;

        ~Ast_Node_Pointer_Operator(); Ast_Node_Pointer_Operator(Ast_Node*, utils::LinkedList <int>*);

        static Ast_Node_Pointer_Operator* generate(Ast_Control*);

        static Ast_Node* getValue(Ast_Control*);

    };

    struct Ast_Node_Parenthesis : public Ast_Node {
        
        Ast_Node_Expression* value;
        
        ~Ast_Node_Parenthesis(); Ast_Node_Parenthesis(Ast_Node_Expression*);

        static Ast_Node_Parenthesis* generate(Ast_Control*);

    };

    struct Ast_Node_Accessing : public Ast_Node {

        Ast_Node* value_before_accessing, *value_after_acessing;
        bool pointer_accessing;

        ~Ast_Node_Accessing(); Ast_Node_Accessing(Ast_Node*, Ast_Node*, bool);

        static Ast_Node_Accessing* generator(Ast_Control*, Ast_Node*);

        static Ast_Node_Struct_Declaration* setValueBeforeNameSpace(Ast_Control*, Ast_Node*);

        static int isNameDeclared(Ast_Control*, Ast_Node_Struct_Declaration*);

        static Ast_Node* getAccessingValue(Ast_Control*, int);

    };

}

#endif