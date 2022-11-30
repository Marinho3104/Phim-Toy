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

    /* Node that wraps all code inside a namespace */
    struct Ast_Node_Name_Space : public Ast_Node {

        utils::LinkedList <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*, utils::LinkedList <Ast_Node*>*);

        static void generate(Ast_Control*, Name_Space*);

        static Ast_Node_Name_Space* generateByOperator(Ast_Control*, Name_Space*);

        static utils::LinkedList <Ast_Node*>* getDeclarations(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getDeclarationsSingle(Ast_Control*, Name_Space*);
        

    };

    /* Node Code Block */
    struct Ast_Node_Code_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body;
        Ast_Node_Code_Block* previous_block;
        int declaration_type, count_off;
        Name_Tracker* name_tracker;
        Name_Space* name_space;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>*); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*, int);

        static void generate(Ast_Control*);

        static void setUp(Ast_Control*, int);

        void generateBody(Ast_Control*);

        utils::LinkedList <Ast_Node*>* getNewNodes(Ast_Control*, Name_Space*);

        int getCountOffBefore();

        int getDeclarationId(char*);

        bool isDeclarationIdGlobal(char*);

    };

    /* Node Variable Declaration represents a variable declaration to Stack memory */
    struct Ast_Node_Variable_Declaration : public Ast_Node {

        Type_Information* variable_type;
        int declaration_id;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(Type_Information*, int);

        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Type_Information*);

        static utils::LinkedList <Ast_Node*>* generateFunctionParameter(Ast_Control*);

    };

    /* Represent a function declaration */
    struct Ast_Node_Function_Declaration : public Ast_Node {
        
        utils::LinkedList <Ast_Node*>* parameters;
        Name_Space* name_space; // if null means is current name space
        Ast_Node_Code_Block* function_body; // NULL if is forward declaration
        Type_Information* return_type;
        int declaration_id, body_pos;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(utils::LinkedList <Ast_Node*>*, Ast_Node_Code_Block*, Type_Information*, int);

        static utils::LinkedList <Ast_Node*>* getParameters(Ast_Control*);

        static Ast_Node_Function_Declaration* generate(Ast_Control*, Type_Information*, Name_Space*, int);

        // static 

        
    };

    /* Represent a struct declaration */
    struct Ast_Node_Struct_Declaration : public Ast_Node {
        int declaration_id, body_pos; // Struct Declaration id
        bool is_contract; // Is contract
        Name_Space* own_name_space;
        utils::LinkedList <Ast_Node*>* fields;
        utils::LinkedList <Ast_Node_Function_Declaration*>* functions;

        ~Ast_Node_Struct_Declaration(); 
        
        Ast_Node_Struct_Declaration(int, bool, Name_Space*, utils::LinkedList <Ast_Node*>*, utils::LinkedList <Ast_Node_Function_Declaration*>*);
        
        static Ast_Node_Struct_Declaration* generate(Ast_Control*);

        static Name_Space* createNameSpace(Ast_Control*);





        static void ignoreCodeBlock(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getFields(Ast_Control*);

        static utils::LinkedList <Ast_Node_Function_Declaration*>* getFunctions(Ast_Control*, int); 

    };

    /* Node Expression represents any kinda of expression */
    struct Ast_Node_Expression : public Ast_Node {

        int expression_id;
        Ast_Node* first;
        Ast_Node_Expression* second;

        ~Ast_Node_Expression(); Ast_Node_Expression(int, Ast_Node*, Ast_Node_Expression*);

        static Ast_Node_Expression* generate(Ast_Control*, Name_Space*);

        static Ast_Node* getFirstNode(Ast_Control*, Name_Space*);

        static Ast_Node_Expression* getSecondNode(Ast_Control*);

    };

    /* Node Value representa implicit value */
    struct Ast_Node_Value : public Ast_Node {

        int value_position, token_id; // Value position in storage || Token id type

        ~Ast_Node_Value(); Ast_Node_Value(int, int);

        /* Generator */
        static Ast_Node_Value* generate(Ast_Control*);
    
    };

    /* Node Variable represent the use of value of a variable name */
    struct Ast_Node_Variable : public Ast_Node {

        Name_Space* name_space; // if null means is current name space
        int declaration_id;
        bool is_global;

        ~Ast_Node_Variable(); Ast_Node_Variable(Name_Space*, int, bool);

        static Ast_Node_Variable* generate(Ast_Control*, Name_Space*);

    };

    /* Represent a assign to a variable */
    struct Ast_Node_Variable_Assignment : public Ast_Node {

        Ast_Node* value_before_assign;
        Ast_Node_Expression* value;
        bool operation_is_left;
        int expression_id;

        ~Ast_Node_Variable_Assignment(); Ast_Node_Variable_Assignment(Ast_Node*, Ast_Node_Expression*, bool, int);

        static Ast_Node_Variable_Assignment* generate(Ast_Control*);

        static Ast_Node_Variable_Assignment* generate(Ast_Control*, Ast_Node*);

        static Ast_Node* getValueBeforeAssign(Ast_Control*);
        
        static Ast_Node_Expression* getValue(Ast_Control*);

    };

    /* Represent any pointer operation at node */
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        Ast_Node* value;
        int pointer_level;

        ~Ast_Node_Pointer_Operators(); Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);

        static Ast_Node_Pointer_Operators* generate(Ast_Control*, utils::LinkedList <int>*);

        static Ast_Node* getValue(Ast_Control*);

    };

    /* Represent any parenthesis */
    struct Ast_Node_Parenthesis : public Ast_Node {
        
        Ast_Node_Expression* value; // Value inside parenthesis
        
        ~Ast_Node_Parenthesis(); Ast_Node_Parenthesis(Ast_Node_Expression*);

        static Ast_Node_Parenthesis* generate(Ast_Control*);

    };

    /* Represent a function call */
    struct Ast_Node_Function_Call : public Ast_Node {
        utils::LinkedList <Ast_Node_Expression*>* parameters; // Parameters of function
        int declaration_id; // Id
        Name_Space* name_space; // if null read before

        ~Ast_Node_Function_Call(); Ast_Node_Function_Call(int, utils::LinkedList <Ast_Node_Expression*>*, Name_Space*);
        
        static Ast_Node_Function_Call* generate(Ast_Control*, Name_Space*);

        static utils::LinkedList <Ast_Node_Expression*>* getFunctionCallParameters(Ast_Control*);

    };

    /* Represent a accessing variable in struct variable declaration */
    struct Ast_Node_Accessing : public Ast_Node {

        // TODO

    };

}

#endif