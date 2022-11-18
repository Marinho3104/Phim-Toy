#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

#include <iostream>

namespace utils { template <typename> struct LinkedList; }

namespace parser {
    
    // Forward
    struct TypeInformation;
    struct Ast_Control;
    struct Environment;

    /* Node default representation for all types of nodes 
    *   Should never be declared by it self !!
    */
    struct Ast_Node { int type; /* Node type */ };

    #define AST_NODE_BLOCK 1
    /* Represent every block of code
    *   
    */
    struct Ast_Node_Block : public Ast_Node {

        utils::LinkedList <Ast_Node*>* body; // Block body
        Environment* environment; // Block environment
        Ast_Node_Block* previous; // Previous Block

        Ast_Node_Block(Ast_Node_Block*, int);
        /* Check if name pos is declared in Block or in previous ones */
        bool checkIfNameIsDeclared(int);
        /* Check if name pos is declared as Variable in Block or in previous ones */
        bool checkVariableNameIsDeclared(int);
        /* Check if name pos is declared as Function in Block or in previous ones */
        bool checkFunctionNameIsDeclared(int);
        /* Check if name pos is declared as Struct in Block or in previous ones */
        bool checkStructNameIsDeclared(int);

        /* Generate block ast */
        void generateBody(Ast_Control*);
        /* Dont know sorry :( */
        static Ast_Node_Block* generate(Ast_Control*, Ast_Node_Block*, int);

    };

    #define AST_NODE_BLOCK_END 2
    /* Represent end of block of code */
    struct Ast_Node_Block_End : public Ast_Node { 
        Ast_Node_Block_End() { type = AST_NODE_BLOCK_END; }; 
        static Ast_Node_Block_End* generate() {
            // std::cout << "--> Block end <--" << std::endl; 
            Ast_Node_Block_End* _ = (Ast_Node_Block_End*) malloc(sizeof(Ast_Node_Block_End)); 
            new(_) Ast_Node_Block_End(); 
            return _;
        } 
    };

    #define AST_NODE_EXPRESSION 3
    /* Represent a expression */
    struct Ast_Node_Expression : public Ast_Node {
        Ast_Node* frst, *scnd; // First and second values
        int expressionIdPos; // Token id of expression
        
        Ast_Node_Expression(Ast_Node*, Ast_Node*, int);
        /**/
        static Ast_Node_Expression* generate(Ast_Control*, Ast_Node_Block*);

    };

    #define AST_NODE_VALUE 4
    /* Represent a implicit value */
    struct Ast_Node_Value : public Ast_Node {
        int valuePos, typeId; // Value position in storage || Token id type
        Ast_Node_Value(int, int);
        /**/
        static Ast_Node_Value* generate(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_VARIABLE_DECLARATION 6
    /**/
    struct Ast_Node_Variable_Declaration : public Ast_Node {
        int typePos, namePos; // Position of type information || Position of name
        Ast_Node* value; // Value assigned to this variable | NULL if no value assigned
        Ast_Node_Variable_Declaration(int, int, Ast_Node*);
        /**/
        static Ast_Node_Variable_Declaration* getVariableDeclarationWithAssign(Ast_Control*, Ast_Node_Block*, int, int, Ast_Node*);
        /**/
        static Ast_Node_Variable_Declaration* getVariableDeclarationWithOutAssign(Ast_Control*, Ast_Node_Block*, int, int);
        /**/
        static utils::LinkedList <Ast_Node*>* generate(Ast_Control*, Ast_Node_Block*, TypeInformation*);
        /**/
        static utils::LinkedList <Ast_Node*>* generateFunctionParameters(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_POINTER_OPERATORS 7
    /**/
    struct Ast_Node_Pointer_Operators : public Ast_Node {

        utils::LinkedList <int>* operatorsBefore; // All operators before variable -> * &
        Ast_Node* value;
        Ast_Node_Pointer_Operators(utils::LinkedList <int>*, Ast_Node*);
        /**/
        static Ast_Node_Pointer_Operators* generate(Ast_Control*, Ast_Node_Block*);

    };

    #define AST_NODE_VARIABLE 8
    /**/
    struct Ast_Node_Variable : public Ast_Node {
        
        int namePos; // Name position
        Ast_Node_Variable(int);
        /**/
        static Ast_Node_Variable* generate(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_VARIABLE_ASSIGNMENT 9
    /**/
    struct Ast_Node_Variable_Assignment : public Ast_Node {
        bool opIsLeft; // Is operator in left side of variable
        int namePos, expressionIdPos; // Name position || Expression Id position
        Ast_Node* value; // Value assigned to this variable || NULL if is single assingment

        Ast_Node_Variable_Assignment(bool, int, int, Ast_Node*);
        /**/
        static Ast_Node_Variable_Assignment* generate(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_PARENTHESIS 10
    /**/
    struct Ast_Node_Parenthesis : public Ast_Node {
        Ast_Node* value; // Value inside parenthesis
        Ast_Node_Parenthesis(Ast_Node*);
        /**/
        static Ast_Node_Parenthesis* generate(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_FUNCTION_DECLARATION 11
    /**/
    struct Ast_Node_Function_Declaration : public Ast_Node {
        int typeRtrPos, namePos; // Type return position || Name position
        utils::LinkedList <Ast_Node*>* parameters; // Parameters of function
        Ast_Node* body; // Function body || NULL If is forward declaration
        Ast_Node_Function_Declaration(int, int, utils::LinkedList <Ast_Node*>*, Ast_Node*);
        /**/
        static Ast_Node_Function_Declaration* generate(Ast_Control*, Ast_Node_Block*, TypeInformation*);
    };

    #define AST_NODE_FUNCTION_CALL 12
    /**/
    struct Ast_Node_Function_Call : public Ast_Node {
        int namePos; // Name position
        utils::LinkedList <Ast_Node*>* parameters; // Parameters of function
        Ast_Node_Function_Call(int, utils::LinkedList <Ast_Node*>*);
        /**/
        static utils::LinkedList <Ast_Node*>* getFunctionCallParameters(Ast_Control*, Ast_Node_Block*);
        /**/
        static Ast_Node_Function_Call* generate(Ast_Control*, Ast_Node_Block*);
    };

    #define AST_NODE_STRUCT_DECLARATION 13
    /**/
    struct Ast_Node_Struct_Declaration : public Ast_Node {
        int namePos; // Struct name position
        bool isContract; // Is contract
        Ast_Node* body; // Body || NULL
        Ast_Node_Struct_Declaration(int, bool, Ast_Node*);
        /**/
        static Ast_Node_Struct_Declaration* generate(Ast_Control*, Ast_Node_Block*);
    };

}

#endif