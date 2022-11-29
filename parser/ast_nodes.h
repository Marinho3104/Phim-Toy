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

        static utils::LinkedList <Ast_Node*>* getDeclarations(Ast_Control*);

        static utils::LinkedList <Ast_Node*>* getDeclarationsSingle(Ast_Control*);
        

    };

    /* Node Code Block */
    struct Ast_Node_Code_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body;
        Ast_Node_Code_Block* previous_block;
        int declaration_type, count_off;
        Name_Tracker* name_tracker;
        Name_Space* name_space;

        bool last_is_global;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(utils::LinkedList <Ast_Node*>*); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*, int);

        static void generate(Ast_Control*);

        static void setUp(Ast_Control*, int);

        void generateBody(Ast_Control*);

        utils::LinkedList <Ast_Node*>* getNewNodes(Ast_Control*);

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

        static int addToCorrectNameTracker(Ast_Control*);

    };

    /* Represent a function declaration */
    struct Ast_Node_Function_Declaration : public Ast_Node {
        
        utils::LinkedList <Ast_Node*>* parameters;
        Ast_Node_Code_Block* function_body; // NULL if is forward declaration
        Type_Information* return_type;
        int declaration_id;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(utils::LinkedList <Ast_Node*>*, Ast_Node_Code_Block*, Type_Information*, int);

        static utils::LinkedList <Ast_Node*>* getParameters(Ast_Control*);

        static Ast_Node_Function_Declaration* generate(Ast_Control*, Type_Information*);

        // static 

        
    };

    /* Node Expression represents any kinda of expression */
    struct Ast_Node_Expression : public Ast_Node {

        int expression_id;
        Ast_Node* first;
        Ast_Node_Expression* second;

        ~Ast_Node_Expression(); Ast_Node_Expression(int, Ast_Node*, Ast_Node_Expression*);

        static Ast_Node_Expression* generate(Ast_Control*);

        static Ast_Node* getFirstNode(Ast_Control*);

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

        int declaration_id;
        bool is_global;

        ~Ast_Node_Variable(); Ast_Node_Variable(int, bool);

        static Ast_Node_Variable* generate(Ast_Control*);

    };

}

#endif